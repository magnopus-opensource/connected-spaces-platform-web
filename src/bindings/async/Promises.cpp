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

    const promise = new Promise((res) => {
        resolve = res;
    });

    const callback = (arg) =>
    {
        if (cloneArg) {
            // Guard for values that we should not try to clone.
            // In particular pointer or optional types will request a clone but the value may be
            // null or undefined.
            if (!arg || typeof arg !== 'object' || !('clone' in arg)) {
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
