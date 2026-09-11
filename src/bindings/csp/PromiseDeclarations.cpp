#include "PromiseDeclarations.h"

#include "emscripten/bind.h"

/*
 * Registrations for the promise types
 * Typescript sigs are defined in PromiseDeclarations.cpp
 */

EMSCRIPTEN_BINDINGS(CSPPromiseDeclarations)
{
    emscripten::register_type<PromiseOfBoolean>("Promise<boolean>");
    emscripten::register_type<PromiseOfSpaceEntityPointer>("Promise<SpaceEntity | null>");
    emscripten::register_type<PromiseOfErrorCode>("Promise<ErrorCode>");
    emscripten::register_type<PromiseOfNullResult>("Promise<NullResult>");
    emscripten::register_type<PromiseOfStringResult>("Promise<StringResult>");
    emscripten::register_type<PromiseOfMessageResult>("Promise<MessageResult>");
    emscripten::register_type<PromiseOfMessageCollectionResult>("Promise<MessageCollectionResult>");
    emscripten::register_type<PromiseOfConversationResult>("Promise<ConversationResult>");
    emscripten::register_type<PromiseOfNumberOfRepliesResult>("Promise<NumberOfRepliesResult>");
    emscripten::register_type<PromiseOfAnnotationResult>("Promise<AnnotationResult>");
    emscripten::register_type<PromiseOfAnnotationThumbnailCollectionResult>("Promise<AnnotationThumbnailCollectionResult>");
}
