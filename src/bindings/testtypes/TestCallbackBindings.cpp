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

#include <CSP/Common/Optional.h>
#include <iostream>
#include <optional>
#include <thread>
#include <utility>

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
typedef std::function<void(const csp::common::Optional<BindingsTestType>& optionalValueArg)> TestCallbackOptionalOfValue;
typedef std::function<void(const csp::common::Optional<BindingsTestType*>& optionalPointerArg)> TestCallbackOptionalOfPointer;
typedef std::function<void(const csp::common::Optional<csp::common::Array<BindingsTestType>>& optionalOfArrayArg)> TestCallbackOptionalOfArray;
typedef std::function<void(const csp::common::Array<csp::common::Optional<BindingsTestType>>& arrayOfOptionalArg)> TestCallbackArrayOfOptional;
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

csp::common::Optional<BindingsTestType> valueOpt { BindingsTestType(1, "One") };
csp::common::Optional<BindingsTestType*> pointerOpt { new BindingsTestType(1, "One") };
csp::common::Optional<csp::common::Array<BindingsTestType>> optOfArray { { BindingsTestType(1, "One"), BindingsTestType(2, "Two") } };
csp::common::Array<csp::common::Optional<BindingsTestType>> arrayOfOpt { { BindingsTestType(1, "One") }, { BindingsTestType(2, "Two") } };
csp::common::Array<csp::common::Optional<BindingsTestType>> arrayOfSomeNullOpt { nullptr, { BindingsTestType(2, "Two") } };
}

namespace {
template <typename Callable, typename... Args> void CallOnThread(Callable&& callable, Args&&... args) { callable(std::forward<Args>(args)...); }
template <typename Callable, typename... Args> void CallOffThread(Callable&& callable, Args&&... args)
{
    std::thread aThread { std::forward<Callable>(callable), std::forward<Args>(args)... };
    aThread.detach();
}
}

class CallbacksBindingMechanismsTestType {
public:
    CallbacksBindingMechanismsTestType(bool offThread = false) : m_offThread(offThread) { std::cout << m_offThread; }

    /* Dispatched on the calling thread, or on a detached thread, depending on m_offThread. */
    void CallbackFunctionNoArgs(TestCallbackNamespace::TestCallbackNoArgs callback) { m_offThread ? CallOffThread(callback) : CallOnThread(callback); }
    void CallbackFunctionPrimitiveArg(TestCallbackNamespace::TestCallbackPrimitiveArg callback) { m_offThread ? CallOffThread(callback, 10) : CallOnThread(callback, 10); }
    void CallbackFunctionPointerArg(TestCallbackNamespace::TestCallbackPointerArg callback)
    {
        m_offThread ? CallOffThread(callback, singleTypeOnePtr) : CallOnThread(callback, singleTypeOnePtr);
    }
    void CallbackFunctionValueArg(TestCallbackNamespace::TestCallbackValueArg callback)
    {
        m_offThread ? CallOffThread(callback, singleTypeOne) : CallOnThread(callback, singleTypeOne);
    }
    void CallbackFunctionValueArgByConstRef(TestCallbackNamespace::TestCallbackValueArgByConstRef callback)
    {
        m_offThread ? CallOffThread(callback, singleTypeOne) : CallOnThread(callback, singleTypeOne);
    }
    void CallbackFunctionContainerOfPointers(TestCallbackNamespace::TestCallbackContainerOfPointers callback)
    {
        m_offThread ? CallOffThread(callback, pointerArray) : CallOnThread(callback, pointerArray);
    }
    void CallbackFunctionContainerOfValues(TestCallbackNamespace::TestCallbackContainerOfValues callback)
    {
        m_offThread ? CallOffThread(callback, valueArray) : CallOnThread(callback, valueArray);
    }
    void CallbackFunctionContainerOfValuesByConstRef(TestCallbackNamespace::TestCallbackContainerOfValuesByConstRef callback)
    {
        m_offThread ? CallOffThread(callback, valueArray) : CallOnThread(callback, valueArray);
    }
    void CallbackFunctionMultipleArgs(TestCallbackNamespace::TestCallbackMultipleArgs callback) { m_offThread ? CallOffThread(callback, 1, 2) : CallOnThread(callback, 1, 2); }
    void CallbackFunctionNestedContainerOfPointers(TestCallbackNamespace::TestCallbackNestedContainerOfPointers callback)
    {
        m_offThread ? CallOffThread(callback, pointerMap) : CallOnThread(callback, pointerMap);
    }
    void CallbackFunctionNestedContainerOfValues(TestCallbackNamespace::TestCallbackNestedContainerOfValues callback)
    {
        m_offThread ? CallOffThread(callback, valueMap) : CallOnThread(callback, valueMap);
    }
    void CallbackFunctionNestedContainerOfValuesByConstRef(TestCallbackNamespace::TestCallbackNestedContainerOfValuesByConstRef callback)
    {
        m_offThread ? CallOffThread(callback, valueMap) : CallOnThread(callback, valueMap);
    }
    void CallbackFunctionMixedArgs(TestCallbackNamespace::TestCallbackMixedArgs callback)
    {
        m_offThread ? CallOffThread(callback, valueArray, valueArrayTwo, pointerArray, 1, singleTypeOne, singleTypeOnePtr)
                    : CallOnThread(callback, valueArray, valueArrayTwo, pointerArray, 1, singleTypeOne, singleTypeOnePtr);
    }
    void CallbackFunctionValueOpt(TestCallbackNamespace::TestCallbackOptionalOfValue callback)
    {
        m_offThread ? CallOffThread(callback, valueOpt) : CallOnThread(callback, valueOpt);
    }
    void CallbackFunctionPointerOpt(TestCallbackNamespace::TestCallbackOptionalOfPointer callback)
    {
        m_offThread ? CallOffThread(callback, pointerOpt) : CallOnThread(callback, pointerOpt);
    }
    void CallbackFunctionOptOfArray(TestCallbackNamespace::TestCallbackOptionalOfArray callback)
    {
        m_offThread ? CallOffThread(callback, optOfArray) : CallOnThread(callback, optOfArray);
    }
    void CallbackFunctionArrayOfOpt(TestCallbackNamespace::TestCallbackArrayOfOptional callback)
    {
        m_offThread ? CallOffThread(callback, arrayOfOpt) : CallOnThread(callback, arrayOfOpt);
    }
    void CallbackFunctionArrayOfSomeNullOpt(TestCallbackNamespace::TestCallbackArrayOfOptional callback)
    {
        m_offThread ? CallOffThread(callback, arrayOfSomeNullOpt) : CallOnThread(callback, arrayOfSomeNullOpt);
    }
    void CallbackFunctionNullValueOpt(TestCallbackNamespace::TestCallbackOptionalOfValue callback)
    {
        m_offThread ? CallOffThread(callback, nullptr) : CallOnThread(callback, nullptr);
    }
    void CallbackFunctionNullPointerOpt(TestCallbackNamespace::TestCallbackOptionalOfPointer callback)
    {
        m_offThread ? CallOffThread(callback, nullptr) : CallOnThread(callback, nullptr);
    }

private:
    bool m_offThread = false; //If true, we'll call callbacks on a detached thread.
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

MAKE_CALLBACK(TestCallbackNamespace::TestCallbackOptionalOfValue, TestCallbackOptionalOfValueJSCallback, "(valueArg: BindingsTestType | undefined) => void")
MAKE_CALLBACK(TestCallbackNamespace::TestCallbackOptionalOfPointer, TestCallbackOptionalOfPointerJSCallback, "(pointerArg: BindingsTestType | undefined) => void")
MAKE_CALLBACK(TestCallbackNamespace::TestCallbackOptionalOfArray, TestCallbackOptionalOfArrayJSCallback, "(optionalOfArrayArg: BindingsTestType[] | undefined) => void")
MAKE_CALLBACK(TestCallbackNamespace::TestCallbackArrayOfOptional, TestCallbackArrayOfOptionalJSCallback, "(arrayOfOptionalArg: (BindingsTestType | undefined)[]) => void")

EMSCRIPTEN_BINDINGS(CSPCallbacksTestTypeBindings)
{
    emscripten::class_<CallbacksBindingMechanismsTestType>("CallbacksBindingMechanismsTestType")
        .class_function(
            "create", +[] { return CallbacksBindingMechanismsTestType(false); })
        .class_function(
            "create(offThread)", +[](bool offThread) { return CallbacksBindingMechanismsTestType(offThread); })
        .function(
            "callbackFunctionNoArgs(callback)",
            +[](CallbacksBindingMechanismsTestType& self, TestCallbackNoArgsJSCallback callback) { self.CallbackFunctionNoArgs(ToNativeCallback(callback)); })
        .function(
            "callbackFunctionPrimitiveArg(callback)",
            +[](CallbacksBindingMechanismsTestType& self, TestCallbackPrimitiveArgJSCallback callback) { self.CallbackFunctionPrimitiveArg(ToNativeCallback(callback)); })
        .function(
            "callbackFunctionPointerArg(callback)",
            +[](CallbacksBindingMechanismsTestType& self, TestCallbackPointerArgJSCallback callback) { self.CallbackFunctionPointerArg(ToNativeCallback(callback)); })
        .function(
            "callbackFunctionValueArg(callback)",
            +[](CallbacksBindingMechanismsTestType& self, TestCallbackValueArgJSCallback callback) { self.CallbackFunctionValueArg(ToNativeCallback(callback)); })
        .function(
            "callbackFunctionValueArgByConstRef(callback)",
            +[](CallbacksBindingMechanismsTestType& self, TestCallbackValueArgByConstRefJSCallback callback) {
                self.CallbackFunctionValueArgByConstRef(ToNativeCallback(callback));
            })
        .function(
            "callbackFunctionContainerOfPointers(callback)",
            +[](CallbacksBindingMechanismsTestType& self, TestCallbackContainerOfPointersJSCallback callback) {
                self.CallbackFunctionContainerOfPointers(ToNativeCallback(callback));
            })
        .function(
            "callbackFunctionContainerOfValues(callback)",
            +[](CallbacksBindingMechanismsTestType& self, TestCallbackContainerOfValuesJSCallback callback) { self.CallbackFunctionContainerOfValues(ToNativeCallback(callback)); })
        .function(
            "callbackFunctionContainerOfValuesByConstRef(callback)",
            +[](CallbacksBindingMechanismsTestType& self, TestCallbackContainerOfValuesByConstRefJSCallback callback) {
                self.CallbackFunctionContainerOfValuesByConstRef(ToNativeCallback(callback));
            })
        .function(
            "callbackFunctionMultipleArgs(callback)",
            +[](CallbacksBindingMechanismsTestType& self, TestCallbackMultipleArgsJSCallback callback) { self.CallbackFunctionMultipleArgs(ToNativeCallback(callback)); })
        .function(
            "callbackFunctionNestedContainerOfPointers(callback)",
            +[](CallbacksBindingMechanismsTestType& self, TestCallbackNestedContainerOfPointersJSCallback callback) {
                self.CallbackFunctionNestedContainerOfPointers(ToNativeCallback(callback));
            })
        .function(
            "callbackFunctionNestedContainerOfValues(callback)",
            +[](CallbacksBindingMechanismsTestType& self, TestCallbackNestedContainerOfValuesJSCallback callback) {
                self.CallbackFunctionNestedContainerOfValues(ToNativeCallback(callback));
            })
        .function(
            "callbackFunctionNestedContainerOfValuesByConstRef(callback)",
            +[](CallbacksBindingMechanismsTestType& self, TestCallbackNestedContainerOfValuesByConstRefJSCallback callback) {
                self.CallbackFunctionNestedContainerOfValuesByConstRef(ToNativeCallback(callback));
            })
        .function(
            "callbackFunctionMixedArgs(callback)",
            +[](CallbacksBindingMechanismsTestType& self, TestCallbackMixedArgsJSCallback callback) { self.CallbackFunctionMixedArgs(ToNativeCallback(callback)); })
        .function(
            "callbackFunctionValueOpt(callback)",
            +[](CallbacksBindingMechanismsTestType& self, TestCallbackOptionalOfValueJSCallback callback) { self.CallbackFunctionValueOpt(ToNativeCallback(callback)); })
        .function(
            "callbackFunctionPointerOpt(callback)",
            +[](CallbacksBindingMechanismsTestType& self, TestCallbackOptionalOfPointerJSCallback callback) { self.CallbackFunctionPointerOpt(ToNativeCallback(callback)); })
        .function(
            "callbackFunctionOptOfArray(callback)",
            +[](CallbacksBindingMechanismsTestType& self, TestCallbackOptionalOfArrayJSCallback callback) { self.CallbackFunctionOptOfArray(ToNativeCallback(callback)); })
        .function(
            "callbackFunctionArrayOfOpt(callback)",
            +[](CallbacksBindingMechanismsTestType& self, TestCallbackArrayOfOptionalJSCallback callback) { self.CallbackFunctionArrayOfOpt(ToNativeCallback(callback)); })
        .function(
            "callbackFunctionArrayOfSomeNullOpt(callback)",
            +[](CallbacksBindingMechanismsTestType& self, TestCallbackArrayOfOptionalJSCallback callback) { self.CallbackFunctionArrayOfSomeNullOpt(ToNativeCallback(callback)); })
        .function(
            "callbackFunctionNullValueOpt(callback)",
            +[](CallbacksBindingMechanismsTestType& self, TestCallbackOptionalOfValueJSCallback callback) { self.CallbackFunctionNullValueOpt(ToNativeCallback(callback)); })
        .function(
            "callbackFunctionNullPointerOpt(callback)",
            +[](CallbacksBindingMechanismsTestType& self, TestCallbackOptionalOfPointerJSCallback callback) { self.CallbackFunctionNullPointerOpt(ToNativeCallback(callback)); });
}
