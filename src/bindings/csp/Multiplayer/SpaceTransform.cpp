// Stub binding.

#include "CSP/Multiplayer/SpaceTransform.h"

#include "emscripten/bind.h"

EMSCRIPTEN_BINDINGS(CSPSpaceTransform)
{
    emscripten::value_object<csp::multiplayer::SpaceTransform>("SpaceTransform")
        .field("position", &csp::multiplayer::SpaceTransform::Position)
        .field("rotation", &csp::multiplayer::SpaceTransform::Rotation)
        .field("scale", &csp::multiplayer::SpaceTransform::Scale);
}
