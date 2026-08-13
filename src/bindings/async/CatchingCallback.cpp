#include "CatchingCallback.h"

#include <emscripten.h>

EM_JS(emscripten::EM_VAL, catching_callback, (emscripten::EM_VAL cb_handle, bool catching), {
    const cb = Emval.toValue(cb_handle);
    return Emval.toHandle(catching
          ? function(...a) { try { cb(...a); } catch (e) { console.error('Unhandled exception in off-thread callback:', e); } }
          : function(...a) { cb(...a); });
});
