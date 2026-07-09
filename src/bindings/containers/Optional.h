#pragma once

#include "../utils/JSDisposable.h"
#include "CSP/Common/Optional.h"
#include "emscripten/bind.h"
#include "emscripten/val.h"
#include <optional>
#include <string>
#include <type_traits>

namespace emscripten::internal {

template <typename T> struct TypeID<csp::common::Optional<T>> {
    static constexpr TYPEID get() { return TypeID<std::optional<T>>::get(); }
};

template <typename T> struct BindingType<csp::common::Optional<T>> {
    using OptionalBinding = BindingType<std::optional<T>>;
    using WireType = typename OptionalBinding::WireType;

    static WireType toWireType(const csp::common::Optional<T>& opt, rvp::default_tag)
    {
        std::optional<T> stdOpt = opt.HasValue() ? std::make_optional<T>(*opt) : std::nullopt;

        return OptionalBinding::toWireType(stdOpt, rvp::default_tag {});
    }

    static csp::common::Optional<T> fromWireType(WireType v)
    {
        std::optional<T> opt = OptionalBinding::fromWireType(v);

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
