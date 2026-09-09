#include "../../async/Promises.h"
#include "../../containers/String.h"
#include "../../utils/Handles.h"
#include "../CallbackDeclarations.h"
#include "../PointerDeclarations.h"
#include "../PromiseDeclarations.h"

#include "CSP/Common/SharedEnums.h"
#include "CSP/Common/String.h"
#include "CSP/Multiplayer/MultiPlayerConnection.h"
#include "CSP/Multiplayer/OnlineRealtimeEngine.h"

#include "emscripten/bind.h"
#include "emscripten/val.h"

EMSCRIPTEN_BINDINGS(CSPMultiplayerConnection)
{
    emscripten::enum_<csp::multiplayer::ConnectionState>("ConnectionState", emscripten::enum_value_type::number)
        .value("Connecting", csp::multiplayer::ConnectionState::Connecting)
        .value("Connected", csp::multiplayer::ConnectionState::Connected)
        .value("Disconnecting", csp::multiplayer::ConnectionState::Disconnecting)
        .value("Disconnected", csp::multiplayer::ConnectionState::Disconnected);

    emscripten::class_<csp::multiplayer::MultiplayerConnection>("MultiplayerConnection")
        .property("clientId", &csp::multiplayer::MultiplayerConnection::GetClientId)
        .property("connectionState", &csp::multiplayer::MultiplayerConnection::GetConnectionState)
        .property("allowSelfMessagingFlag", &csp::multiplayer::MultiplayerConnection::GetAllowSelfMessagingFlag)
        .function(
            "setDisconnectionCallback(callback)",
            +[](csp::multiplayer::MultiplayerConnection& self, ConnectionStatusCallback callback) { self.SetDisconnectionCallback(ToNativeCallback(callback)); })
        .function(
            "setConnectionCallback(callback)",
            +[](csp::multiplayer::MultiplayerConnection& self, ConnectionStatusCallback callback) { self.SetConnectionCallback(ToNativeCallback(callback)); })
        .function(
            "setNetworkInterruptionCallback(callback)",
            +[](csp::multiplayer::MultiplayerConnection& self, ConnectionStatusCallback callback) { self.SetNetworkInterruptionCallback(ToNativeCallback(callback)); })
        .function(
            "setAllowSelfMessagingFlag(allowSelfMessaging)",
            +[](csp::multiplayer::MultiplayerConnection& self, bool allowSelfMessaging) {
                return Promisify<PromiseOfErrorCode>([&](emscripten::val cb) { self.SetAllowSelfMessagingFlag(allowSelfMessaging, ToNativeCallback(cb.as<ErrorCodeCallback>())); });
            })
        .function(
            "getOnlineRealtimeEngine",
            +[](const csp::multiplayer::MultiplayerConnection& self) { return bindings::utils::NonOwningVal<OnlineRealtimeEnginePointer>(self.GetOnlineRealtimeEngine()); });
}
