#include "../../../containers/String.h"

#include "CSP/Common/String.h"
#include "CSP/Multiplayer/Components/ScriptSpaceComponent.h"
#include "CSP/Multiplayer/Script/EntityScript.h"

#include "emscripten/bind.h"
#include <cstdint>

EMSCRIPTEN_BINDINGS(CSPEntityScript)
{
    emscripten::class_<csp::multiplayer::EntityScript>("EntityScript")
        .property("ownerId", &csp::multiplayer::EntityScript::GetOwnerId, &csp::multiplayer::EntityScript::SetOwnerId)
        .function("getScriptSource", &csp::multiplayer::EntityScript::GetScriptSource)
        .function("setScriptSource(scriptSource)", &csp::multiplayer::EntityScript::SetScriptSource)
        .function("invoke", &csp::multiplayer::EntityScript::Invoke)
        .function("runScript(scriptSource)", &csp::multiplayer::EntityScript::RunScript)
        .function("hasError", &csp::multiplayer::EntityScript::HasError)
        .function("hasEntityScriptComponent", &csp::multiplayer::EntityScript::HasEntityScriptComponent)
        .function("getErrorText", &csp::multiplayer::EntityScript::GetErrorText)
        .function("setScriptSpaceComponent(entityScriptComponent)", &csp::multiplayer::EntityScript::SetScriptSpaceComponent, emscripten::allow_raw_pointers())
        .function("onPropertyChanged(componentId, propertyKey)", &csp::multiplayer::EntityScript::OnPropertyChanged)
        .function(
            "postMessageToScript(message, messageParamsJson)",
            +[](csp::multiplayer::EntityScript& self, const csp::common::String& message, const csp::common::String& messageParamsJson) {
                self.PostMessageToScript(message, messageParamsJson);
            })
        .function("onSourceChanged(scriptSource)", &csp::multiplayer::EntityScript::OnSourceChanged)
        .function("registerSourceAsModule", &csp::multiplayer::EntityScript::RegisterSourceAsModule)
        .function("shutdown", &csp::multiplayer::EntityScript::Shutdown);
}
