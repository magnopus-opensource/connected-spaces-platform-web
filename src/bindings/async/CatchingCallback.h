#pragma once

#include "emscripten/val.h"

/*
 * Calls a callable inside JS, inside a try/catch block
 * This is for exception handling, specifically cross-thread exceptions, which embind doesn't
 * support natively. Letting an exception unwind the stack would be catastrophic. Think about
 * this, if you're using proxy_sync, you are essentially calling the callback directly from the
 * browsers event-dispatch code. If we unwind the stack, it would end up crashing into _that_.
 *
 * All this does is catch any exceptions, and then calls console.error() with the information.
 * Off-thread callbacks never actually throw. An unfortunate asymmetry, but I think all we can do
 * unless we're willing to block.
 *
 * Extern C to ensure we use the same language linkage as the macro does. Thank heavens there was
 * a clear error about this. Arguably, we could have defined this in the same translation unit
 * as it was used ... but that would require making a Callbacks.cpp TU, and that file is almost
 * entirely templates. Maybe I'm just being lazy, ... sue me.
 *
 * So we can have a uniform story between on-thread and off-thread callbacks, we actually just
 * shunt everything through here, with a bool to decide if we'll catch the exception or let
 * it bubble out. (Pass true if off-thread, false if on-thread).
 */
extern "C" emscripten::EM_VAL catching_callback(emscripten::EM_VAL cb_handle, bool catchAndPrintExceptions);
