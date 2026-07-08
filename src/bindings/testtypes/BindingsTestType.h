/*
 * Fake bindings just so we can build out a test suite.
 * These may get deleted at the end of the project. It's difficult to test
 * as you go without doing something like this, because much of CSP is
 * interdependent, so you can't really use many real types early on in the
 * bindings migration.
 */
#pragma once

#include "../containers/Array.h"
#include "../containers/List.h"
#include "../containers/Map.h"
#include "../containers/String.h"
#include "../utils/JSDisposable.h"

#include "CSP/Common/Array.h"
#include "CSP/Common/List.h"
#include "CSP/Common/Map.h"
#include "CSP/Common/Optional.h"
#include "CSP/Common/String.h"
#include "emscripten/bind.h"
#include "emscripten/val.h"
#include <string>

/*
 * A nonsense, instrumentable C++ object that we can bind and test mechanisms with.
 */
class BindingsTestType {
public:
    BindingsTestType() { ++AliveCount; }
    BindingsTestType(int value, std::string name) : m_value(value), m_name(std::move(name)) { ++AliveCount; }

    BindingsTestType(const BindingsTestType& other) : m_value(other.m_value), m_name(other.m_name) { ++AliveCount; }
    BindingsTestType(BindingsTestType&& other) noexcept : m_value(other.m_value), m_name(std::move(other.m_name)) { ++AliveCount; }

    /* No Constructor/Destructor dance when doing copy/move assignment, so AliveCount remains stable*/
    BindingsTestType& operator=(const BindingsTestType&) = default;
    BindingsTestType& operator=(BindingsTestType&&) noexcept = default;

    ~BindingsTestType() { --AliveCount; }

    int GetValue() const { return m_value; }
    void SetValue(int value) { m_value = value; }

    const std::string& GetName() const { return m_name; }
    void SetName(std::string name) { m_name = std::move(name); }

    bool operator==(const BindingsTestType& other) const { return m_value == other.m_value && m_name == other.m_name; }

    // Test instrumentation, keep track of how many instances of this object exist
    static inline int AliveCount = 0;

private:
    int m_value = 0;
    std::string m_name;
};

EMSCRIPTEN_BINDINGS(CSPTestTypeBindings)
{

    emscripten::class_<BindingsTestType>("BindingsTestType")
        .class_function(
            "create(value, name)", +[](int value, std::string name) { return BindingsTestType(value, std::move(name)); })
        .property("value", &BindingsTestType::GetValue, &BindingsTestType::SetValue)
        .property("name", &BindingsTestType::GetName, &BindingsTestType::SetName)
        .function("equals", &BindingsTestType::operator==)
        .class_property("aliveCount", &BindingsTestType::AliveCount);

    /*
     * We'll bind all these as functions because we're looking to test the raw mechanisms as they would apply to
     * any old method in the API. However, in a real case you'd probably want to bind getter/setter pairs like
     * this as properties
     *
     * Sidenote: pretty sure the value based returns here are going to involve 2 copies. One into the return
     * value of the method itself, and then once again as the data is copied across the JS/C++ runtime boundaries.
     * We have options here:
     *   - Just don't do value returns to owned data like this out of CSP. Tbh, the value return here is contrived anyhow, the const ref one makes
     * more sense.
     *   - Have move operators on our container types, then the returns are RVO'd and it won't matter anyway.
     *   - Do different container element ownership across the interop boundary. Will look into this, although it seems counter to the theory of how
     * embind wants you to do it.
     */

    // Array
    emscripten::register_type<csp::common::Array<int>>("number[]");
    emscripten::register_type<csp::common::Array<BindingsTestType>>("BindingsTestType[]");
    emscripten::register_type<csp::common::Array<BindingsTestType*>>("(BindingsTestType | null)[]");
    emscripten::register_type<csp::common::Array<csp::common::String>>("string[]");

    // List
    emscripten::register_type<csp::common::List<int>>("number[]");
    emscripten::register_type<csp::common::List<BindingsTestType>>("BindingsTestType[]");
    emscripten::register_type<csp::common::List<BindingsTestType*>>("(BindingsTestType | null)[]");

    // Map
    emscripten::register_type<csp::common::Map<int, int>>("Map<number, number>");
    emscripten::register_type<csp::common::Map<int, BindingsTestType>>("Map<number, BindingsTestType>");
    emscripten::register_type<csp::common::Map<int, BindingsTestType*>>("Map<number, (BindingsTestType | null)>");
    emscripten::register_type<csp::common::Map<csp::common::String, int>>("Map<string, number>");
    emscripten::register_type<csp::common::Map<csp::common::String, csp::common::String>>("Map<string, string>");

    // Return types, allows embinds machinery to emit a different typescript signature for container returns, meaning we can use `using` in a
    // type-checked manner. You need to remember to convert to these types in the returning methods, but you don't need to worry about it for
    // parameters.

    // Array
    emscripten::register_type<bindings::utils::JSDisposable<csp::common::Array<int>>>("(number[] & Disposable)");
    emscripten::register_type<bindings::utils::JSDisposable<csp::common::Array<BindingsTestType>>>("(BindingsTestType[] & Disposable)");
    emscripten::register_type<bindings::utils::JSDisposable<csp::common::Array<csp::common::String>>>("(string[] & Disposable)");

    // List
    emscripten::register_type<bindings::utils::JSDisposable<csp::common::List<int>>>("(number[] & Disposable)");
    emscripten::register_type<bindings::utils::JSDisposable<csp::common::List<BindingsTestType>>>("(BindingsTestType[] & Disposable)");

    // Map
    emscripten::register_type<bindings::utils::JSDisposable<csp::common::Map<int, int>>>("(Map<number, number> & Disposable)");
    emscripten::register_type<bindings::utils::JSDisposable<csp::common::Map<int, BindingsTestType>>>("(Map<number, BindingsTestType> & Disposable)");
    emscripten::register_type<bindings::utils::JSDisposable<csp::common::Map<csp::common::String, int>>>("(Map<string, number> & Disposable)");
    emscripten::register_type<bindings::utils::JSDisposable<csp::common::Map<csp::common::String, csp::common::String>>>("(Map<string, string> & Disposable)");

    // Optional
    emscripten::register_optional<int>();
    emscripten::register_optional<BindingsTestType>();
    emscripten::register_optional<csp::common::String>();
}
