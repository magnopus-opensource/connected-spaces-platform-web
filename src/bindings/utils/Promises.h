#pragma once

#include "../async/Callbacks.h"
#include "Handles.h"
#include "emscripten/bind.h"
#include "emscripten/emscripten.h"
#include "emscripten/val.h"
#include <CSP/Common/String.h>
#include <type_traits>

// Make a JS promise object
EM_JS(emscripten::EM_VAL, make_promise_with_resolver, (), {
    // clang-format off
    let resolve;
    let reject;

    const promise = new Promise((res, rej) => {
        resolve = res;
        reject = rej;
    });

    return Emval.toHandle({ promise, resolve, reject });
    // clang-format on
});

/**
 * Wrap a promise in a "thenable" that has a dispose function.
 */
EM_JS(emscripten::EM_VAL, make_disposable_thenable, (emscripten::EM_VAL promiseHandle), {
    // clang-format off
    const promise = Emval.toValue(promiseHandle);

    class DisposableThenable {
        constructor(promise) {
            this.promise = promise;
        }

        then(onFulfilled, onRejected) {
            return this.promise.then((value) => {
                return onFulfilled ? onFulfilled(value) : value;
            }, onRejected);
        }

        [Symbol.dispose]() {
            console.log("DisposableThenable - Disposing promise result");

            this.promise.then((value) => {
                if (value && typeof value.delete === 'function') {
                    value.delete();
                }
            });
        }
    }

    return Emval.toHandle(new DisposableThenable(promise));
    // clang-format on
});

/*
 * Wrap a promise so the value is automatically disposed after the first then()/await has run.
 * A second subscriber will get an already-disposed value.
 *
 * DOES NOT WORK with await as it is a custom thenable, not a real Promise.
 * The finally will actually run before the await subscriber because the await machinery means that
 * the subscriber is not actually a direct .then() on the promise, but rather a .then() on
 * an internal PromiseResolveThenableJob, so the onFulfilled is not actually the user's subscriber,
 * but rather an internal resolver. So the finally will be run before the user code.
 */
EM_JS(emscripten::EM_VAL, make_auto_disposing_thenable, (emscripten::EM_VAL promiseHandle), {
    // clang-format off
    const promise = Emval.toValue(promiseHandle);

    return Emval.toHandle({
        then(onFulfilled, onRejected) {
            return promise.then((value) => {
                try {
                    return onFulfilled ? onFulfilled(value) : value;
                } finally {
                    if (value && typeof value.delete === 'function') {
                        value.delete();
                    }
                }
            }, onRejected);
        }
    });
// clang-format on
});

/*
 * Idea from an LLM.
 * This does't touch the original promise but attaches a subscriber that disposes the value after a
 * tick of the event loop.
 *
 * This obviously is not workable because a subscriber could be added at any time as long as the
 * promise is kept around.
 */
EM_JS(void, dispose_promise_result_after_settlement, (emscripten::EM_VAL promiseHandle), {
    // clang-format off
    const promise = Emval.toValue(promiseHandle);

    promise.then((value) => {
        queueMicrotask(() => {
            if (value && typeof value.delete === 'function' && !value.isDeleted()) {
                console.log("Disposing promise result");

                value.delete();
            }
        });
    });
    // clang-format on
});

namespace bindings::utils {

// Transform a callback into something that returns a JS promise
template <typename CbArg, typename ReturnValType = emscripten::val, typename F> inline ReturnValType promisify(F&& f)
{
    using DecayedCbArg = std::decay_t<CbArg>;

    emscripten::val holder = emscripten::val::take_ownership(make_promise_with_resolver());
    emscripten::val resolve = holder["resolve"];

    // Function that ultimately gets passed into to CSP as the callback
    std::function<void(CbArg)> callback = [resolve](CbArg arg) {
        if constexpr (std::is_pointer_v<DecayedCbArg>) {
            // For pointers, use NonOwningVal to pass through without copying - same as the normal callback path.
            // Without this, an extra copy was being made.
            // This constructs the val with rvp::reference, bypassing GenericBindingType's default copy.
            // If there is no auto-disposal, this is not the right construct as we will need to be
            // able to dispose values types and ideally make disposing pointers a no-op.
            resolve(NonOwningVal(arg));
        } else {
            resolve(arg);
        }
    };

    f(callback);

    emscripten::val promise = holder["promise"];

    return ReturnValType(std::move(promise));
}

template <typename CbArg, typename ReturnValType = emscripten::val, typename F> inline ReturnValType promisify_disposable_thenable(F&& f)
{
    using DecayedCbArg = std::decay_t<CbArg>;

    // Primitives, strings and pointers do not need disposal. Support for container types (Array/List/Map/Optional) not added at this point.
    constexpr bool needsDisposal = !std::is_pointer_v<DecayedCbArg> && !std::is_arithmetic_v<DecayedCbArg> && !std::is_same_v<DecayedCbArg, csp::common::String>;

    emscripten::val holder = emscripten::val::take_ownership(make_promise_with_resolver());
    emscripten::val resolve = holder["resolve"];

    std::function<void(CbArg)> callback = [resolve](CbArg arg) {
        if constexpr (std::is_pointer_v<DecayedCbArg>) {
            resolve(NonOwningVal(arg));
        } else {
            resolve(arg);
        }
    };

    f(callback);

    emscripten::val promise = holder["promise"];
    if constexpr (needsDisposal) {
        promise = emscripten::val::take_ownership(make_disposable_thenable(promise.as_handle()));
    }

    return ReturnValType(std::move(promise));
}

template <typename CbArg, typename ReturnValType = emscripten::val, typename F> inline ReturnValType promisify_auto_disposing_thenable(F&& f)
{
    using DecayedCbArg = std::decay_t<CbArg>;

    // Primitives, strings and pointers do not need disposal. Support for container types (Array/List/Map/Optional) not added at this point.
    constexpr bool needsDisposal = !std::is_pointer_v<DecayedCbArg> && !std::is_arithmetic_v<DecayedCbArg> && !std::is_same_v<DecayedCbArg, csp::common::String>;

    emscripten::val holder = emscripten::val::take_ownership(make_promise_with_resolver());
    emscripten::val resolve = holder["resolve"];

    std::function<void(CbArg)> callback = [resolve](CbArg arg) {
        if constexpr (std::is_pointer_v<DecayedCbArg>) {
            resolve(NonOwningVal(arg));
        } else {
            resolve(arg);
        }
    };

    f(callback);

    emscripten::val promise = holder["promise"];
    if constexpr (needsDisposal) {
        promise = emscripten::val::take_ownership(make_auto_disposing_thenable(promise.as_handle()));
    }

    return ReturnValType(std::move(promise));
}

template <typename CbArg, typename ReturnValType = emscripten::val, typename F> inline ReturnValType promisify_disposal_microtask(F&& f)
{
    using DecayedCbArg = std::decay_t<CbArg>;

    // Primitives, strings and pointers do not need disposal. Support for container types (Array/List/Map/Optional) not added at this point.
    constexpr bool needsDisposal = !std::is_pointer_v<DecayedCbArg> && !std::is_arithmetic_v<DecayedCbArg> && !std::is_same_v<DecayedCbArg, csp::common::String>;

    emscripten::val holder = emscripten::val::take_ownership(make_promise_with_resolver());
    emscripten::val resolve = holder["resolve"];

    std::function<void(CbArg)> callback = [resolve](CbArg arg) {
        if constexpr (std::is_pointer_v<DecayedCbArg>) {
            resolve(NonOwningVal(arg));
        } else {
            resolve(arg);
        }
    };

    f(callback);

    emscripten::val promise = holder["promise"];
    if constexpr (needsDisposal) {
        dispose_promise_result_after_settlement(promise.as_handle());
    }

    return ReturnValType(std::move(promise));
}

// template <typename CbArg, typename ReturnValType = emscripten::val, typename F> inline ReturnValType promisify(F&& f)
// {
//     return promisify_disposable_thenable<CbArg, ReturnValType>(std::forward<F>(f));
// }

}
