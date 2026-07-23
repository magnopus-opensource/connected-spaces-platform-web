#pragma once

#include "emscripten/val.h"

namespace bindings::utils {

/*
 * Clone a JS array. Returns a new array in which every bound handle reachable through the array,
 * including through nested arrays is cloned, non-handles are just carried across via a copy as
 * you'd expect. Invalid if argument is not an array.
 */
emscripten::val CloneArray(emscripten::val arr);

/*
 * Clone a JS Map. Returns a new Map with the same keys. Keys are intentionally
 * left untouched: map keys are constrained to primitives (see Map.h IsValidMapKey).
 * Each value is cloned. Invalid if argument is not a map.
 */
emscripten::val CloneMap(emscripten::val map);

}
