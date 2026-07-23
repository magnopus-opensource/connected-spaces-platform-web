#pragma once
#include "../utils/Handles.h"
#include "../utils/JSDisposable.h"
#include "CSP/Common/List.h"
#include "emscripten/bind.h"
#include "emscripten/val.h"
#include <optional>
#include <type_traits>

/*
 * Binding machinery for csp::common::List
 * Include this file, then register bindings to function that consume/return List as follows.
```
EMSCRIPTEN_BINDINGS(MyBindingsModule)
{
emscripten::register_type<csp::common::List<StorageType>>("StorageType[]");
emscripten::register_type<bindings::utils::JSDisposable<csp::common::List<StorageType>>>("(StorageType[] & Disposable)");

emscripten::class_<TypeToBind>("TypeToBind")
    .class_function("create", +[](){ return TypeToBind(); })
    .function("functionThatReturnsList", +[](const TypeToBind& self) {
        return bindings::utils::JSDisposable<csp::common::List<StorageType>>{self.FunctionThatReturnsList()};
    })
    .function("functionThatTakesList(value)", &TypeToBind::FunctionThatTakesList);
}
```
*
* Lists and Arrays are conceptually identical from a JS/TS perspective, they both present as native JS/TS arrays.
*/

/*
 * Bind List to a js/ts type. Will most likely copy each element (js.set calls ToWireType).
 * BindingType handles the C++<->JS marshalling; each instantiation must additionally be
 * registered inside EMSCRIPTEN_BINDINGS via emscripten::register_type<>("...") so embind
 * has a typeid->name entry (otherwise "Missing binding for type" at module init) and the
 * generated .d.ts gets a meaningful element type rather than `any`.
 *
 * For example, to register both an argument type, and a return type that is `using` enabled :
 *  emscripten::register_type<csp::common::List<MyType>>("MyType[]");
 *  emscripten::register_type<bindings::utils::JSDisposable<csp::common::List<MyType>>>("(MyType[] & Disposable)");
 */
namespace emscripten::internal {

template <typename T> struct BindingType<csp::common::List<T>> {
    using ValBinding = BindingType<val>;
    using WireType = ValBinding::WireType;

    // Parameter-path only: no [Symbol.dispose] attached. Returns go through
    // bindings::utils::JSDisposable<csp::common::List<T>> instead.
    static WireType toWireType(const csp::common::List<T>& list, rvp::default_tag)
    {
        val newJSArray = val::array();
        for (size_t i = 0; i < list.Size(); ++i) {
            if constexpr (std::is_pointer_v<T>) {
                // Pointer element: hand JS a non-owning reference to CSP-owned memory.
                // Deleting/disposing the handle will not destroy the C++ object.
                newJSArray.set(i, bindings::utils::NonOwningValRef(list[i]));
            } else {
                // Value element: embind copies it across the boundary.
                // Disposal is necessary (via `using` or otherwise) otherwise these copies are a big leak.
                newJSArray.set(i, list[i]);
            }
        }
        return ValBinding::toWireType(newJSArray, rvp::default_tag { });
    }

    static csp::common::List<T> fromWireType(WireType v)
    {
        val js = ValBinding::fromWireType(v);
        const unsigned len = js["length"].as<unsigned>();
        csp::common::List<T> out(len);
        for (unsigned i = 0; i < len; ++i) {
            if constexpr (std::is_pointer_v<T>) {
                // Pointer element: borrow the raw address from the JS handle (no copy) to pass back to CSP.
                // The array references memory owned elsewhere, so these pointers dangle
                // if the underlying objects are deleted while the array still holds them.
                out.Append(js[i].as<T>(emscripten::allow_raw_pointers()));
            } else {
                // Value element: embind copies it across the boundary.
                out.Append(js[i].as<T>());
            }
        }
        return out;
    }
};

template <typename T> struct BindingType<bindings::utils::JSDisposable<csp::common::List<T>>> {
    using ValBinding = BindingType<val>;
    using WireType = ValBinding::WireType;

    // Return path. Attaches [Symbol.dispose] to allow `using` storage in JS land.
    static WireType toWireType(const bindings::utils::JSDisposable<csp::common::List<T>>& wrapper, rvp::default_tag)
    {
        static_assert(!std::is_pointer_v<T>, "JSDisposable<List<T*>> is forbidden: non-owning pointer containers are not disposable.");

        const auto& list = wrapper.view;
        val newJSArray = val::array();
        for (size_t i = 0; i < list.Size(); ++i) {
            // Value element: embind copies it across the boundary.
            newJSArray.set(i, list[i]);
        }

        // Attach [Symbol.dispose] so JS `using` releases bound handles at scope exit.
        static const val symbolDispose = val::global("Symbol")["dispose"];
        static const val disposeArrayFn = val::module_property("disposeArray");

        // Bind our disposal function with the new array arg to the arrays Symbol.dispose slot.
        // We define it as a non-enumerable property using a descriptor.
        val descriptor = val::object();
        descriptor.set("value", disposeArrayFn.call<val>("bind", val::undefined(), newJSArray));
        descriptor.set("enumerable", false);
        val::global("Object").call<void>("defineProperty", newJSArray, symbolDispose, descriptor);

        return ValBinding::toWireType(newJSArray, rvp::default_tag { });
    }
};
}
