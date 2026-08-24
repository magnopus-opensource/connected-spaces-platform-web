#include "Promises.h"

#include "emscripten/em_js.h"
#include "emscripten/val.h"

/*
 * Create a JavaScript promise along with a callback used to resolve it with a value.
 *
 * The callback will perform an additional clone of the value before resolving the promise if
 * cloneArg is true.
 */
EM_JS(emscripten::EM_VAL, make_promise_with_cloning_callback, (bool cloneArg), {
    // clang-format off
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

    const callback = (arg) => {
        // Clone the argument if requested, otherwise just resolve with the original value.
        // We need to clone to keep the argument alive because we use this callback with
        // toNativeCallback, which will dispose the argument after the callback returns.
        if (cloneArg) {
            if (!arg) {
                // Do not try to clone null or undefined values. Note that this will catch all
                // "falsy" values such as 0 or empty string which we also do not clone anyway.
                // Null or undefined pointer or optional types, which request a clone, will arrive
                // here too.
                resolve(arg);
            } else if ((Array.isArray(arg) || arg instanceof Map)) {
                // Clone a container (array or Map) using the correct clone function.
                const clonedContainer = cloneContainer(arg);

                if (clonedContainer !== null) {
                    resolve(clonedContainer);
                } else {
                    // In theory this should never happen except in the case of a coding error.
                    reject('Error: could not clone container - unknown container type');
                }
            } else if (typeof arg !== 'object' || !('clone' in arg)) {
                // Do not clone other types that don't have a clone function.
                resolve(arg);
            } else {
                resolve(arg.clone());
            }
        } else {
            resolve(arg);
        }

        // arg will be disposed on exiting here if callback is used with toNativeCallback and not
        // cloned
    };

    return Emval.toHandle({ promise, callback });
    // clang-format on
});
