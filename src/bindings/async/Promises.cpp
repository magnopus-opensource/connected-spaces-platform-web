#include "Promises.h"

#include "emscripten/em_js.h"

// Declare the JavaScript symbols that the JS code in this file depends on.
// We need to do this to ensure the dependencies are included in the output JS file.
EM_JS_DEPS(make_promise_with_cloning_callback_deps, "$CspRequestError");

/*
 * Create a JavaScript promise along with a callback used to resolve it with a value.
 *
 * The callback will perform an additional clone of the value before resolving the promise if it is
 * of a non-null, non-primitive type with a clone function.
 * We also clone pointer types, for consistency with value types and non-awaitable callback
 * arguments.
 */
EM_JS(emscripten::EM_VAL, make_promise_with_cloning_callback, (emscripten::EM_VAL progressCallbackHandle), {
    // clang-format off

    const progressCallback = Emval.toValue(progressCallbackHandle);

    let resolve;
    let reject;

    const promise = new Promise((res, rej) => {
        resolve = res;
        reject = rej;
    });

    // Clone containers (arrays and maps) using the dedicated clone functions for each.
    // Also attach a dispose function to the cloned containers, mirroring how JSDisposable is used
    // used for the standard returns for containers from C++ functions. This allows container
    // arguments to be used with "using" in JavaScript in the same way as other bound types.
    const cloneContainer = (arg) => {
        if (Array.isArray(arg)) {
            const cloned = Module['cloneArray'](arg);

            Object.defineProperty(cloned, Symbol.dispose, {
                value : Module['disposeArray'].bind(undefined, cloned),
                enumerable : false,
            });

            return cloned;
        }

        if (arg instanceof Map) {
            const cloned = Module['cloneMap'](arg);

            Object.defineProperty(cloned, Symbol.dispose, {
                value : Module['disposeMap'].bind(undefined, cloned),
                enumerable : false,
            });

            return cloned;
        }

        return null;
    };

    const cloneArg = (arg) => {
        if (!arg) {
            // Do not try to clone null or undefined values. Note that this will catch all
            // "falsy" values such as 0 or empty string which we also do not clone anyway.
            // Null or undefined pointer/optional types, which request a clone, will arrive here too.
            return arg;
        } else if ((Array.isArray(arg) || arg instanceof Map)) {
            // Clone a container (array or Map) using the correct clone function.
            const clonedContainer = cloneContainer(arg);

            if (clonedContainer !== null) {
                return clonedContainer;
            } else {
                // In theory this should never happen except in the case of a coding error.
                reject('Error: could not clone container - unknown container type');
            }
        } else if (typeof arg !== 'object' || !('clone' in arg)) {
            // Do not clone other types that don't have a clone function. This will catch primitive
            // types (number, boolean, string)
            return arg;
        } else {
            return arg.clone();
        }
    };

    const isResultType = (arg) => {
        return arg && arg instanceof Module['ResultBase'];
    };

    const callback = (arg) => {
        // Clone the argument to keep it alive because we use this callback with toNativeCallback,
        // which will dispose the argument after the callback returns.

        if (isResultType(arg)) {
            if (arg.resultCode === Module['EResultCode'].Success) {
                resolve(cloneArg(arg));
            } else if (arg.resultCode === Module['EResultCode'].Failed) {
                reject(new CspRequestError('CSP request failed', arg.resultCode, arg.httpResultCode, arg.responseBody, arg.failureReason));
            } else if (arg.resultCode === Module['EResultCode'].InProgress) {
                progressCallback?.(arg.requestProgress, arg.responseProgress);
            } else if (arg.resultCode === Module['EResultCode'].Init) {
                // Do nothing - request has just been initialised
            } else {
                resolve(cloneArg(arg));
            }
        }
        else {
            resolve(cloneArg(arg));
        }

        // arg will be disposed on exiting here if callback is used with toNativeCallback and not
        // cloned
    };

    return Emval.toHandle({ promise, callback });
    // clang-format on
});
