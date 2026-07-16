#include "Memory.h"
#include "Handles.h"
#include "emscripten/bind.h"
#include "emscripten/val.h"
#include <cassert>

/*
 * Utils for memory management, most necessary for containers.
 *
 * Arrays returned from CSP get a [Symbol.dispose] attached at the binding boundary
 * in the JSDisposable<csp::common::Array<T>> WireType adapter (return path), routing through
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
 *
 * `disposeMap` is the JS Map counterpart of `disposeArray`. It disposes the Map's
 * values only; keys are left alone because map keys are constrained to primitives
 * (see Map.h IsValidMapKey) and so own no C++ memory.
 *
 * These are disposal functions that are sometimes called in destructors, so must not throw.
 */

namespace {

/*
 * Recursive walker. Disposes any bound ClassHandle reachable through the array,
 * descends into nested arrays, silently skips everything else.
 */
void DisposeAll(emscripten::val v) noexcept
{
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

    bindings::utils::DisposeElement(v);
}

}

namespace bindings::utils {

void DisposeElement(emscripten::val v) noexcept
{
    assert(bindings::utils::IsBoundHandle(v) && "disposeElement was passed a value that is not a bound handle");
    if (!bindings::utils::IsBoundHandle(v)) { //CONSIDERATION FOR CODE REVIEW: Keep the returns for safety, or remove for clarity?
        return;
    }

    if (v.call<bool>("isDeleted")) {
        return;
    }

    v.call<void>("delete");
}

void DisposeArray(emscripten::val arr) noexcept
{
    assert(arr.isArray() && "disposeArray was passed a non-array value");
    DisposeAll(arr);
}

void DisposeMap(emscripten::val map) noexcept
{
    static const emscripten::val globalMap = emscripten::val::global("Map");
    assert(map.instanceof(globalMap) && "disposeMap was passed a non-Map value");
    if (!map.instanceof(globalMap)) {
        return;
    }
    // Array.from(map.values()) yields the values in an array, so we can reuse the array disposal machinery we already have.
    DisposeAll(emscripten::val::global("Array").call<emscripten::val>("from", map.call<emscripten::val>("values")));
}

/* This is a purely internal method as in JS land we simply rely on element disposal, as undefined objects cant be disposed of anyhow */
void DisposeOptional(emscripten::val opt) noexcept
{
    if (opt.isUndefined()) {
        // Undefined is a valid state, just bail.
        return;
    }

    DisposeAll(opt);
}

}

EMSCRIPTEN_BINDINGS(CSPMemory)
{
    emscripten::function("disposeElement", &bindings::utils::DisposeElement);
    emscripten::function("disposeArray", &bindings::utils::DisposeArray);
    emscripten::function("disposeMap", &bindings::utils::DisposeMap);
}
