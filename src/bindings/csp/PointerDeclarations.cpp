#include "PointerDeclarations.h"

#include "emscripten/bind.h"

/*
 * Registrations for the pointer types declared in PointerDeclarations.h.
 * Each registration is what gives the pointer its TypeScript signature in the generated .d.ts.
 */

EMSCRIPTEN_BINDINGS(CSPPointerDeclarations)
{
    emscripten::register_type<CancellationTokenPointer>("CancellationToken | null");
    emscripten::register_type<SpaceEntityPointer>("SpaceEntity | null");
}
