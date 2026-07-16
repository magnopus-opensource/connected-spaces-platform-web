#pragma once

#include "emscripten/val.h"

namespace bindings::utils {

/*
 * Dispose a single bound handle.
 *
 * Input must be a bound ClassHandle; anything else (including arrays) is invalid.
 * An already-deleted handle is tolerated as a no-op.
 */
void DisposeElement(emscripten::val v) noexcept;

/*
 * Dispose every bound handle reachable in a JS array, including through nested arrays.
 * Input must be an array; non-array input is invalid. Empty arrays and arrays containing
 * no handles are a tolerated no-op — this is the path `using` routes through, so it
 * must work uniformly on whatever shape an array return happens to have.
 */
void DisposeArray(emscripten::val arr) noexcept;

/*
 * Dispose every bound handle held as a *value* in a JS Map. Keys are intentionally
 * left untouched: map keys are constrained to primitives (see Map.h IsValidMapKey) and
 * so own no C++ memory. Invalid if not a map. Should tolerate null input and empty values.
 * This is the path `using` routes through, so it must work uniformly on whatever shape
 * a map return happens to have.
 */
void DisposeMap(emscripten::val map) noexcept;

/*
 * Dispose an optional. Will do nothing if the value is undefined, else will perform regular
 * element disposal.
 * This is the path `using` routes through, so it must work uniformly on whatever shape
 * an optional return happens to have.
 */
void DisposeOptional(emscripten::val opt) noexcept;

}
