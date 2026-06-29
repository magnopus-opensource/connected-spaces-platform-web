#pragma once
#include "../utils/JSDisposable.h"
#include "CSP/Common/Map.h"
#include "CSP/Common/String.h"
#include "emscripten/bind.h"
#include "emscripten/val.h"
#include <optional>
#include <type_traits>

/*
 * Binding machinery for csp::common::Map
 * Include this file, then register bindings to function that consume/return List as follows.
```
EMSCRIPTEN_BINDINGS(MyBindingsModule)
{
emscripten::register_type<csp::common::Map<KeyType, StorageType>>("Map<KeyType, StorageType>");
emscripten::register_type<bindings::utils::JSDisposable<csp::common::Map<KeyType, StorageType>>>("(Map<KeyType, STorageType> & Disposable)");

emscripten::class_<TypeToBind>("TypeToBind")
    .class_function("create", +[](){ return TypeToBind(); })
    .function("functionThatReturnsMap", +[](const TypeToBind& self) {
        return bindings::utils::JSDisposable<csp::common::Map<KeyType, StorageType>>{self.FunctionThatReturnsMap()};
    })
    .function("functionThatTakesMap(value)", &TypeToBind::FunctionThatTakesMap);
}
```
* Maps follow a similar philosophy to lists and arrays, attempting to for the most part be
* value types that copy their data. It is for this reason that we currently limit the key type
* in maps to be a primitive, to avoid needing to worry about disposing keys.
*/

namespace bindings::utils {
/*
 * Compile time allow-list of key types that can safely round-trip as JS Map keys.
 *
 * A JS Map compares keys by identity. Only types that embind
 * marshals to a JS primitive behave correctly as keys in terms of equality.
 * Somewhat tangential, although the same root reasoning, we also don't bother
 * to do disposal cleanup of keys, as they should be primitives and thus not
 * have owning memory implications.
 *
 * Note this probably doesn't currently support enums, but could, just use std::is_enum if that becomes necessary.
 */
template <typename K> struct IsValidMapKey : std::bool_constant<std::is_integral_v<K>> { };
// Exact match, which is almost always what you want for map keys (will reject cv-qualified references)
template <> struct IsValidMapKey<csp::common::String> : std::true_type { };
}

/*
 * Bind Map to a js/ts type. Will most likely copy each element (js.set calls ToWireType).
 * BindingType handles the C++<->JS marshalling; each instantiation must additionally be
 * registered inside EMSCRIPTEN_BINDINGS via emscripten::register_type<>("...") so embind
 * has a typeid->name entry (otherwise "Missing binding for type" at module init) and the
 * generated .d.ts gets a meaningful element type rather than `any`.
 *
 * For example, to register both an argument type, and a return type that is `using` enabled :
 *  emscripten::register_type<csp::common::Map<KeyType, ValueType>>("Map<KeyType, StorageType>");
 *  emscripten::register_type<bindings::utils::CSPMapDisposable<KeyType, ValueType>>("(Map<KeyType, StorageType> & Disposable)");
 */
namespace emscripten::internal {

template <typename Key, typename Value> struct BindingType<csp::common::Map<Key, Value>> {
    // Guards the parameter path. The return path (the JSDisposable specialization below) carries its own
    // copy of this assert, since binding a map return-only never instantiates this specialization.
    static_assert(bindings::utils::IsValidMapKey<Key>::value,
        "csp::common::Map can only be bound with a primitive key type (an integral type, or csp::common::String). "
        "To allow a new primitive-like key, add an IsValidMapKey specialization.");

    using ValBinding = BindingType<val>;
    using WireType = ValBinding::WireType;

    // Parameter-path only: no [Symbol.dispose] attached. Returns go through
    // bindings::utils::JSDisposable<csp::common::Map<Key, Value>> instead.
    static WireType toWireType(const csp::common::Map<Key, Value>& map, rvp::default_tag)
    {
        val newJSMap = val::global("Map").new_();
        for (const auto& [key, value] : map.GetUnderlying()) {
            if constexpr (std::is_pointer_v<Value>) {
                // Pointer element: hand JS a non-owning reference to CSP-owned memory.
                // Deleting/disposing the handle will not destroy the C++ object.
                newJSMap.call<void>("set", key, val(value, emscripten::return_value_policy::reference()));
            } else {
                // Value element: embind copies it across the boundary.
                // Disposal is necessary (via `using` or otherwise) otherwise these copies are a big leak.
                newJSMap.call<void>("set", key, val(value));
            }
        }
        return ValBinding::toWireType(newJSMap, rvp::default_tag { });
    }

    static csp::common::Map<Key, Value> fromWireType(WireType v)
    {
        val js = ValBinding::fromWireType(v);
        csp::common::Map<Key, Value> out;

        // Entries becomes an indexable array of KeyValuePairs, like [[1, "One"],[2, "Two"]];
        val entries = val::global("Array").call<val>("from", js);
        const unsigned len = entries["length"].as<unsigned>();

        for (unsigned i = 0; i < len; ++i) {
            val keyValuePair = entries[i];
            Key key = keyValuePair[0].as<Key>();

            if constexpr (std::is_pointer_v<Value>) {
                // Pointer element: borrow the raw address from the JS handle (no copy) to pass back to CSP.
                // The map references memory owned elsewhere, so these pointers dangle
                // if the underlying objects are deleted while the map still holds them.
                out[key] = keyValuePair[1].as<Value>(emscripten::allow_raw_pointers());
            } else {
                // Value element: embind copies it across the boundary.
                out[key] = keyValuePair[1].as<Value>();
            }
        }
        return out;
    }
};

template <typename Key, typename Value> struct BindingType<bindings::utils::JSDisposable<csp::common::Map<Key, Value>>> {
    // Guards the return path; see the parameter-path specialization above.
    static_assert(bindings::utils::IsValidMapKey<Key>::value,
        "csp::common::Map can only be bound with a primitive key type (an integral type, or csp::common::String). "
        "To allow a new primitive-like key, add an IsValidMapKey specialization.");

    using ValBinding = BindingType<val>;
    using WireType = ValBinding::WireType;

    // Return path. Attaches [Symbol.dispose] to allow `using` storage in JS land.
    static WireType toWireType(const bindings::utils::JSDisposable<csp::common::Map<Key, Value>>& wrapper, rvp::default_tag)
    {
        static_assert(!std::is_pointer_v<Value>, "JSDisposable<Map<K, V*>> is forbidden: non-owning pointer containers are not disposable.");

        const auto& map = wrapper.view;
        val newJSMap = val::global("Map").new_();
        for (const auto& [key, value] : map.GetUnderlying()) {
            // Value element: embind copies it across the boundary.
            // Disposal is necessary (via `using` or otherwise) otherwise these copies are a big leak.
            newJSMap.call<void>("set", key, val(value));
        }

        // Attach [Symbol.dispose] so JS `using` releases bound handles at scope exit.
        static const val symbolDispose = val::global("Symbol")["dispose"];
        static const val disposeMapFn = val::module_property("disposeMap");

        // Bind our disposal function with the new array arg to the arrays Symbol.dispose slot.
        // We define it as a non-enumerable property using a descriptor.
        val descriptor = val::object();
        descriptor.set("value", disposeMapFn.call<val>("bind", val::undefined(), newJSMap));
        descriptor.set("enumerable", false);
        val::global("Object").call<void>("defineProperty", newJSMap, symbolDispose, descriptor);

        return ValBinding::toWireType(newJSMap, rvp::default_tag { });
    }
};
}
