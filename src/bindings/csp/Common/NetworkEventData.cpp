#include "../../containers/Array.h"
#include "../../containers/Map.h"
#include "../../containers/String.h"
#include "../../utils/JSDisposable.h"

#include "CSP/Common/Array.h"
#include "CSP/Common/NetworkEventData.h"
#include "CSP/Common/ReplicatedValue.h"
#include "CSP/Common/String.h"

#include "emscripten/bind.h"

EMSCRIPTEN_BINDINGS(CSPNetworkEventData)
{
    emscripten::enum_<csp::common::EAssetChangeType>("EAssetChangeType", emscripten::enum_value_type::number)
        .value("Created", csp::common::EAssetChangeType::Created)
        .value("Updated", csp::common::EAssetChangeType::Updated)
        .value("MusubiFailed", csp::common::EAssetChangeType::MusubiFailed)
        .value("Deleted", csp::common::EAssetChangeType::Deleted)
        .value("Invalid", csp::common::EAssetChangeType::Invalid)
        .value("Num", csp::common::EAssetChangeType::Num);

    emscripten::enum_<csp::common::EPermissionChangeType>("EPermissionChangeType", emscripten::enum_value_type::number)
        .value("Created", csp::common::EPermissionChangeType::Created)
        .value("Updated", csp::common::EPermissionChangeType::Updated)
        .value("Removed", csp::common::EPermissionChangeType::Removed)
        .value("Invalid", csp::common::EPermissionChangeType::Invalid);

    emscripten::enum_<csp::common::ESequenceUpdateType>("ESequenceUpdateType", emscripten::enum_value_type::number)
        .value("Create", csp::common::ESequenceUpdateType::Create)
        .value("Update", csp::common::ESequenceUpdateType::Update)
        .value("Delete", csp::common::ESequenceUpdateType::Delete)
        .value("Invalid", csp::common::ESequenceUpdateType::Invalid);

    emscripten::enum_<csp::common::ESequenceType>("ESequenceType", emscripten::enum_value_type::number)
        .value("Default", csp::common::ESequenceType::Default)
        .value("Hotspot", csp::common::ESequenceType::Hotspot);

    emscripten::register_type<csp::common::Array<csp::common::ReplicatedValue>>("ReplicatedValue[]");
    emscripten::register_type<bindings::utils::JSDisposable<csp::common::Array<csp::common::ReplicatedValue>>>("(ReplicatedValue[] & Disposable)");

    emscripten::register_type<csp::common::Array<csp::systems::SpaceUserRole>>("SpaceUserRole[]");

    emscripten::class_<csp::common::NetworkEventData>("NetworkEventData")
        .class_function(
            "create", +[]() { return csp::common::NetworkEventData(); })
        .property("eventName", &csp::common::NetworkEventData::EventName)
        .property("senderClientId", &csp::common::NetworkEventData::SenderClientId)
        .function(
            "getEventValues",
            +[](const csp::common::NetworkEventData& self) { return bindings::utils::JSDisposable<csp::common::Array<csp::common::ReplicatedValue>> { self.EventValues }; })
        .function(
            "setEventValues(value)", +[](csp::common::NetworkEventData& self, csp::common::Array<csp::common::ReplicatedValue> value) { self.EventValues = std::move(value); });

    emscripten::class_<csp::common::AssetDetailBlobChangedNetworkEventData, emscripten::base<csp::common::NetworkEventData>>("AssetDetailBlobChangedNetworkEventData")
        .class_function(
            "create", +[]() { return csp::common::AssetDetailBlobChangedNetworkEventData(); })
        .property("changeType", &csp::common::AssetDetailBlobChangedNetworkEventData::ChangeType)
        .property("assetId", &csp::common::AssetDetailBlobChangedNetworkEventData::AssetId)
        .property("version", &csp::common::AssetDetailBlobChangedNetworkEventData::Version)
        .property("assetType", &csp::common::AssetDetailBlobChangedNetworkEventData::AssetType)
        .property("assetCollectionId", &csp::common::AssetDetailBlobChangedNetworkEventData::AssetCollectionId);

    emscripten::class_<csp::common::ConversationNetworkEventData, emscripten::base<csp::common::NetworkEventData>>("ConversationNetworkEventData")
        .class_function("create", +[]() { return csp::common::ConversationNetworkEventData(); });
    //TODO: These are multiplayer objects. When multiplayer gets bound, bind these correctly (ie, if bound as classes, do get/set split with JSDisposable)
    // .property("messageType", &csp::common::ConversationNetworkEventData::MessageType)
    // .property("messageInfo", &csp::common::ConversationNetworkEventData::MessageInfo);

    emscripten::class_<csp::common::AccessControlChangedNetworkEventData, emscripten::base<csp::common::NetworkEventData>>("AccessControlChangedNetworkEventData")
        .class_function(
            "create", +[]() { return csp::common::AccessControlChangedNetworkEventData(); })
        .property("spaceId", &csp::common::AccessControlChangedNetworkEventData::SpaceId)
        .property("userRoles", &csp::common::AccessControlChangedNetworkEventData::UserRoles)
        .property("changeType", &csp::common::AccessControlChangedNetworkEventData::ChangeType)
        .property("userId", &csp::common::AccessControlChangedNetworkEventData::UserId);

    emscripten::class_<csp::common::SequenceChangedNetworkEventData, emscripten::base<csp::common::NetworkEventData>>("SequenceChangedNetworkEventData")
        .class_function(
            "create", +[]() { return csp::common::SequenceChangedNetworkEventData(); })
        .property("updateType", &csp::common::SequenceChangedNetworkEventData::UpdateType)
        .property("sequenceType", &csp::common::SequenceChangedNetworkEventData::SequenceType)
        .property("key", &csp::common::SequenceChangedNetworkEventData::Key)
        .property("newKey", &csp::common::SequenceChangedNetworkEventData::NewKey)
        .property("spaceId", &csp::common::SequenceChangedNetworkEventData::SpaceId);

    emscripten::class_<csp::common::AsyncCallCompletedEventData, emscripten::base<csp::common::NetworkEventData>>("AsyncCallCompletedEventData")
        .class_function(
            "create", +[]() { return csp::common::AsyncCallCompletedEventData(); })
        .property("operationName", &csp::common::AsyncCallCompletedEventData::OperationName)
        .property("references", &csp::common::AsyncCallCompletedEventData::References)
        .property("success", &csp::common::AsyncCallCompletedEventData::Success)
        .property("statusReason", &csp::common::AsyncCallCompletedEventData::StatusReason);

    emscripten::class_<csp::common::MaterialChangedParams>("MaterialChangedParams")
        .class_function(
            "create", +[]() { return csp::common::MaterialChangedParams(); })
        .property("materialCollectionId", &csp::common::MaterialChangedParams::MaterialCollectionId)
        .property("materialId", &csp::common::MaterialChangedParams::MaterialId)
        .property("changeType", &csp::common::MaterialChangedParams::ChangeType);
}
