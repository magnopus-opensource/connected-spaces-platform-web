#pragma once

#include "emscripten/val.h"

#include "CSP/Common/Interfaces/IRealtimeEngine.h"

#include "../async/Callbacks.h"

/*
 * Declarations for shared CSP type bindings.
 * Any type bindings for generic types without a single natural home in the CSP source tree can be
 * declared here.
 *
 * We also declare promise types here to avoid risk of redefinition and make them easily findable.
 */

EMSCRIPTEN_DECLARE_VAL_TYPE(PromiseOfBoolean);
EMSCRIPTEN_DECLARE_VAL_TYPE(PromiseOfSpaceEntityPointer);

DECLARE_CALLBACK(std::function<void()>, VoidCallback)
DECLARE_CALLBACK(csp::multiplayer::CallbackHandler, BooleanCallback)
