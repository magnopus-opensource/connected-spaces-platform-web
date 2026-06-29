#pragma once

#include "../testtypes/BindingsTestType.h"
#include "emscripten/bind.h"
#include "emscripten/val.h"
#include <CSP/Common/List.h>
#include <CSP/Common/String.h>
#include <functional>
#include <type_traits>

namespace bindings::callbacks {

/*
 * Containers can store value or pointer types. The only containers that need to be marked Disposable
 * are ones that store value types. If the leaf type is a pointer, it need not be disposable, as the
 * pointer will be non-owning
 *
 * These are used in ToDisposableJSArg, to mark callback args as disposable correctly.
 * Arguably you could build a utility around this to auto-annotate the stuff that returns containers too,
 * avoid having to make the explicit decision to wrap in JSDisposable and put Disposable in the typescript
 * signiature. Might be a good idea to be honest, as it is strange to have this as an automated decision
 * point here, but not in the more direct cases. We only have this because we are incapable of
 * doing it directly in this specific case however.
 */
template <typename T> struct NeedsDisposal : std::bool_constant<!std::is_pointer_v<T>> { };
template <typename U> struct NeedsDisposal<csp::common::List<U>> : NeedsDisposal<U> { };
template <typename U> struct NeedsDisposal<csp::common::Array<U>> : NeedsDisposal<U> { };
template <typename K, typename V> struct NeedsDisposal<csp::common::Map<K, V>> : NeedsDisposal<V> { };

/* Specialize the disposable containers, everything else isn't disposable */
template <typename> struct IsDisposableContainer : std::false_type { };
template <typename U> struct IsDisposableContainer<csp::common::List<U>> : std::true_type { };
template <typename U> struct IsDisposableContainer<csp::common::Array<U>> : std::true_type { };
template <typename K, typename V> struct IsDisposableContainer<csp::common::Map<K, V>> : std::true_type { };

/*
 * Convert an argument to a JSDisposable if neccesary, otherwise just forward it back unchanged.
 */
template <typename T> decltype(auto) ToDisposableJSArg(T&& arg)
{
    using DecayedArgT = std::decay_t<T>;
    if constexpr (IsDisposableContainer<DecayedArgT>::value && NeedsDisposal<DecayedArgT>::value) {
        return bindings::utils::JSDisposable<DecayedArgT> { std::forward<T>(arg) };
    } else if constexpr (std::is_pointer_v<DecayedArgT>) {
        // Single pointer arg is a non-owning borrow (API-wide rule; Material results excepted). Hand JS a
        // reference() proxy so a stray .delete()/GC can't free CSP-owned memory. Mirrors the per-element
        // reference() the container paths use for pointer elements.
        return emscripten::val(arg, emscripten::return_value_policy::reference());
    } else {
        return std::forward<T>(arg);
    }
}
}

/*
 * Perform the emscripten incantations to register and adapt a callback, enriching it with typescript types.
 * Firstly, declare a value type, generating a typeID aware `val` type within the emscripten machinery, which is neccesary for typescript generation.
 * Then, inside a bespoke EMSCRIPTEN_BINDINGS block, register this new type to a specific typescript signiature, enriching it so it appears where it is used.
 * Finally, generate a converter function.
 *   This converter function convertes from the `val` type (which is something Javascript can use and call), and the C++ callback
 *   We create a closure which captures the `val` callback, and does nothing but call it with forwarded arguments
 *   This adapter closure is what ends up being passed into C++, bridging the two langauges, and allowing C++ to call JS.
 */
#define MAKE_CALLBACK(FullyQualifiedCppCallbackType, EmbindCallbackType, TypescriptSig)                                                                                            \
    EMSCRIPTEN_DECLARE_VAL_TYPE(EmbindCallbackType);                                                                                                                               \
    EMSCRIPTEN_BINDINGS(register_##EmbindCallbackType) { emscripten::register_type<EmbindCallbackType>(#EmbindCallbackType, TypescriptSig); }                                      \
    inline FullyQualifiedCppCallbackType ToNativeCallback(EmbindCallbackType cb)                                                                                                   \
    {                                                                                                                                                                              \
        return [cb](auto&&... args) { cb(bindings::callbacks::ToDisposableJSArg(std::forward<decltype(args)>(args))..., emscripten::allow_raw_pointers { }); };                    \
    }

/*
 * At time of writing, all callbacks return `void`, so we have not considered return values.
 *
 * Binding looks like this :
 *
 * .function(
             "CallbackFunctionOnThreadNoArgs",
             +[](CallbacksBindingMechanismsTestType& self, TestCallbackNoArgs callback) { self.CallbackFunctionOnThreadNoArgs(ToNativeCallback(callback)); });
 */

/*
 * These are just test callback declarations. I considered putting this in its own file, but we really don't need many so it just goes here
 * Actually, this probably should be its own file similar to the other test types, if you're seeing this in a PR, I probably did this later.
 * These are inside a namespace to mirror how we're going to get them from CSP, to ensure we can handle qualification.
 */
namespace TestCallbackNamespace {
typedef std::function<void()> TestCallbackNoArgs;
typedef std::function<void(int primitiveArg)> TestCallbackPrimitiveArg;
typedef std::function<void(BindingsTestType* pointerArg)> TestCallbackPointerArg;
typedef std::function<void(BindingsTestType valueArg)> TestCallbackValueArg;
typedef std::function<void(csp::common::List<BindingsTestType> valueContainerArg)> TestCallbackContainerOfValues;
typedef std::function<void(csp::common::List<BindingsTestType*> pointerContainerArg)> TestCallbackContainerOfPointers;
typedef std::function<void(int primitiveArg1, float primitiveArg2)> TestCallbackMultipleArgs;
typedef std::function<void(csp::common::Map<int, csp::common::List<BindingsTestType*>> pointerContainerArg)> TestCallbackNestedContainerOfPointers;
typedef std::function<void(csp::common::Map<int, csp::common::List<BindingsTestType>> pointerContainerArg)> TestCallbackNestedContainerOfValues;
}

/* Test callbacks */
MAKE_CALLBACK(TestCallbackNamespace::TestCallbackNoArgs, TestCallbackNoArgsJSCallback, "() => void")
MAKE_CALLBACK(TestCallbackNamespace::TestCallbackPrimitiveArg, TestCallbackPrimitiveArgJSCallback, "(primitiveArg: number) => void")
MAKE_CALLBACK(TestCallbackNamespace::TestCallbackPointerArg, TestCallbackPointerArgJSCallback, "(pointerArg: BindingsTestType) => void")
MAKE_CALLBACK(TestCallbackNamespace::TestCallbackValueArg, TestCallbackValueArgJSCallback, "(valueArg: BindingsTestType & Disposable) => void")
MAKE_CALLBACK(TestCallbackNamespace::TestCallbackContainerOfValues, TestCallbackContainerOfValuesJSCallback, "(valueContainerArg: BindingsTestType[] & Disposable) => void")
MAKE_CALLBACK(TestCallbackNamespace::TestCallbackContainerOfPointers, TestCallbackContainerOfPointersJSCallback, "(pointerContainerArg: BindingsTestType[]) => void")
MAKE_CALLBACK(TestCallbackNamespace::TestCallbackMultipleArgs, TestCallbackMultipleArgsJSCallback, "(primitiveArg1: number, primitiveArg2: number) => void")
MAKE_CALLBACK(
    TestCallbackNamespace::TestCallbackNestedContainerOfPointers, TestCallbackNestedContainerOfPointersJSCallback, "(pointerContainerArg: Map<number, BindingsTestType[]>) => void")
MAKE_CALLBACK(TestCallbackNamespace::TestCallbackNestedContainerOfValues, TestCallbackNestedContainerOfValuesJSCallback,
    "(pointerContainerArg: Map<number, BindingsTestType[]> & Disposable) => void")

/* Declare all callback types in the API here*/
