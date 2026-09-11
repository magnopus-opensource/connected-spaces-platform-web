#include "../../containers/List.h"
#include "../../containers/String.h"
#include "../CallbackDeclarations.h"

#include "CSP/Common/Interfaces/IJSScriptRunner.h"
#include "CSP/Common/Interfaces/IRealtimeEngine.h"
#include "CSP/Common/List.h"
#include "CSP/Common/String.h"
#include "CSP/Common/Systems/Log/LogSystem.h"
#include "CSP/Multiplayer/MultiPlayerConnection.h"
#include "CSP/Multiplayer/NetworkEventBus.h"
#include "CSP/Multiplayer/OnlineRealtimeEngine.h"
#include "CSP/Multiplayer/SpaceEntity.h"

#include "emscripten/bind.h"
#include <cstdint>

EMSCRIPTEN_BINDINGS(CSPOnlineRealtimeEngine)
{
    emscripten::class_<csp::multiplayer::OnlineRealtimeEngine, emscripten::base<csp::common::IRealtimeEngine>>("OnlineRealtimeEngine")
        .class_function(
            "create(multiplayerConnection, logSystem, networkEventBus, remoteScriptRunner)",
            +[](csp::multiplayer::MultiplayerConnection& multiplayerConnection, csp::common::LogSystem& logSystem, csp::multiplayer::NetworkEventBus& networkEventBus,
                 csp::common::IJSScriptRunner& remoteScriptRunner) {
                return new csp::multiplayer::OnlineRealtimeEngine(multiplayerConnection, logSystem, networkEventBus, remoteScriptRunner);
            },
            emscripten::allow_raw_pointers())
        .class_function(
            "createWithJsonSchemas(multiplayerConnection, logSystem, networkEventBus, remoteScriptRunner, jsonSchemas)",
            +[](csp::multiplayer::MultiplayerConnection& multiplayerConnection, csp::common::LogSystem& logSystem, csp::multiplayer::NetworkEventBus& networkEventBus,
                 csp::common::IJSScriptRunner& remoteScriptRunner, const csp::common::List<csp::common::String>& jsonSchemas) {
                return new csp::multiplayer::OnlineRealtimeEngine(multiplayerConnection, logSystem, networkEventBus, remoteScriptRunner, jsonSchemas);
            },
            emscripten::allow_raw_pointers())
        .property("isLeaderElectionEnabled", &csp::multiplayer::OnlineRealtimeEngine::IsLeaderElectionEnabled)
        .property("leaderId", &csp::multiplayer::OnlineRealtimeEngine::GetLeaderId)
        .property("entityPatchRateLimitEnabled", &csp::multiplayer::OnlineRealtimeEngine::GetEntityPatchRateLimitEnabled,
            &csp::multiplayer::OnlineRealtimeEngine::SetEntityPatchRateLimitEnabled)
        .function("queueEntityUpdate(entity)", &csp::multiplayer::OnlineRealtimeEngine::QueueEntityUpdate, emscripten::allow_raw_pointers())
        .function("processPendingEntityOperations", &csp::multiplayer::OnlineRealtimeEngine::ProcessPendingEntityOperations)
        .function("claimScriptOwnership(entity)", &csp::multiplayer::OnlineRealtimeEngine::ClaimScriptOwnership, emscripten::allow_raw_pointers())
        .function(
            "setRemoteEntityCreatedCallback(callback)",
            +[](csp::multiplayer::OnlineRealtimeEngine& self, EntityCreatedCallback callback) { self.SetRemoteEntityCreatedCallback(ToNativeCallback(callback)); })
        .function(
            "setScriptLeaderReadyCallback(callback)",
            +[](csp::multiplayer::OnlineRealtimeEngine& self, BooleanCallback callback) { self.SetScriptLeaderReadyCallback(ToNativeCallback(callback)); })
        .function(
            "setOnElectedScopeLeaderCallback(callback)",
            +[](csp::multiplayer::OnlineRealtimeEngine& self, ScopeLeaderCallback callback) { self.SetOnElectedScopeLeaderCallback(ToNativeCallback(callback)); })
        .function(
            "setOnVacatedAsScopeLeaderCallback(callback)",
            +[](csp::multiplayer::OnlineRealtimeEngine& self, ScopeLeaderCallback callback) { self.SetOnVacatedAsScopeLeaderCallback(ToNativeCallback(callback)); });
}
