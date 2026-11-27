#include "../../../containers/String.h"
#include "../../CallbackDeclarations.h"

#include "CSP/Common/Interfaces/IJSScriptRunner.h"
#include "CSP/Common/String.h"
#include "CSP/Systems/Script/ScriptSystem.h"

#include "emscripten/bind.h"
#include <cstdint>

namespace emscripten::internal {
template <> void raw_destructor<csp::systems::ScriptSystem>(csp::systems::ScriptSystem*) { }
}

EMSCRIPTEN_BINDINGS(CSPScriptSystem)
{
    emscripten::class_<csp::systems::ScriptErrorData>("ScriptErrorData")
        .class_function(
            "create", +[]() { return csp::systems::ScriptErrorData(); })
        .property("contextId", &csp::systems::ScriptErrorData::ContextId)
        .property("scriptName", &csp::systems::ScriptErrorData::ScriptName)
        .property("errorMessage", &csp::systems::ScriptErrorData::ErrorMessage);

    emscripten::class_<csp::systems::ScriptSystem, emscripten::base<csp::common::IJSScriptRunner>>("ScriptSystem")
        .function("initialise", &csp::systems::ScriptSystem::Initialise)
        .function("shutdown", &csp::systems::ScriptSystem::Shutdown)
        .function("runScriptFile(contextId, scriptFilePath)", &csp::systems::ScriptSystem::RunScriptFile)
        .function(
            "setScriptErrorCallback(callback)",
            +[](csp::systems::ScriptSystem& self, ScriptErrorCallback callback) { self.SetScriptErrorCallback(ToNativeCallback(callback)); });
}
