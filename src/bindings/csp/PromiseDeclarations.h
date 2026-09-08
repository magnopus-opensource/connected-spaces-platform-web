#pragma once

#include "emscripten/val.h"

/*
 * Val types for Promises the bindings expose.
 * Typescript sigs are defined in PointerDeclarations.cpp
 */

EMSCRIPTEN_DECLARE_VAL_TYPE(PromiseOfBoolean);
EMSCRIPTEN_DECLARE_VAL_TYPE(PromiseOfSpaceEntityPointer);
