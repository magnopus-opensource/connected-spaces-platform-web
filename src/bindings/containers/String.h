#pragma once
#include "CSP/Common/String.h"
#include "emscripten/bind.h"
#include "emscripten/val.h"
#include <optional>
#include <string>
#include <type_traits>

/*
 * Binding machinery for csp::common::String
 * Include this file, then register bindings to functions that consume/return csp::common::String as
 * normal, for example as follows.
```
EMSCRIPTEN_BINDINGS(MyBindingsModule)
{
emscripten::class_<TypeToBind>("TypeToBind")
  .class_function("create", +[](){ return TypeToBind(); })
  .function("functionThatReturnsString", &TypeToBind::FunctionThatReturnsString)
  .function("functionThatTakesString(value)", &TypeToBind::FunctionThatTakesString);
}
```
*/

/*
 * Bind csp::common::String to a js/ts type.
 * We use the existing std::string bindings to implement this.
 */
namespace emscripten::internal {

template <> struct TypeID<csp::common::String> {
    static constexpr TYPEID get() { return TypeID<std::string>::get(); }
};

template <> struct BindingType<csp::common::String> {
    using StringBinding = BindingType<std::string>;
    using WireType = StringBinding::WireType;

    static WireType toWireType(const csp::common::String& str, rvp::default_tag)
    {
        // Explicitly construct std::string using length to avoid truncation at the first \0 character
        std::string stdStr(str.c_str(), str.Length());

        return StringBinding::toWireType(stdStr, rvp::default_tag { });
    }

    static csp::common::String fromWireType(WireType v)
    {
        std::string str = StringBinding::fromWireType(v);

        return csp::common::String(str.c_str(), str.length());
    }
};

}
