#include "../../async/Promises.h"
#include "../../containers/Array.h"
#include "../../containers/String.h"
#include "../../utils/JSDisposable.h"
#include "../CallbackDeclarations.h"
#include "../PromiseDeclarations.h"

#include "CSP/Common/Array.h"
#include "CSP/Common/NetworkEventData.h"
#include "CSP/Common/ReplicatedValue.h"
#include "CSP/Common/SharedEnums.h"
#include "CSP/Common/String.h"
#include "CSP/Multiplayer/NetworkEventBus.h"

#include "emscripten/bind.h"
#include "emscripten/val.h"
#include <cstdint>

EMSCRIPTEN_BINDINGS(CSPNetworkEventBus)
{
    emscripten::class_<csp::multiplayer::NetworkEventRegistration>("NetworkEventRegistration")
        .class_function(
            "create(eventReceiverId, eventName)",
            +[](const csp::common::String& eventReceiverId, const csp::common::String& eventName) {
                return csp::multiplayer::NetworkEventRegistration(eventReceiverId, eventName);
            })
        .property("eventReceiverId", &csp::multiplayer::NetworkEventRegistration::EventReceiverId)
        .property("eventName", &csp::multiplayer::NetworkEventRegistration::EventName)
        .function("equals(other)", &csp::multiplayer::NetworkEventRegistration::operator==);

    emscripten::class_<csp::multiplayer::NetworkEventBus>("NetworkEventBus")
        .function(
            "sendNetworkEvent(eventName, args)",
            +[](csp::multiplayer::NetworkEventBus& self, const csp::common::String& eventName, const csp::common::Array<csp::common::ReplicatedValue>& args) {
                return Promisify<PromiseOfErrorCode>([&](emscripten::val cb) { self.SendNetworkEvent(eventName, args, ToNativeCallback(cb.as<ErrorCodeCallback>())); });
            })
        .function(
            "sendNetworkEventToClient(eventName, args, targetClientId)",
            +[](csp::multiplayer::NetworkEventBus& self, const csp::common::String& eventName, const csp::common::Array<csp::common::ReplicatedValue>& args,
                 uint64_t targetClientId) {
                return Promisify<PromiseOfErrorCode>(
                    [&](emscripten::val cb) { self.SendNetworkEventToClient(eventName, args, targetClientId, ToNativeCallback(cb.as<ErrorCodeCallback>())); });
            })
        .function(
            "listenCustomNetworkEvent(eventReceiverId, eventName, callback)",
            +[](csp::multiplayer::NetworkEventBus& self, csp::common::String eventReceiverId, csp::common::String eventName, CustomNetworkEventCallback callback) {
                self.ListenCustomNetworkEvent(eventReceiverId, eventName, ToNativeCallback(callback));
            })
        .function(
            "listenAccessControlChangedEvent(eventReceiverId, callback)",
            +[](csp::multiplayer::NetworkEventBus& self, csp::common::String eventReceiverId, AccessControlChangedEventCallback callback) {
                self.ListenAccessControlChangedEvent(eventReceiverId, ToNativeCallback(callback));
            })
        .function(
            "listenAssetDetailBlobChangedEvent(eventReceiverId, callback)",
            +[](csp::multiplayer::NetworkEventBus& self, csp::common::String eventReceiverId, AssetDetailBlobChangedEventCallback callback) {
                self.ListenAssetDetailBlobChangedEvent(eventReceiverId, ToNativeCallback(callback));
            })
        .function(
            "listenAsyncCallCompletedEvent(eventReceiverId, operationName, callback)",
            +[](csp::multiplayer::NetworkEventBus& self, csp::common::String eventReceiverId, csp::common::String operationName, AsyncCallCompletedEventCallback callback) {
                self.ListenAsyncCallCompletedEvent(eventReceiverId, operationName, ToNativeCallback(callback));
            })
        .function(
            "listenConversationEvent(eventReceiverId, callback)",
            +[](csp::multiplayer::NetworkEventBus& self, csp::common::String eventReceiverId, ConversationEventCallback callback) {
                self.ListenConversationEvent(eventReceiverId, ToNativeCallback(callback));
            })
        .function(
            "listenSequenceChangedEvent(eventReceiverId, callback)",
            +[](csp::multiplayer::NetworkEventBus& self, csp::common::String eventReceiverId, SequenceChangedEventCallback callback) {
                self.ListenSequenceChangedEvent(eventReceiverId, ToNativeCallback(callback));
            })
        .function("stopListenCustomNetworkEvent(eventReceiverId, eventName)", &csp::multiplayer::NetworkEventBus::StopListenCustomNetworkEvent)
        .function("stopListenAccessControlChangedEvent(eventReceiverId)", &csp::multiplayer::NetworkEventBus::StopListenAccessControlChangedEvent)
        .function("stopListenAssetDetailBlobChangedEvent(eventReceiverId)", &csp::multiplayer::NetworkEventBus::StopListenAssetDetailBlobChangedEvent)
        .function("stopListenAsyncCallCompletedEvent(eventReceiverId, operationName)", &csp::multiplayer::NetworkEventBus::StopListenAsyncCallCompletedEvent)
        .function("stopListenConversationEvent(eventReceiverId)", &csp::multiplayer::NetworkEventBus::StopListenConversationEvent)
        .function("stopListenSequenceChangedEvent(eventReceiverId)", &csp::multiplayer::NetworkEventBus::StopListenSequenceChangedEvent)
        .function("stopListenAllNetworkEvents(eventReceiverId)", &csp::multiplayer::NetworkEventBus::StopListenAllNetworkEvents)
        .function(
            "allRegistrations",
            +[](const csp::multiplayer::NetworkEventBus& self) {
                return bindings::utils::JSDisposable<csp::common::Array<csp::multiplayer::NetworkEventRegistration>> { self.AllRegistrations() };
            })
        .function("startEventMessageListening", &csp::multiplayer::NetworkEventBus::StartEventMessageListening);
}
