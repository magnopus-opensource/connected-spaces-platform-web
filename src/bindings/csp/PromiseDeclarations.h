#pragma once

#include "emscripten/val.h"

/*
 * Val types for Promises the bindings expose.
 * Typescript sigs are defined in PromiseDeclarations.cpp
 */

EMSCRIPTEN_DECLARE_VAL_TYPE(PromiseOfBoolean);
EMSCRIPTEN_DECLARE_VAL_TYPE(PromiseOfSpaceEntityPointer);
EMSCRIPTEN_DECLARE_VAL_TYPE(PromiseOfErrorCode);
EMSCRIPTEN_DECLARE_VAL_TYPE(PromiseOfNullResult);
EMSCRIPTEN_DECLARE_VAL_TYPE(PromiseOfStringResult);
EMSCRIPTEN_DECLARE_VAL_TYPE(PromiseOfMessageResult);
EMSCRIPTEN_DECLARE_VAL_TYPE(PromiseOfMessageCollectionResult);
EMSCRIPTEN_DECLARE_VAL_TYPE(PromiseOfConversationResult);
EMSCRIPTEN_DECLARE_VAL_TYPE(PromiseOfNumberOfRepliesResult);
EMSCRIPTEN_DECLARE_VAL_TYPE(PromiseOfAnnotationResult);
EMSCRIPTEN_DECLARE_VAL_TYPE(PromiseOfAnnotationThumbnailCollectionResult);
