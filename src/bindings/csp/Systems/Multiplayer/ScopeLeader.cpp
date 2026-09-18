#include "../../../containers/String.h"

#include "CSP/Common/String.h"
#include "CSP/Systems/Multiplayer/ScopeLeader.h"
#include "CSP/Systems/WebService.h"

#include "emscripten/bind.h"

EMSCRIPTEN_BINDINGS(CSPScopeLeader)
{
    emscripten::class_<csp::systems::ScopeLeader>("ScopeLeader")
        .class_function(
            "create", +[]() { return csp::systems::ScopeLeader(); })
        .property("scopeId", &csp::systems::ScopeLeader::ScopeId)
        .property("scopeLeaderUserId", &csp::systems::ScopeLeader::ScopeLeaderUserId)
        .property("scopeClientId", &csp::systems::ScopeLeader::ScopeClientId)
        .property("electionInProgress", &csp::systems::ScopeLeader::ElectionInProgress);
}
