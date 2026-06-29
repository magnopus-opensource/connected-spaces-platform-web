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
#include <CSP/Common/List.h>
#include <CSP/Common/Map.h>

/*
 * A class to provide some fundamental patterns as interfaces to test binding mechanisms.
 * Testing real interfaces may be preferable, but we need to bootstrap.
 * This does not indicate any sort of support set, it's more just a utility type such that
 * we can test binding expressions freely.
 *
 * Focuses on callback interfaces.
 */
class CallbacksBindingMechanismsTestType {
public:
    CallbacksBindingMechanismsTestType() { }

    /* On Thread */
    void CallbackFunctionOnThreadNoArgs(TestCallbackNamespace::TestCallbackNoArgs callback) { callback(); }
    void CallbackFunctionOnThreadPrimitiveArg(TestCallbackNamespace::TestCallbackPrimitiveArg callback) { callback(10); }
    void CallbackFunctionOnThreadPointerArg(TestCallbackNamespace::TestCallbackPointerArg callback) { callback(new BindingsTestType(1, "One")); }
    void CallbackFunctionOnThreadValueArg(TestCallbackNamespace::TestCallbackValueArg callback) { callback(BindingsTestType(1, "One")); }
    void CallbackFunctionOnThreadContainerOfPointers(TestCallbackNamespace::TestCallbackContainerOfPointers callback)
    {
        callback(csp::common::List<BindingsTestType*> { new BindingsTestType(1, "One"), new BindingsTestType(2, "Two") });
    }
    void CallbackFunctionOnThreadContainerOfValues(TestCallbackNamespace::TestCallbackContainerOfValues callback)
    {
        callback(csp::common::List<BindingsTestType> { BindingsTestType(1, "One"), BindingsTestType(2, "Two") });
    }
    void CallbackFunctionOnThreadMultipleArgs(TestCallbackNamespace::TestCallbackMultipleArgs callback) { callback(1, 2); }
    void CallbackFunctionOnThreadNestedContainerOfPointers(TestCallbackNamespace::TestCallbackNestedContainerOfPointers callback)
    {
        callback(csp::common::Map<int, csp::common::List<BindingsTestType*>> {
            { 0, csp::common::List<BindingsTestType*> { new BindingsTestType(1, "One"), new BindingsTestType(2, "Two") } },
            { 1, csp::common::List<BindingsTestType*> { new BindingsTestType(3, "Three"), new BindingsTestType(4, "Four)") } } });
    }
    void CallbackFunctionOnThreadNestedContainerOfValues(TestCallbackNamespace::TestCallbackNestedContainerOfValues callback)
    {
        callback(csp::common::Map<int, csp::common::List<BindingsTestType>> { { 0, csp::common::List<BindingsTestType> { BindingsTestType(1, "One"), BindingsTestType(2, "Two") } },
            { 1, csp::common::List<BindingsTestType> { BindingsTestType(3, "Three"), BindingsTestType(4, "Four)") } } });
    }

    /* Off Thread : TODO*/

private:
};

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
            "callbackFunctionOnThreadMultipleArgs(callback)",
            +[](CallbacksBindingMechanismsTestType& self, TestCallbackMultipleArgsJSCallback callback) { self.CallbackFunctionOnThreadMultipleArgs(ToNativeCallback(callback)); })
        .function(
            "callbackFunctionOnThreadNestedContainerOfPointers(callback)",
            +[](CallbacksBindingMechanismsTestType& self, TestCallbackNestedContainerOfPointersJSCallback callback) {
                self.CallbackFunctionOnThreadNestedContainerOfPointers(ToNativeCallback(callback));
            })
        .function(
            "callbackFunctionOnThreadNestedContainerOfValues(callback)", +[](CallbacksBindingMechanismsTestType& self, TestCallbackNestedContainerOfValuesJSCallback callback) {
                self.CallbackFunctionOnThreadNestedContainerOfValues(ToNativeCallback(callback));
            });
}
