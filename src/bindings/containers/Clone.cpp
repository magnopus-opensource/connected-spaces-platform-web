#include "Clone.h"
#include "../utils/Handles.h"
#include "emscripten/bind.h"
#include "emscripten/val.h"
#include <stdexcept>

/*
 * Utils for cloning containers of memory.
 *
 * `cloneArray`/`cloneMap` return new array/Map, cloning
 * each bound handle and copying primitives across as-is. Cloning is lenient: anything that
 * isn't a bound handle is passed through untouched, and nested arrays are handled uniformly.
 * Remember that a .clone() on a bound handle is not a copy of underlying memory,
 * it's just a reference count increase (new handle though).
 *
 * The recursive-walker shape here is structurally similar to Disposal.cpp. If we end
 * up needing to do this walk again, we should separate structure from action to avoid
 * duplication.
 */

namespace {

/*
 * Recursive walker. Clones any bound ClassHandle reachable through the array,
 * descends into nested arrays, silently skips everything else.
 */
emscripten::val CloneAll(emscripten::val v)
{
    if (v.isArray()) {
        emscripten::val out = emscripten::val::array();
        const unsigned length = v["length"].as<unsigned>();
        for (unsigned i = 0; i < length; ++i) {
            out.set(i, CloneAll(v[i]));
        }
        return out;
    }

    // Be lenient because we could be cloning arrays of JS objects, or mixed arrays.
    if (!bindings::utils::IsBoundHandle(v)) {
        return v;
    }

    return v.call<emscripten::val>("clone");
}

}

namespace bindings::utils {

emscripten::val CloneArray(emscripten::val arr)
{
    if (!arr.isArray()) {
        throw std::runtime_error("cloneArray was passed a non-array value");
    }
    return CloneAll(arr);
}

emscripten::val CloneMap(emscripten::val map)
{
    static const emscripten::val globalMap = emscripten::val::global("Map");
    if (!map.instanceof(globalMap)) {
        throw std::runtime_error("cloneMap was passed a non-Map value");
    }

    emscripten::val newMap = globalMap.new_();
    // Array.from(map.values()) yields the values in an array, so we can reuse the array cloning machinery we already have.
    emscripten::val entries = emscripten::val::global("Array").call<emscripten::val>("from", map);
    const unsigned len = entries["length"].as<unsigned>();
    for (unsigned i = 0; i < len; ++i) {
        emscripten::val keyValuePair = entries[i];
        newMap.call<void>("set", keyValuePair[0], CloneAll(keyValuePair[1]));
    }
    return newMap;
}

}

EMSCRIPTEN_BINDINGS(CSPContainerClone)
{
    emscripten::function("cloneArray", &bindings::utils::CloneArray);
    emscripten::function("cloneMap", &bindings::utils::CloneMap);
}
