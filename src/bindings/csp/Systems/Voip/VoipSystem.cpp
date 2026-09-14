#include "CSP/Systems/Voip/VoipSystem.h"

#include "emscripten/bind.h"

namespace emscripten::internal {
template <> void raw_destructor<csp::systems::VoipSystem>(csp::systems::VoipSystem*) { }
}

EMSCRIPTEN_BINDINGS(CSPVoipSystem)
{
    emscripten::class_<csp::systems::VoipSystem>("VoipSystem")
        .property("isLocalUserMuted", &csp::systems::VoipSystem::IsLocalUserMuted)
        .function("muteLocalUser(isMuted)", &csp::systems::VoipSystem::MuteLocalUser);
}
