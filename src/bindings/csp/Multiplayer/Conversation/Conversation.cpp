#include "../../../containers/Array.h"
#include "../../../containers/Map.h"
#include "../../../containers/String.h"
#include "../../../utils/JSDisposable.h"

#include "CSP/Common/Array.h"
#include "CSP/Common/Map.h"
#include "CSP/Common/String.h"
#include "CSP/Common/Vector.h"
#include "CSP/Multiplayer/Conversation/Conversation.h"
#include "CSP/Systems/Assets/Asset.h"
#include "CSP/Systems/WebService.h"

#include "emscripten/bind.h"

EMSCRIPTEN_BINDINGS(CSPConversation)
{
    emscripten::enum_<csp::multiplayer::ConversationEventType>("ConversationEventType", emscripten::enum_value_type::number)
        .value("NewConversation", csp::multiplayer::ConversationEventType::NewConversation)
        .value("NewMessage", csp::multiplayer::ConversationEventType::NewMessage)
        .value("DeleteMessage", csp::multiplayer::ConversationEventType::DeleteMessage)
        .value("DeleteConversation", csp::multiplayer::ConversationEventType::DeleteConversation)
        .value("ConversationInformation", csp::multiplayer::ConversationEventType::ConversationInformation)
        .value("MessageInformation", csp::multiplayer::ConversationEventType::MessageInformation)
        .value("SetAnnotation", csp::multiplayer::ConversationEventType::SetAnnotation)
        .value("DeleteAnnotation", csp::multiplayer::ConversationEventType::DeleteAnnotation)
        .value("SetConversationAnnotation", csp::multiplayer::ConversationEventType::SetConversationAnnotation)
        .value("DeleteConversationAnnotation", csp::multiplayer::ConversationEventType::DeleteConversationAnnotation);

    emscripten::class_<csp::multiplayer::MessageInfo>("MessageInfo")
        .class_function(
            "create", +[]() { return csp::multiplayer::MessageInfo(); })
        .class_function(
            "createWithMessage(conversationId, isConversation, message)",
            +[](const csp::common::String& conversationId, bool isConversation, const csp::common::String& message) {
                return csp::multiplayer::MessageInfo(conversationId, isConversation, message);
            })
        .class_function(
            "createWithMessageId(conversationId, isConversation, message, messageId)",
            +[](const csp::common::String& conversationId, bool isConversation, const csp::common::String& message, const csp::common::String& messageId) {
                return csp::multiplayer::MessageInfo(conversationId, isConversation, message, messageId);
            })
        .property("conversationId", &csp::multiplayer::MessageInfo::ConversationId)
        .property("createdTimestamp", &csp::multiplayer::MessageInfo::CreatedTimestamp)
        .property("editedTimestamp", &csp::multiplayer::MessageInfo::EditedTimestamp)
        .property("userId", &csp::multiplayer::MessageInfo::UserId)
        .property("message", &csp::multiplayer::MessageInfo::Message)
        .property("messageId", &csp::multiplayer::MessageInfo::MessageId)
        .function("equals(other)", &csp::multiplayer::MessageInfo::operator==);

    emscripten::class_<csp::multiplayer::MessageUpdateParams>("MessageUpdateParams")
        .class_function(
            "create(newMessage)",
            +[](const csp::common::String& newMessage) {
                csp::multiplayer::MessageUpdateParams messageUpdateParams;
                messageUpdateParams.NewMessage = newMessage;
                return messageUpdateParams;
            })
        .property("newMessage", &csp::multiplayer::MessageUpdateParams::NewMessage);

    emscripten::value_object<csp::multiplayer::AnnotationUpdateParams>("AnnotationUpdateParams")
        .field("verticalFov", &csp::multiplayer::AnnotationUpdateParams::VerticalFov)
        .field("authorCameraPosition", &csp::multiplayer::AnnotationUpdateParams::AuthorCameraPosition)
        .field("authorCameraRotation", &csp::multiplayer::AnnotationUpdateParams::AuthorCameraRotation);

    emscripten::class_<csp::multiplayer::AnnotationData>("AnnotationData")
        .class_function(
            "create", +[]() { return csp::multiplayer::AnnotationData(); })
        .class_function(
            "createWithData(annotationId, annotationThumbnailId, verticalFov, authorCameraPosition, authorCameraRotation)",
            +[](const csp::common::String& annotationId, const csp::common::String& annotationThumbnailId, double verticalFov, const csp::common::Vector3& authorCameraPosition,
                 const csp::common::Vector4& authorCameraRotation) {
                return csp::multiplayer::AnnotationData(annotationId, annotationThumbnailId, verticalFov, authorCameraPosition, authorCameraRotation);
            })
        .property("annotationId", &csp::multiplayer::AnnotationData::AnnotationId)
        .property("annotationThumbnailId", &csp::multiplayer::AnnotationData::AnnotationThumbnailId)
        .property("verticalFov", &csp::multiplayer::AnnotationData::VerticalFov)
        .property("authorCameraPosition", &csp::multiplayer::AnnotationData::AuthorCameraPosition)
        .property("authorCameraRotation", &csp::multiplayer::AnnotationData::AuthorCameraRotation);

    emscripten::class_<csp::multiplayer::MessageResult, emscripten::base<csp::systems::ResultBase>>("MessageResult")
        .function("getMessageInfo", +[](const csp::multiplayer::MessageResult& self) { return self.GetMessageInfo(); });

    emscripten::class_<csp::multiplayer::MessageCollectionResult, emscripten::base<csp::systems::ResultBase>>("MessageCollectionResult")
        .property("totalCount", &csp::multiplayer::MessageCollectionResult::GetTotalCount)
        .function(
            "getMessages", +[](const csp::multiplayer::MessageCollectionResult& self) {
                return bindings::utils::JSDisposable<csp::common::Array<csp::multiplayer::MessageInfo>> { self.GetMessages() };
            });

    emscripten::class_<csp::multiplayer::ConversationResult, emscripten::base<csp::systems::ResultBase>>("ConversationResult")
        .function("getConversationInfo", +[](const csp::multiplayer::ConversationResult& self) { return self.GetConversationInfo(); });

    emscripten::class_<csp::multiplayer::NumberOfRepliesResult, emscripten::base<csp::systems::ResultBase>>("NumberOfRepliesResult")
        .property("count", &csp::multiplayer::NumberOfRepliesResult::GetCount);

    emscripten::class_<csp::multiplayer::AnnotationResult, emscripten::base<csp::systems::ResultBase>>("AnnotationResult")
        .function(
            "getAnnotationData", +[](const csp::multiplayer::AnnotationResult& self) { return self.GetAnnotationData(); })
        .function(
            "getAnnotationAsset", +[](const csp::multiplayer::AnnotationResult& self) { return self.GetAnnotationAsset(); })
        .function("getAnnotationThumbnailAsset", +[](const csp::multiplayer::AnnotationResult& self) { return self.GetAnnotationThumbnailAsset(); });

    emscripten::class_<csp::multiplayer::AnnotationThumbnailCollectionResult, emscripten::base<csp::systems::ResultBase>>("AnnotationThumbnailCollectionResult")
        .property("totalCount", &csp::multiplayer::AnnotationThumbnailCollectionResult::GetTotalCount)
        .function(
            "getAnnotationThumbnailAssetsMap", +[](const csp::multiplayer::AnnotationThumbnailCollectionResult& self) {
                return bindings::utils::JSDisposable<csp::common::Map<csp::common::String, csp::systems::Asset>> { self.GetAnnotationThumbnailAssetsMap() };
            });
}
