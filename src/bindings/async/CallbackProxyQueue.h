#pragma once
#include "emscripten/proxying.h"

namespace bindings::async {
// The same queue across all translation units, everywhere, for all time.
inline emscripten::ProxyingQueue& CallbackProxyQueue()
{
    static emscripten::ProxyingQueue queue;
    return queue;
}
}
