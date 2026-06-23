#pragma once

#include "CSP/Common/Optional.h"
#include "emscripten/bind.h"
#include "emscripten/val.h"
#include <optional>
#include <string>
#include <type_traits>

namespace emscripten::internal
{

template <typename T> struct TypeID<csp::common::Optional<T>>
{
    static constexpr TYPEID get() { return TypeID<std::optional<T>>::get(); }
};

template <typename T> struct BindingType<csp::common::Optional<T>>
{
    using OptionalBinding = BindingType<std::optional<T>>;
    using WireType = typename OptionalBinding::WireType;

    static WireType toWireType(const csp::common::Optional<T>& opt, rvp::default_tag)
    {
        std::optional<T> stdOpt = opt.HasValue() ? std::make_optional<T>(*opt) : std::nullopt;

        return OptionalBinding::toWireType(stdOpt, rvp::default_tag { });
    }

    static csp::common::Optional<T> fromWireType(WireType v)
    {
        std::optional<T> opt = OptionalBinding::fromWireType(v);

        return opt.has_value() ? csp::common::Optional<T>(*opt) : csp::common::Optional<T>();
    }
};

}
