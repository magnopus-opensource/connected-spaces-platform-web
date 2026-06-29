/*
 * Fake bindings just so we can build out a test suite.
 * These may get deleted at the end of the project. It's difficult to test
 * as you go without doing something like this, because much of CSP is
 * interdependent, so you can't really use many real types early on in the
 * bindings migration.
 *
 * The EMSCRIPTEN_BINDINGS registration for BindingsTestType (and the container
 * register_type calls keyed on it) live here, in exactly one translation unit.
 * The class definition stays in the header so multiple TUs can use the type,
 * but the registration must compile once or embind throws "Cannot register
 * type 'BindingsTestType' twice" at module init.
 */

#include "BindingsTestType.h"
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

    /* Return types, allows embinds machinery to emit a different typescript signature for container returns, meaning we can use `using` in a
     * type-checked manner. You need to remember to convert to these types in the returning methods, but you don't need to worry about it for
     * parameters.
     * As pointers across the API boundary are never ownership-transfer, you don't need to list pointer types here
     */

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
}
