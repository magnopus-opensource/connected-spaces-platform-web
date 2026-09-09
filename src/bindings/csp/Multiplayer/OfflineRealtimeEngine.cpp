#include "../../containers/List.h"
#include "../../containers/String.h"

#include "CSP/Common/Interfaces/IJSScriptRunner.h"
#include "CSP/Common/Interfaces/IRealtimeEngine.h"
#include "CSP/Common/List.h"
#include "CSP/Common/String.h"
#include "CSP/Common/Systems/Log/LogSystem.h"
#include "CSP/Multiplayer/CSPSceneDescription.h"
#include "CSP/Multiplayer/OfflineRealtimeEngine.h"

#include "emscripten/bind.h"
#include <cstdint>

EMSCRIPTEN_BINDINGS(CSPOfflineRealtimeEngine)
{
    emscripten::class_<csp::multiplayer::OfflineRealtimeEngine, emscripten::base<csp::common::IRealtimeEngine>>("OfflineRealtimeEngine")
        .class_function(
            "create(logSystem, remoteScriptRunner)",
            +[](csp::common::LogSystem& logSystem, csp::common::IJSScriptRunner& remoteScriptRunner) {
                return new csp::multiplayer::OfflineRealtimeEngine(logSystem, remoteScriptRunner);
            },
            emscripten::allow_raw_pointers())
        .class_function(
            "createWithJsonSchemas(logSystem, remoteScriptRunner, jsonSchemas)",
            +[](csp::common::LogSystem& logSystem, csp::common::IJSScriptRunner& remoteScriptRunner, const csp::common::List<csp::common::String>& jsonSchemas) {
                return new csp::multiplayer::OfflineRealtimeEngine(logSystem, remoteScriptRunner, jsonSchemas);
            },
            emscripten::allow_raw_pointers())
        .class_function(
            "createFromSceneDescription(sceneDescription, logSystem, remoteScriptRunner)",
            +[](const csp::multiplayer::CSPSceneDescription& sceneDescription, csp::common::LogSystem& logSystem, csp::common::IJSScriptRunner& remoteScriptRunner) {
                return new csp::multiplayer::OfflineRealtimeEngine(sceneDescription, logSystem, remoteScriptRunner);
            },
            emscripten::allow_raw_pointers())
        .class_function(
            "createFromSceneDescriptionWithJsonSchemas(sceneDescription, logSystem, remoteScriptRunner, jsonSchemas)",
            +[](const csp::multiplayer::CSPSceneDescription& sceneDescription, csp::common::LogSystem& logSystem, csp::common::IJSScriptRunner& remoteScriptRunner,
                 const csp::common::List<csp::common::String>& jsonSchemas) {
                return new csp::multiplayer::OfflineRealtimeEngine(sceneDescription, logSystem, remoteScriptRunner, jsonSchemas);
            },
            emscripten::allow_raw_pointers())
        .class_function("localClientId", +[]() { return csp::multiplayer::OfflineRealtimeEngine::LocalClientId(); });
}
