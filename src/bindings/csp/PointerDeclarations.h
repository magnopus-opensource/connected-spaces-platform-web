#pragma once

#include "emscripten/val.h"

/*
 * Manifest Val types for pointers to CSP types exposed as nullable single-object handles.
 * Pairs with PointerDeclarations.cpp
 */

EMSCRIPTEN_DECLARE_VAL_TYPE(CancellationTokenPointer);
EMSCRIPTEN_DECLARE_VAL_TYPE(SpaceEntityPointer);
