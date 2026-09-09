#include "../../../containers/String.h"

#include "CSP/Common/String.h"
#include "CSP/Multiplayer/Components/ScriptSpaceComponent.h"

#include "emscripten/bind.h"
#include <cstdint>

EMSCRIPTEN_BINDINGS(CSPScriptSpaceComponent)
{
    emscripten::enum_<csp::multiplayer::ScriptScope>("ScriptScope", emscripten::enum_value_type::number)
        .value("Local", csp::multiplayer::ScriptScope::Local)
        .value("Owner", csp::multiplayer::ScriptScope::Owner)
        .value("Num", csp::multiplayer::ScriptScope::Num);

    emscripten::enum_<csp::multiplayer::ScriptComponentPropertyKeys>("ScriptComponentPropertyKeys", emscripten::enum_value_type::number)
        .value("ScriptSource", csp::multiplayer::ScriptComponentPropertyKeys::ScriptSource)
        .value("OwnerId", csp::multiplayer::ScriptComponentPropertyKeys::OwnerId)
        .value("ScriptScope", csp::multiplayer::ScriptComponentPropertyKeys::ScriptScope)
        .value("Num", csp::multiplayer::ScriptComponentPropertyKeys::Num);

    emscripten::class_<csp::multiplayer::ScriptSpaceComponent, emscripten::base<csp::multiplayer::ComponentBase>>("ScriptSpaceComponent")
        .property("scriptSource", &csp::multiplayer::ScriptSpaceComponent::GetScriptSource, &csp::multiplayer::ScriptSpaceComponent::SetScriptSource)
        .property("ownerId", &csp::multiplayer::ScriptSpaceComponent::GetOwnerId, &csp::multiplayer::ScriptSpaceComponent::SetOwnerId)
        .property("scriptScope", &csp::multiplayer::ScriptSpaceComponent::GetScriptScope, &csp::multiplayer::ScriptSpaceComponent::SetScriptScope);
}
