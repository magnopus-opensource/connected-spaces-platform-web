#include "emscripten/bind.h"
#include "emscripten/val.h"
#include "Handles.h"
#include <stdexcept>

/*
 * Utils for memory management, most necessary for containers.
 *
 * Arrays returned from CSP get a [Symbol.dispose] attached at the binding boundary
 * in the CSPArrayJSDisposable<T> WireType adapter (return path), routing through
 * `disposeArray` below. The intended user-facing pattern is `using arr = ...`
 * on anything CSP returns — including basic-type arrays, where dispose is just
 * a no-op walk.
 *
 * array disposal is intentionally lenient: it disposes any bound handle it finds
 * and silently ignores anything else (numbers, plain objects, nulls). It recurses
 * through nested arrays, so an array of arrays of handles cleans up correctly.
 *
 * `disposeElement` is the single-object counterpart. You can use this if you
 * like as a user, but it's mostly here so it can be called by array disposal.
 */

namespace {

/*
 * Dispose a single bound handle.
 *
 * Input must be a bound ClassHandle; anything else (including arrays) throws.
 * An already-deleted handle is tolerated as a no-op.
 */
void DisposeElement(emscripten::val v) {
    if (!bindings::utils::IsBoundHandle(v)) {
        throw std::runtime_error("disposeElement was passed a value that is not a bound handle");
    }

    if (v.call<bool>("isDeleted")) {
        return;
    }

    v.call<void>("delete");
}

/*
 * Recursive walker. Disposes any bound ClassHandle reachable through the array,
 * descends into nested arrays, silently skips everything else.
 */
void DisposeAll(emscripten::val v) {
    if (v.isArray()) {
        const unsigned length = v["length"].as<unsigned>();
        for (unsigned i = 0; i < length; ++i) {
            DisposeAll(v[i]);
        }
        return;
    }

    // Be lenient because we could be disposing arrays of JS objects, or mixed arrays.
    if (!bindings::utils::IsBoundHandle(v)) {
        return;
    }

    DisposeElement(v);
}

/*
 * Dispose every bound handle reachable in a JS array, including through nested arrays.
 * Input must be an array; non-array input throws. Empty arrays and arrays containing
 * no handles are a tolerated no-op — this is the path `using` routes through, so it
 * must work uniformly on whatever shape an array return happens to have.
 */
void DisposeArray(emscripten::val arr) {
    if (!arr.isArray()) {
        throw std::runtime_error("disposeArray was passed a non-array value");
    }
    DisposeAll(arr);
}

}

EMSCRIPTEN_BINDINGS(CSPMemory) {
    emscripten::function("disposeElement", &DisposeElement);
    emscripten::function("disposeArray", &DisposeArray);
}
