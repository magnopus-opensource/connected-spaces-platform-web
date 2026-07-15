#pragma once

#include "../utils/JSDisposable.h"
#include "CSP/Common/Optional.h"
#include "emscripten/bind.h"
#include "emscripten/val.h"
#include <optional>
#include <type_traits>

/*
 * Binding machinery for csp::common::Optional
 * Include this file, then register bindings to functions that consume/return csp::common::Optional
 * as normal, for example as follows.
```
EMSCRIPTEN_BINDINGS(MyBindingsModule)
{
// Note the use of register_optional instead of register_type
emscripten::register_optional<csp::common::Optional<StorageType>>();

emscripten::class_<TypeToBind>("TypeToBind")
  .class_function("create", +[](){ return TypeToBind(); })
  .function("functionThatReturnsOptional", &TypeToBind::FunctionThatReturnsOptional)
  .function("functionThatTakesOptional(value)", &TypeToBind::FunctionThatTakesOptional);
}
```
*/

namespace emscripten::internal {

template <typename T> struct TypeID<csp::common::Optional<T>> {
    static constexpr TYPEID get() { return TypeID<std::optional<T>>::get(); }
};

template <typename T> struct BindingType<csp::common::Optional<T>> {
    using OptionalBinding = BindingType<std::optional<T>>;
    using WireType = typename OptionalBinding::WireType;

    static WireType toWireType(const csp::common::Optional<T>& opt, rvp::default_tag)
    {
        // The use of std::optional via make_optional here will result in an additional copy of the contained value
        std::optional<T> stdOpt = opt.HasValue() ? std::make_optional<T>(*opt) : std::nullopt;

        return OptionalBinding::toWireType(stdOpt, rvp::default_tag {});
    }

    static csp::common::Optional<T> fromWireType(WireType v)
    {
        std::optional<T> opt = OptionalBinding::fromWireType(v);

        // There will be an additional copy of the contained value here if the value is non-movable
        return opt.has_value() ? csp::common::Optional<T>(std::move(*opt)) : csp::common::Optional<T>();
    }
};

/*
 * Return-path binding for optional with types that need disposable behaviour.
 * Combines optional semantics (value or undefined) with [Symbol.dispose] attachment.
 *
 * A binding for JSDisposable<T> must exist for this to work.
 *
 * Example usage in binding site:
 *   .function("getOptionalList", +[](const MyType& self) {
 *       return bindings::utils::JSDisposable<csp::common::Optional<csp::common::List<T>>>{self.GetOptionalList()};
 *   })
 *
 * Registration:
 *   emscripten::register_type<bindings::utils::JSDisposable<csp::common::Optional<csp::common::List<T>>>>("(T[] & Disposable) | undefined");
 */
template <typename T> struct BindingType<bindings::utils::JSDisposable<csp::common::Optional<T>>> {
    using ValBinding = BindingType<val>;
    using InnerDisposableBinding = BindingType<bindings::utils::JSDisposable<T>>;
    using WireType = typename ValBinding::WireType;

    static WireType toWireType(const bindings::utils::JSDisposable<csp::common::Optional<T>>& wrapper, rvp::default_tag)
    {
        const auto& opt = wrapper.view;

        if (!opt.HasValue()) {
            return ValBinding::toWireType(val::undefined(), rvp::default_tag {});
        }

        // Wrap the inner value in a JSDisposable and delegate to its BindingType
        bindings::utils::JSDisposable<T> innerWrapper(*opt);
        return InnerDisposableBinding::toWireType(innerWrapper, rvp::default_tag {});
    }
};

}
