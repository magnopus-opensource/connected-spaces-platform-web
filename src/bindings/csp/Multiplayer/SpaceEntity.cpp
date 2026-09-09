// Stub binding.

#include "CSP/Multiplayer/SpaceEntity.h"

#include "emscripten/bind.h"

EMSCRIPTEN_BINDINGS(CSPSpaceEntity) { emscripten::class_<csp::multiplayer::SpaceEntity>("SpaceEntity"); }
