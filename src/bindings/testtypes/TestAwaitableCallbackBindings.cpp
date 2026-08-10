/*
 * Fake bindings just so we can build out a test suite.
 * These may get deleted at the end of the project. It's difficult to test
 * as you go without doing something like this, because much of CSP is
 * interdependent, so you can't really use many real types early on in the
 * bindings migration.
 *
 * This one is for awaitable callbacks.
 */

#include "../utils/JSDisposable.h"
#include "../utils/Promises.h"
#include "BindingsTestType.h"

#include "emscripten/bind.h"
#include "emscripten/val.h"
#include <CSP/Common/Array.h>
#include <CSP/Common/Map.h>

#include <CSP/Common/Optional.h>
#include <iostream>
#include <optional>

/*
 * A class to provide some fundamental patterns as interfaces to test binding mechanisms.
 * Testing real interfaces may be preferable, but we need to bootstrap.
 * This does not indicate any sort of support set, it's more just a utility type such that
 * we can test binding expressions freely.
 *
 * Focuses on async callback interfaces.
 */

/*
 * These wouldn't exist normally, these would be defined in CSP and we'd just use them. They're here merely as test proxies
 * These are inside a namespace to mirror how we're going to get them from CSP, to ensure we can handle qualification.
 */
namespace TestAwaitableCallbackNamespace {
typedef std::function<void(int primitiveArg)> TestAwaitableCallbackPrimitiveArg;

typedef std::function<void(BindingsTestType valueArg)> TestAwaitableCallbackValueArg;
typedef std::function<void(const BindingsTestType& valueArg)> TestAwaitableCallbackConstRefArg;

typedef std::function<void(BindingsTestType* pointerArg)> TestAwaitableCallbackPointerArg;
}

namespace {

// /*
//  * We want the tests to only care about the aliveCount the bindings introduce.
//  * Therefore, build all the test types only once up front, so the aliveCount dosen't get
//  * confusing during the tests.
//  */

BindingsTestType singleTypeOne { 1, "One" };
BindingsTestType singleTypeTwo { 2, "Two" };

BindingsTestType* singleTypeOnePtr = new BindingsTestType { 1, "One" };
BindingsTestType* singleTypeTwoPtr = new BindingsTestType { 2, "Two" };
}

class AwaitableCallbacksBindingMechanismsTestType {
public:
    AwaitableCallbacksBindingMechanismsTestType() { }

    void AwaitableCallbackFunctionPrimitiveArg(TestAwaitableCallbackNamespace::TestAwaitableCallbackPrimitiveArg callback) { callback(10); }
    void AwaitableCallbackFunctionPrimitiveMultiArg(int a, int b, TestAwaitableCallbackNamespace::TestAwaitableCallbackPrimitiveArg callback) { callback(a + b); }

    void AwaitableCallbackFunctionValueArg(TestAwaitableCallbackNamespace::TestAwaitableCallbackValueArg callback) { callback(singleTypeOne); }
    void AwaitableCallbackFunctionConstRefArg(TestAwaitableCallbackNamespace::TestAwaitableCallbackConstRefArg callback) { callback(singleTypeOne); }

    void AwaitableCallbackFunctionPointerArg(TestAwaitableCallbackNamespace::TestAwaitableCallbackPointerArg callback) { callback(singleTypeOnePtr); }
};

// TODO: Just declaring here for testing
// Could possibly declare an independent type for each callback
// This can be extracted somewhere else, it doesn't need to be its own EMSCRIPTEN_BINDINGS block
EMSCRIPTEN_DECLARE_VAL_TYPE(PromiseOfNumber);
EMSCRIPTEN_BINDINGS(register_PromiseOfNumber) { emscripten::register_type<PromiseOfNumber>("PromiseOfNumber", "Promise<number>"); }
// For test with disposable promises only
// EMSCRIPTEN_BINDINGS(register_PromiseOfNumber) { emscripten::register_type<PromiseOfNumber>("Promise<number> & Disposable"); }

EMSCRIPTEN_DECLARE_VAL_TYPE(PromiseOfBindingsTestType);
EMSCRIPTEN_BINDINGS(register_PromiseOfBindingsTestType) { emscripten::register_type<PromiseOfBindingsTestType>("PromiseOfBindingsTestType", "Promise<BindingsTestType>"); }
// For test with disposable promises only
// EMSCRIPTEN_BINDINGS(register_PromiseOfBindingsTestType) { emscripten::register_type<PromiseOfBindingsTestType>("Promise<BindingsTestType> & Disposable"); }

namespace {

}

EMSCRIPTEN_BINDINGS(CSPAwaitableCallbacksTestTypeBindings)
{
    emscripten::class_<AwaitableCallbacksBindingMechanismsTestType>("AwaitableCallbacksBindingMechanismsTestType")
        .class_function(
            "create", +[]() { return AwaitableCallbacksBindingMechanismsTestType(); })
        .function(
            "awaitableCallbackFunctionPrimitiveArg",
            +[](AwaitableCallbacksBindingMechanismsTestType& self) {
                return bindings::utils::promisify<int, PromiseOfNumber>([&](auto cb) { self.AwaitableCallbackFunctionPrimitiveArg(cb); });
            })
        .function(
            "awaitableCallbackFunctionPrimitiveMultiArg(a, b)",
            +[](AwaitableCallbacksBindingMechanismsTestType& self, int a, int b) {
                return bindings::utils::promisify<int, PromiseOfNumber>([&](auto cb) { self.AwaitableCallbackFunctionPrimitiveMultiArg(a, b, cb); });
            })
        .function(
            "awaitableCallbackFunctionValueArg",
            +[](AwaitableCallbacksBindingMechanismsTestType& self) {
                return bindings::utils::promisify<BindingsTestType, PromiseOfBindingsTestType>([&](auto cb) { self.AwaitableCallbackFunctionValueArg(cb); });
            })
        .function(
            "awaitableCallbackFunctionConstRefArg",
            +[](AwaitableCallbacksBindingMechanismsTestType& self) {
                return bindings::utils::promisify<BindingsTestType, PromiseOfBindingsTestType>([&](auto cb) { self.AwaitableCallbackFunctionConstRefArg(cb); });
            })
        .function(
            "awaitableCallbackFunctionPointerArg",
            +[](AwaitableCallbacksBindingMechanismsTestType& self) {
                return bindings::utils::promisify<BindingsTestType*, PromiseOfBindingsTestType>([&](auto cb) { self.AwaitableCallbackFunctionPointerArg(cb); });
            })
        .function(
            "awaitableCallbackFunctionValueArgDisposablePromise", +[](AwaitableCallbacksBindingMechanismsTestType& self) {
                return bindings::utils::promisify_disposable_thenable<BindingsTestType, PromiseOfBindingsTestType>([&](auto cb) { self.AwaitableCallbackFunctionValueArg(cb); });
            });
}
