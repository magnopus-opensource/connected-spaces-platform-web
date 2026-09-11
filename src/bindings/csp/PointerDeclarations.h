#pragma once

#include "emscripten/val.h"

/*
 * Manifest Val types for pointers to CSP types exposed as nullable single-object handles.
 * Pairs with PointerDeclarations.cpp
 */

EMSCRIPTEN_DECLARE_VAL_TYPE(CancellationTokenPointer);
EMSCRIPTEN_DECLARE_VAL_TYPE(SpaceEntityPointer);
EMSCRIPTEN_DECLARE_VAL_TYPE(ComponentBasePointer);
EMSCRIPTEN_DECLARE_VAL_TYPE(MultiplayerConnectionPointer);
EMSCRIPTEN_DECLARE_VAL_TYPE(NetworkEventBusPointer);
EMSCRIPTEN_DECLARE_VAL_TYPE(OnlineRealtimeEnginePointer);
EMSCRIPTEN_DECLARE_VAL_TYPE(LogSystemPointer);

EMSCRIPTEN_DECLARE_VAL_TYPE(EntityScriptReference);
EMSCRIPTEN_DECLARE_VAL_TYPE(SystemsManagerReference);
