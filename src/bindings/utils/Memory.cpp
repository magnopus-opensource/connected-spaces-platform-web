#include "Memory.h"
#include "Handles.h"
#include "emscripten/bind.h"
#include "emscripten/emscripten.h"
#include "emscripten/val.h"
#include <cassert>
#include <string>

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
 * In the case where the .delete() would throw, we instead log to the warnings stream.
 */

namespace {

/*
 * Recursive walker. Disposes any bound ClassHandle reachable through the array,
 * descends into nested arrays, silently skips everything else.
 */
void DisposeAllNoThrow(emscripten::val v) noexcept
{
    if (v.isArray()) {
        const unsigned length = v["length"].as<unsigned>();
        for (unsigned i = 0; i < length; ++i) {
            DisposeAllNoThrow(v[i]);
        }
        return;
    }

    // Be lenient because we could be disposing arrays of JS objects, or mixed arrays.
    if (!bindings::utils::IsBoundHandle(v)) {
        return;
    }

    bindings::utils::DisposeElementNoThrow(v);
}

void DisposeAll(emscripten::val v)
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

/*
 * Catch the JS deletion
 * We do this because we're using RAII mechanisms in the bindings, which means we have
 * "No throwing in destructors" as a rule, but we still want clients to know
 * that they're double deleting (even though this is always fine, it's just a no-op).
 *
 * We invoke delete via the prototype rather than the object, because ForbidOwningMemoryBehaviours
 * (see Handles.h) replaces `delete` with `undefined` as an own property on non-owning handles.
 * Going through the prototype bypasses this, allowing us to present a non-owning handle that can't
 * delete whilst still allowing the RAII system to delete internally. This catching_delete is
 * exclusively used in the RAII path.
 *
 * Technically a user could also go through the prototype, but they'd really have to be trying to break things.
 */
EM_JS(char*, catching_delete, (emscripten::EM_VAL handleId), {
    const obj = Emval.toValue(handleId);
    try {
        Object.getPrototypeOf(obj).delete.call(obj);
        return stringToNewUTF8("");
    } catch (e) {
        return stringToNewUTF8(e.message);
    }
});

namespace bindings::utils {

void DisposeElementNoThrow(emscripten::val v) noexcept
{
    assert(bindings::utils::IsBoundHandle(v) && "disposeElement was passed a value that is not a bound handle");

    /* Because we can't throw an exception here like a regular .delete() on a handle would, at least log it */
    std::string errorStr = std::string(catching_delete(v.as_handle()));
    if (!errorStr.empty()) {
        emscripten::val::global("console").call<void>("warn", std::string { errorStr });
    }
}

void DisposeElement(emscripten::val v)
{
    if (!bindings::utils::IsBoundHandle(v)) {
        throw std::runtime_error("disposeElement was passed a non-boundhandle value");
    }

    v.call<void>("delete");
}

void DisposeArrayNoThrow(emscripten::val arr) noexcept
{
    assert(arr.isArray() && "disposeArray was passed a non-array value");
    DisposeAllNoThrow(arr);
}

void DisposeArray(emscripten::val arr)
{
    if (!arr.isArray()) {
        throw std::runtime_error("disposeArray was passed a non-array value");
    }
    DisposeAll(arr);
}

void DisposeMapNoThrow(emscripten::val map) noexcept
{
    static const emscripten::val globalMap = emscripten::val::global("Map");
    assert(map.instanceof(globalMap) && "disposeMap was passed a non-Map value");
    if (!map.instanceof(globalMap)) {
        return;
    }
    // Array.from(map.values()) yields the values in an array, so we can reuse the array disposal machinery we already have.
    DisposeAllNoThrow(emscripten::val::global("Array").call<emscripten::val>("from", map.call<emscripten::val>("values")));
}

void DisposeMap(emscripten::val map)
{
    static const emscripten::val globalMap = emscripten::val::global("Map");
    if (!map.instanceof(globalMap)) {
        throw std::runtime_error("disposeMap was passed a non-Map value");
    }
    if (!map.instanceof(globalMap)) {
        return;
    }
    // Array.from(map.values()) yields the values in an array, so we can reuse the array disposal machinery we already have.
    DisposeAll(emscripten::val::global("Array").call<emscripten::val>("from", map.call<emscripten::val>("values")));
}

/* This is a purely internal method as in JS land we simply rely on element disposal, as undefined objects can't be disposed of anyhow */
void DisposeOptionalNoThrow(emscripten::val opt) noexcept
{
    if (opt.isUndefined()) {
        // Undefined is a valid state, just bail.
        return;
    }

    DisposeAllNoThrow(opt);
}

}

EMSCRIPTEN_BINDINGS(CSPMemory)
{
    emscripten::function("disposeElement", &bindings::utils::DisposeElement);
    emscripten::function("disposeArray", &bindings::utils::DisposeArray);
    emscripten::function("disposeMap", &bindings::utils::DisposeMap);
}
