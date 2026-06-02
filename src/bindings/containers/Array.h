#pragma once
#include "emscripten/bind.h"
#include "emscripten/val.h"
#include "CSP/Common/Array.h"
#include <optional>
#include <type_traits>

/*
 * Binding machinery for csp::common::Array 
 * Include this file, then register bindings to function that consume/return Arrays as follows.
```
EMSCRIPTEN_BINDINGS(MyBindingsModule)
{
emscripten::register_type<csp::common::Array<StorageType>>("StorageType[]");
emscripten::register_type<bindings::utils::CSPArrayJSDisposable<StorageType>>("(StorageType[] & Disposable)");

emscripten::class_<TypeToBind>("TypeToBind")
    .class_function("create", +[](){ return TypeToBind(); })
    .function("functionThatReturnsArray", +[](const TypeToBind& self) {
        return bindings::utils::CSPArrayJSDisposable<StorageType>{self.FunctionThatReturnsArray()};
    })
    .function("functionThatTakesArray(value)", &TypeToBind::FunctionThatTakesArray);
}
```
*/

namespace bindings::utils
{
/*
 * Return-only wrapper for arrays crossing C++ -> JS.
 *
 * TS's `using` requires the static type to declare `[Symbol.dispose]`, but embind
 * registers one TS name per C++ type and uses it in both return and parameter
 * positions. We can't make `csp::common::Array<T>` itself appear as `T[] & Disposable`
 * without breaking typescript validation on setters for regular value arrays of primitives.
 * (a plain `[1,2,3]` literal isn't assignable to that type). 
 * So, we route returns through a distinct wrapper type registered
 * as `(T[] & Disposable)`, while `csp::common::Array<T>` stays `T[]` for parameters.
 *
 * This does mean you need to convert to this type at the binding site for returns,
 * which is a trade-off. Forgetting to do this will cause the typescript type checker
 * to disallow you from using `using` when storing an array return.
 * 
 * Supports both owned and non owned memory.
 * In the case of a value return out of CSP `Array<T> Func();`, it populates the optional.
 * In the case of a reference return `Array<T>& Func();`, it points the view to the CSP owned memory directly.
 * You should use `view` to get at the memory here in Wiretype bindings, it'll always point to the right thing.
 * Note that this does not mean that const refs don't copy over the boundary, just that we avoid a copy
 * when using this wrapper type. You could get rid of the `view` and just use `owned` and things would work identically,
 * just with a redundant copy in the bindings for no reason.
 */
 template <typename T>
  class CSPArrayJSDisposable {
      // ownedArray must be declared before arrayView: member init order follows
      // declaration order, and the rvalue ctor binds arrayView to *ownedArray.
      private:
        // In theory, if we hit a reference return that is non-copyable, we could use this as the branching axis
        // for owned/non-owned memory in the wiretype bindings, rather than pointer/value. It would be more
        // honest in a way, albeit more complex conceptually.
        std::optional<csp::common::Array<T>> ownedArray; 
      public:
        //Points to either externally managed memory, or `ownedArray`. Use this in the Wiretype bindings.
        const csp::common::Array<T>& arrayView;

    CSPArrayJSDisposable(csp::common::Array<T>&& array) : ownedArray(std::move(array)), arrayView(*ownedArray) {}
    CSPArrayJSDisposable(const csp::common::Array<T>& array) : ownedArray(std::nullopt), arrayView(array) {}

    CSPArrayJSDisposable(const CSPArrayJSDisposable&) = delete;
    CSPArrayJSDisposable(CSPArrayJSDisposable&&) = delete;
    CSPArrayJSDisposable& operator=(const CSPArrayJSDisposable&) = delete;
    CSPArrayJSDisposable& operator=(CSPArrayJSDisposable&&) = delete;
};
} // namespace bindings::utils

/*
 * Bind Array to a js/ts type. Will most likely copy each element (js.set calls ToWireType).
 * BindingType handles the C++<->JS marshalling; each instantiation must additionally be
 * registered inside EMSCRIPTEN_BINDINGS via emscripten::register_type<>("...") so embind
 * has a typeid->name entry (otherwise "Missing binding for type" at module init) and the
 * generated .d.ts gets a meaningful element type rather than `any`.
 * 
 * For example, to register both an argument type, and a return type that is `using` enabled :
 *  emscripten::register_type<csp::common::Array<MyType>>("MyType[]");
 *  emscripten::register_type<bindings::utils::CSPArrrayDisposable<MyType>>("(MyType[] & Disposable)");
 */
namespace emscripten::internal {

template <typename T>
struct BindingType<csp::common::Array<T>>
{
    using ValBinding = BindingType<val>;
    using WireType   = ValBinding::WireType;

    // Parameter-path only: no [Symbol.dispose] attached. Returns go through
    // bindings::utils::CSPArrayJSDisposable<T> instead.
    static WireType toWireType(const csp::common::Array<T>& arr, rvp::default_tag)
    {
        val newJSArray = val::array();
        for (size_t i = 0; i < arr.Size(); ++i)
        {
            if constexpr (std::is_pointer_v<T>)
            {
                // Pointer element: hand JS a non-owning reference to CSP-owned memory.
                // Deleting/disposing the handle will not destroy the C++ object.
                newJSArray.set(i, arr[i], emscripten::return_value_policy::reference());
            }
            else
            {
                // Value element: embind copies it across the boundary.
                // Disposal is necessary (via `using` or otherwise) otherwise these copies are a big leak.
                newJSArray.set(i, arr[i]);
            }
        }
        return ValBinding::toWireType(newJSArray, rvp::default_tag{});
    }

    static csp::common::Array<T> fromWireType(WireType v)
    {
        val js = ValBinding::fromWireType(v);
        const unsigned len = js["length"].as<unsigned>();
        csp::common::Array<T> out(len);
        for (unsigned i = 0; i < len; ++i)
        {
            if constexpr (std::is_pointer_v<T>)
            {
                // Pointer element: borrow the raw address from the JS handle (no copy) to pass back to CSP.
                // The array references memory owned elsewhere, so these pointers dangle
                // if the underlying objects are deleted while the array still holds them.
                out[i] = js[i].as<T>(emscripten::allow_raw_pointers());
            }
            else
            {
                // Value element: embind copies it across the boundary.
                out[i] = js[i].as<T>();
            }
        }
        return out;
    }
};

template <typename T>
struct BindingType<bindings::utils::CSPArrayJSDisposable<T>>
{
    using ValBinding = BindingType<val>;
    using WireType   = ValBinding::WireType;

    // Return path. Attaches [Symbol.dispose] to allow `using` storage in JS land.
    static WireType toWireType(const bindings::utils::CSPArrayJSDisposable<T>& wrapper, rvp::default_tag)
    {
        const auto& arr = wrapper.arrayView;
        val newJSArray = val::array();
        for (size_t i = 0; i < arr.Size(); ++i)
        {
            if constexpr (std::is_pointer_v<T>)
            {
                // Pointer element: hand JS a NON-OWNING reference to CSP-owned memory.
                newJSArray.set(i, arr[i], emscripten::return_value_policy::reference());
            }
            else
            {
                // Value element: embind copies it across the boundary.
                newJSArray.set(i, arr[i]);
            }
        }

        // Attach [Symbol.dispose] so JS `using` releases bound handles at scope exit.
        static const val symbolDispose = val::global("Symbol")["dispose"];
        static const val disposeArrayFn = val::module_property("disposeArray");
        // bind our disposal function with the new array arg to the arrays Symbol.dispose slot.
        newJSArray.set(symbolDispose, disposeArrayFn.call<val>("bind", val::undefined(), newJSArray));

        return ValBinding::toWireType(newJSArray, rvp::default_tag{});
    }
};
}