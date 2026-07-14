/*
 * Fake bindings just so we can build out a test suite.
 * These may get deleted at the end of the project. It's difficult to test
 * as you go without doing something like this, because much of CSP is
 * interdependent, so you can't really use many real types early on in the
 * bindings migration.
 *
 * This one is for callbacks.
 */

#include "../async/Callbacks.h"
#include "../utils/JSDisposable.h"
#include "BindingsTestType.h"

#include "emscripten/bind.h"
#include "emscripten/val.h"
#include <CSP/Common/Array.h>
#include <CSP/Common/Map.h>

#include <iostream>

/*
 * A class to provide some fundamental patterns as interfaces to test binding mechanisms.
 * Testing real interfaces may be preferable, but we need to bootstrap.
 * This does not indicate any sort of support set, it's more just a utility type such that
 * we can test binding expressions freely.
 *
 * Focuses on callback interfaces.
 */

/*
 * These wouldn't exist normally, these would be defined in CSP and we'd just use them. They're here merely as test proxies
 * These are inside a namespace to mirror how we're going to get them from CSP, to ensure we can handle qualification.
 */
namespace TestCallbackNamespace {
typedef std::function<void()> TestCallbackNoArgs;
typedef std::function<void(int primitiveArg)> TestCallbackPrimitiveArg;
typedef std::function<void(BindingsTestType* pointerArg)> TestCallbackPointerArg;
typedef std::function<void(BindingsTestType valueArg)> TestCallbackValueArg;
typedef std::function<void(const BindingsTestType& valueArg)> TestCallbackValueArgByConstRef;
typedef std::function<void(csp::common::Array<BindingsTestType> valueContainerArg)> TestCallbackContainerOfValues;
typedef std::function<void(const csp::common::Array<BindingsTestType>& valueContainerArg)> TestCallbackContainerOfValuesByConstRef;
typedef std::function<void(csp::common::Array<BindingsTestType*> pointerContainerArg)> TestCallbackContainerOfPointers;
typedef std::function<void(int primitiveArg1, float primitiveArg2)> TestCallbackMultipleArgs;
typedef std::function<void(csp::common::Map<int, csp::common::Array<BindingsTestType*>> pointerContainerArg)> TestCallbackNestedContainerOfPointers;
typedef std::function<void(csp::common::Map<int, csp::common::Array<BindingsTestType>> pointerContainerArg)> TestCallbackNestedContainerOfValues;
typedef std::function<void(const csp::common::Map<int, csp::common::Array<BindingsTestType>>& pointerContainerArg)> TestCallbackNestedContainerOfValuesByConstRef;
typedef std::function<void(csp::common::Array<BindingsTestType> valueContainerArg, const csp::common::Array<BindingsTestType>& valueContainerArgByConstRef,
    csp::common::Array<BindingsTestType*> pointerContainerArg, int primitiveArg, BindingsTestType valueArg, BindingsTestType* pointerArg)>
    TestCallbackMixedArgs;
}

namespace {

/*
 * We want the tests to only care about the aliveCount the bindings introduce.
 * Therefore, build all the test types only once up front, so the aliveCount dosen't get
 * confusing during the tests.
 */

BindingsTestType singleTypeOne { 1, "One" };
BindingsTestType singleTypeTwo { 1, "One" };

BindingsTestType* singleTypeOnePtr = new BindingsTestType { 1, "One" };
BindingsTestType* singleTypeTwoPtr = new BindingsTestType { 1, "One" };

csp::common::Array<BindingsTestType> valueArray { BindingsTestType(1, "One"), BindingsTestType(2, "Two") };
csp::common::Array<BindingsTestType> valueArrayTwo { BindingsTestType(3, "Three"), BindingsTestType(4, "Four") };
csp::common::Array<BindingsTestType*> pointerArray { new BindingsTestType(1, "One"), new BindingsTestType(2, "Two") };
csp::common::Map<int, csp::common::Array<BindingsTestType*>> pointerMap {
    { 0, csp::common::Array<BindingsTestType*> { new BindingsTestType(1, "One"), new BindingsTestType(2, "Two") } },
    { 1, csp::common::Array<BindingsTestType*> { new BindingsTestType(3, "Three"), new BindingsTestType(4, "Four)") } }
};
csp::common::Map<int, csp::common::Array<BindingsTestType>> valueMap { { 0, csp::common::Array<BindingsTestType> { BindingsTestType(1, "One"), BindingsTestType(2, "Two") } },
    { 1, csp::common::Array<BindingsTestType> { BindingsTestType(3, "Three"), BindingsTestType(4, "Four)") } } };
}

class CallbacksBindingMechanismsTestType {
public:
    CallbacksBindingMechanismsTestType() { }

    /* On Thread */
    void CallbackFunctionOnThreadNoArgs(TestCallbackNamespace::TestCallbackNoArgs callback) { callback(); }
    void CallbackFunctionOnThreadPrimitiveArg(TestCallbackNamespace::TestCallbackPrimitiveArg callback) { callback(10); }
    void CallbackFunctionOnThreadPointerArg(TestCallbackNamespace::TestCallbackPointerArg callback) { callback(singleTypeOnePtr); }
    void CallbackFunctionOnThreadValueArg(TestCallbackNamespace::TestCallbackValueArg callback) { callback(singleTypeOne); }
    void CallbackFunctionOnThreadValueArgByConstRef(TestCallbackNamespace::TestCallbackValueArgByConstRef callback) { callback(singleTypeOne); }
    void CallbackFunctionOnThreadContainerOfPointers(TestCallbackNamespace::TestCallbackContainerOfPointers callback) { callback(pointerArray); }
    void CallbackFunctionOnThreadContainerOfValues(TestCallbackNamespace::TestCallbackContainerOfValues callback) { callback(valueArray); }
    void CallbackFunctionOnThreadContainerOfValuesByConstRef(TestCallbackNamespace::TestCallbackContainerOfValuesByConstRef callback) { callback(valueArray); }
    void CallbackFunctionOnThreadMultipleArgs(TestCallbackNamespace::TestCallbackMultipleArgs callback) { callback(1, 2); }
    void CallbackFunctionOnThreadNestedContainerOfPointers(TestCallbackNamespace::TestCallbackNestedContainerOfPointers callback) { callback(pointerMap); }
    void CallbackFunctionOnThreadNestedContainerOfValues(TestCallbackNamespace::TestCallbackNestedContainerOfValues callback) { callback(valueMap); }
    void CallbackFunctionOnThreadNestedContainerOfValuesByConstRef(TestCallbackNamespace::TestCallbackNestedContainerOfValuesByConstRef callback) { callback(valueMap); }
    void CallbackFunctionOnThreadMixedArgs(TestCallbackNamespace::TestCallbackMixedArgs callback)
    {
        callback(valueArray, valueArrayTwo, pointerArray, 1, singleTypeOne, singleTypeOnePtr);
    }

    /* Off Thread : TODO*/

private:
};

/*
 * Test callback declarations, using the MAKE_CALLBACK utility.
 * Doing this defines the appropriate `ToNativeCallback` overload for us, as well as imbuing the callbacks with typescript types.
 * Note how nothing is disposable, because of the RAII mechanisms in place, they shouldn't be.
 */
MAKE_CALLBACK(TestCallbackNamespace::TestCallbackNoArgs, TestCallbackNoArgsJSCallback, "() => void")
MAKE_CALLBACK(TestCallbackNamespace::TestCallbackPrimitiveArg, TestCallbackPrimitiveArgJSCallback, "(primitiveArg: number) => void")
MAKE_CALLBACK(TestCallbackNamespace::TestCallbackPointerArg, TestCallbackPointerArgJSCallback, "(pointerArg: BindingsTestType) => void")
MAKE_CALLBACK(TestCallbackNamespace::TestCallbackValueArg, TestCallbackValueArgJSCallback, "(valueArg: BindingsTestType) => void")
MAKE_CALLBACK(TestCallbackNamespace::TestCallbackValueArgByConstRef, TestCallbackValueArgByConstRefJSCallback, "(valueArg: BindingsTestType) => void")
MAKE_CALLBACK(TestCallbackNamespace::TestCallbackContainerOfValues, TestCallbackContainerOfValuesJSCallback, "(valueContainerArg: BindingsTestType[]) => void")
MAKE_CALLBACK(TestCallbackNamespace::TestCallbackContainerOfValuesByConstRef, TestCallbackContainerOfValuesByConstRefJSCallback, "(valueContainerArg: BindingsTestType[]) => void")
MAKE_CALLBACK(TestCallbackNamespace::TestCallbackContainerOfPointers, TestCallbackContainerOfPointersJSCallback, "(pointerContainerArg: BindingsTestType[]) => void")
MAKE_CALLBACK(TestCallbackNamespace::TestCallbackMultipleArgs, TestCallbackMultipleArgsJSCallback, "(primitiveArg1: number, primitiveArg2: number) => void")
MAKE_CALLBACK(
    TestCallbackNamespace::TestCallbackNestedContainerOfPointers, TestCallbackNestedContainerOfPointersJSCallback, "(pointerContainerArg: Map<number, BindingsTestType[]>) => void")
MAKE_CALLBACK(
    TestCallbackNamespace::TestCallbackNestedContainerOfValues, TestCallbackNestedContainerOfValuesJSCallback, "(pointerContainerArg: Map<number, BindingsTestType[]>) => void")
MAKE_CALLBACK(TestCallbackNamespace::TestCallbackNestedContainerOfValuesByConstRef, TestCallbackNestedContainerOfValuesByConstRefJSCallback,
    "(pointerContainerArg: Map<number, BindingsTestType[]>) => void")
MAKE_CALLBACK(TestCallbackNamespace::TestCallbackMixedArgs, TestCallbackMixedArgsJSCallback,
    "(valueContainerArg: BindingsTestType[] , valueContainerArgByConstRef: BindingsTestType[] , pointerContainerArg: BindingsTestType[], primitiveArg: "
    "number, valueArg: BindingsTestType, pointerArg: BindingsTestType) => void")

EMSCRIPTEN_BINDINGS(CSPCallbacksTestTypeBindings)
{
    emscripten::class_<CallbacksBindingMechanismsTestType>("CallbacksBindingMechanismsTestType")
        .class_function(
            "create", +[]() { return CallbacksBindingMechanismsTestType(); })
        .function(
            "callbackFunctionOnThreadNoArgs(callback)",
            +[](CallbacksBindingMechanismsTestType& self, TestCallbackNoArgsJSCallback callback) { self.CallbackFunctionOnThreadNoArgs(ToNativeCallback(callback)); })
        .function(
            "callbackFunctionOnThreadPrimitiveArg(callback)",
            +[](CallbacksBindingMechanismsTestType& self, TestCallbackPrimitiveArgJSCallback callback) { self.CallbackFunctionOnThreadPrimitiveArg(ToNativeCallback(callback)); })
        .function(
            "callbackFunctionOnThreadPointerArg(callback)",
            +[](CallbacksBindingMechanismsTestType& self, TestCallbackPointerArgJSCallback callback) { self.CallbackFunctionOnThreadPointerArg(ToNativeCallback(callback)); })
        .function(
            "callbackFunctionOnThreadValueArg(callback)",
            +[](CallbacksBindingMechanismsTestType& self, TestCallbackValueArgJSCallback callback) { self.CallbackFunctionOnThreadValueArg(ToNativeCallback(callback)); })
        .function(
            "callbackFunctionOnThreadValueArgByConstRef(callback)",
            +[](CallbacksBindingMechanismsTestType& self, TestCallbackValueArgByConstRefJSCallback callback) {
                self.CallbackFunctionOnThreadValueArgByConstRef(ToNativeCallback(callback));
            })
        .function(
            "callbackFunctionOnThreadContainerOfPointers(callback)",
            +[](CallbacksBindingMechanismsTestType& self, TestCallbackContainerOfPointersJSCallback callback) {
                self.CallbackFunctionOnThreadContainerOfPointers(ToNativeCallback(callback));
            })
        .function(
            "callbackFunctionOnThreadContainerOfValues(callback)",
            +[](CallbacksBindingMechanismsTestType& self, TestCallbackContainerOfValuesJSCallback callback) {
                self.CallbackFunctionOnThreadContainerOfValues(ToNativeCallback(callback));
            })
        .function(
            "callbackFunctionOnThreadContainerOfValuesByConstRef(callback)",
            +[](CallbacksBindingMechanismsTestType& self, TestCallbackContainerOfValuesByConstRefJSCallback callback) {
                self.CallbackFunctionOnThreadContainerOfValuesByConstRef(ToNativeCallback(callback));
            })
        .function(
            "callbackFunctionOnThreadMultipleArgs(callback)",
            +[](CallbacksBindingMechanismsTestType& self, TestCallbackMultipleArgsJSCallback callback) { self.CallbackFunctionOnThreadMultipleArgs(ToNativeCallback(callback)); })
        .function(
            "callbackFunctionOnThreadNestedContainerOfPointers(callback)",
            +[](CallbacksBindingMechanismsTestType& self, TestCallbackNestedContainerOfPointersJSCallback callback) {
                self.CallbackFunctionOnThreadNestedContainerOfPointers(ToNativeCallback(callback));
            })
        .function(
            "callbackFunctionOnThreadNestedContainerOfValues(callback)",
            +[](CallbacksBindingMechanismsTestType& self, TestCallbackNestedContainerOfValuesJSCallback callback) {
                self.CallbackFunctionOnThreadNestedContainerOfValues(ToNativeCallback(callback));
            })
        .function(
            "callbackFunctionOnThreadNestedContainerOfValuesByConstRef(callback)",
            +[](CallbacksBindingMechanismsTestType& self, TestCallbackNestedContainerOfValuesByConstRefJSCallback callback) {
                self.CallbackFunctionOnThreadNestedContainerOfValuesByConstRef(ToNativeCallback(callback));
            })
        .function(
            "callbackFunctionOnThreadMixedArgs(callback)",
            +[](CallbacksBindingMechanismsTestType& self, TestCallbackMixedArgsJSCallback callback) { self.CallbackFunctionOnThreadMixedArgs(ToNativeCallback(callback)); });
}
