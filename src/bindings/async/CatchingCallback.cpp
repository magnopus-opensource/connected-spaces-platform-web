#include "CatchingCallback.h"

#include <emscripten.h>

EM_JS(emscripten::EM_VAL, catching_callback, (emscripten::EM_VAL cb_handle), {
    const cb = Emval.toValue(cb_handle);
    const catchingAdapter = function(... a)
    {
        try {
            cb(... a);
        } catch (e) {
            console.error('Unhandled exception in off-thread callback:', e);
        }
    };
    return Emval.toHandle(catchingAdapter);
});
