// Stub binding.

#include "SpaceEntity.h"

#include "CSP/Multiplayer/SpaceEntity.h"

#include "emscripten/bind.h"

EMSCRIPTEN_BINDINGS(CSPSpaceEntity)
{
    emscripten::class_<csp::multiplayer::SpaceEntity>("SpaceEntity");

    emscripten::register_type<SpaceEntityPointer>("SpaceEntity | null");
}
