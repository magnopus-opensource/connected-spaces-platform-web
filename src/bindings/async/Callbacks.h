#pragma once

#include "../containers/Array.h"
#include "../containers/Disposal.h"
#include "../containers/List.h"
#include "../containers/Map.h"
#include "../containers/Optional.h"
#include "../containers/String.h"
#include "../testtypes/BindingsTestType.h"
#include "../utils/Handles.h"
#include "../utils/RAIIVal.h"
#include "emscripten/bind.h"
#include "emscripten/val.h"
#include <CSP/Common/Optional.h>
#include <emscripten/wire.h>
#include <functional>
#include <iostream>
#include <type_traits>
#include <utility>
#include <vector>

namespace {

/*
 * Type traits to identify containers, such that we can enrich them with appropriate RAII
 * disposal behaviour when they are callback arguments.
 */

//I think this is pretty much std::type_identity<T>, which we'd use if we were c++20.
template <typename T> struct ContainerLeaf {
    using type = T;
};
template <typename T> struct ContainerLeaf<csp::common::List<T>> : ContainerLeaf<T> { };
template <typename T> struct ContainerLeaf<csp::common::Array<T>> : ContainerLeaf<T> { };
template <typename K, typename V> struct ContainerLeaf<csp::common::Map<K, V>> : ContainerLeaf<V> { };
template <typename T> struct ContainerLeaf<csp::common::Optional<T>> : ContainerLeaf<T> { };
template <typename T> using ContainerLeafT = typename ContainerLeaf<T>::type;

template <typename> struct IsContainerTypeRepresentedAsJSArray : std::false_type { };
template <typename T> struct IsContainerTypeRepresentedAsJSArray<csp::common::List<T>> : std::true_type { };
template <typename T> struct IsContainerTypeRepresentedAsJSArray<csp::common::Array<T>> : std::true_type { };

template <typename> struct IsContainerTypeRepresentedAsJSMap : std::false_type { };
template <typename K, typename V> struct IsContainerTypeRepresentedAsJSMap<csp::common::Map<K, V>> : std::true_type { };

template <typename> struct IsContainerTypeRepresentedAsJSOptional : std::false_type { };
template <typename T> struct IsContainerTypeRepresentedAsJSOptional<csp::common::Optional<T>> : std::true_type { };

/* Depending on the type of Arg, makes an RAIIVal with an appropriate disposal policy */
template <typename Arg> inline bindings::utils::RAIIVal MakeRAIIVal(Arg&& arg)
{
    using DecayedArgT = std::decay_t<Arg>;
    using bindings::utils::RAIIVal;
    constexpr bool isArray = IsContainerTypeRepresentedAsJSArray<DecayedArgT>::value;
    constexpr bool isMap = IsContainerTypeRepresentedAsJSMap<DecayedArgT>::value;
    constexpr bool isOptional = IsContainerTypeRepresentedAsJSOptional<DecayedArgT>::value;
    if constexpr (isArray || isMap || isOptional) {
        if constexpr (std::is_pointer_v<ContainerLeafT<DecayedArgT>>) {
            /* Pointer Container (thus non owning elements), no auto disposal */
            return RAIIVal { bindings::utils::NonOwningValRef(std::forward<Arg>(arg)), RAIIVal::DisposePolicy::NoDisposal };
        } else if constexpr (isArray) {
            /* Value Container that gets represented as JS [], automatically disposed as array after callback invocation */
            return RAIIVal { bindings::utils::NonOwningVal(std::forward<Arg>(arg)), RAIIVal::DisposePolicy::Array };
        } else if constexpr (isMap) {
            /* Value Container that gets represented as JS Map, automatically disposed as map after callback invocation */
            return RAIIVal { bindings::utils::NonOwningVal(std::forward<Arg>(arg)), RAIIVal::DisposePolicy::Map };
        } else if constexpr (isOptional) {
            /* Value Optional, automatically disposed as optional after callback invocation */
            return RAIIVal { bindings::utils::NonOwningVal(std::forward<Arg>(arg)), RAIIVal::DisposePolicy::Optional };
        } else {
            static_assert(false, "Container branch impossible");
        }
    } else if constexpr (std::is_pointer_v<DecayedArgT>) {
        /* Pointer (thus non owning), no auto disposal */
        return RAIIVal { bindings::utils::NonOwningValRef(std::forward<Arg>(arg)), RAIIVal::DisposePolicy::NoDisposal };
    } else if constexpr (std::is_arithmetic_v<DecayedArgT> || std::is_same_v<DecayedArgT, csp::common::String>) {
        /* Primitive arg, no auto disposal, and no need for non-owning, would be non-sensical.*/
        return RAIIVal { emscripten::val(std::forward<Arg>(arg)), RAIIVal::DisposePolicy::NoDisposal };
    } else {
        /* Value, automatically disposed singular element after callback invocation */
        return RAIIVal { bindings::utils::NonOwningVal(std::forward<Arg>(arg)), RAIIVal::DisposePolicy::SingleElement };
    }
}

/*
 * Variadic expansion to invoke the callback with a list of args of an unknown size and type arrangement
 * Use an index sequence to expand the variadic to get the array index
 */
template <typename Callback, std::size_t N, std::size_t... I>
inline void InvokeRAIIGuardedCallback(Callback& cb, std::array<bindings::utils::RAIIVal, N>& args, std::index_sequence<I...>)
{
    /*
     * Expands to cb(args[0].Val, args[1].Val, args[2].Val), etc
     * ... I mean, _actually_ expands to cb(args[I[0]].Val, args[I[1]].Val, args[I[2]].Val), with I = index_sequence{0, 1 ,2} ... but who's counting.
     */
    cb(args[I].Val...);
}

/* MAKE_CALLBACK calls this to produce `ToNativeCallback`, which you call at the bindings site */
inline auto AdaptedRAIINativeCallback(emscripten::val cb)
{
    return [cb](auto&&... args) {
        if constexpr (sizeof...(args) == 0) {
            cb(); // No args, this is easy!
        } else {
            /*
             * A variadic expandable array of RAIIVal types. These vals are the args, and this array goes out of scope after the callback invocation
             * MakeRAIIVal()... expands to a comma seperated list of RAIIVal constructors, by wrapping in {}, we get the std::array initializer
             * list constructor syntax.
             *
             * The mental model to have with variadics is the "..." takes the pattern immediately to the left, find the parameter pack(s) inside it,
             * and repeats the ENTIRE PATTERN once per element of the pack(s), replacing the pack itself with the indexed element of the pack(s).
             * The pattern here is 'MakeRAIIVal(std::forward<decltype(args)>(args))', with 'args' being the parameter pack.
             * Therefore, this might expand to something that looks like :
             *
             * std::array<utils::RAIIVal, 3> raiiArgs = { MakeRAIIVal(std::forward<decltype(args[0])>(args[0])),
             *                                            MakeRAIIVal(std::forward<decltype(args[1])>(args[1])),
             *                                            MakeRAIIVal(std::forward<decltype(args[2])>(args[2])) };
             *
             * Whilst the index operator there is a tad misleading (as there's no actual syntax for directly indexing into pack elements),
             * this is still merely a regular std::array declaration.
             *
             * P.S: Don't be confused by sizeof...(args), this is actually an operator applied to a pack, not a parameter pack expansion itself.
             * I'm not a fan of this choice by the comittee to reuse the syntax, gets in the way of mental model formation imo.
             * What's wrong with declaring a new keyword `sizeofpack` eh? Feels quite aesthetically motivated, especially when you consider fold expressions.
             * https://en.cppreference.com/cpp/language/sizeof...
             */
            std::array<bindings::utils::RAIIVal, sizeof...(args)> raiiArgs = { MakeRAIIVal(std::forward<decltype(args)>(args))... };
            /* Call the JS callback with the argument objects. We provide an index sequence {0,1,2,3} so we can index into the raiiArgs std::array variadically */
            InvokeRAIIGuardedCallback(cb, raiiArgs, std::make_index_sequence<sizeof...(args)> { });
            /* Args falls out of scope, disposal occurs according to disposal policy */
        }
    };
}

}

/*
 * Perform the emscripten incantations to register and adapt a callback, enriching it with typescript types.
 * Firstly, declare a value type, generating a typeID aware `val` type within the emscripten machinery, which is neccesary for typescript generation.
 * Then, inside a bespoke EMSCRIPTEN_BINDINGS block, register this new type to a specific typescript signiature, enriching it so it appears where it is used.
 * Finally, generate a converter function.
 * This converter function converts from the `val` type (which is something Javascript can use and call), and the C++ callback
 * We create a closure which captures the `val` callback, and does nothing but call it with forwarded arguments
 * This adapter closure is what ends up being passed into C++, bridging the two langauges, and allowing C++ to call JS.
 *
 * FullyQualifiedCppCallbackType:  Include the namespace, it'll look something like "TestCallbackNamespace::TestCallbackNoArgs"
 * EmbindCallbackType: An arbitrary name to give the embind type of the callback, pattern is to call it like : "TestCallbackNoArgsJSCallback"
 * TypescriptSig: A string representing the typescript signiature, for the no-arg case, it's just "() => void", a single array callback arg might look like "(BindingsTestType[]) => void"
 */

#define MAKE_CALLBACK(FullyQualifiedCppCallbackType, EmbindCallbackType, TypescriptSig)                                                                                            \
    EMSCRIPTEN_DECLARE_VAL_TYPE(EmbindCallbackType);                                                                                                                               \
    EMSCRIPTEN_BINDINGS(register_##EmbindCallbackType) { emscripten::register_type<EmbindCallbackType>(#EmbindCallbackType, TypescriptSig); }                                      \
    inline FullyQualifiedCppCallbackType ToNativeCallback(EmbindCallbackType cb) { return AdaptedRAIINativeCallback(cb); }
