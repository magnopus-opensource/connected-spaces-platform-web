#include "PromiseDeclarations.h"

#include "emscripten/bind.h"

/*
 * Registrations for the promise types
 * Typescript sigs are defined in PointerDeclarations.cpp
 */

EMSCRIPTEN_BINDINGS(CSPPromiseDeclarations)
{
    emscripten::register_type<PromiseOfBoolean>("Promise<boolean>");
    emscripten::register_type<PromiseOfSpaceEntityPointer>("Promise<SpaceEntity | null>");
}
