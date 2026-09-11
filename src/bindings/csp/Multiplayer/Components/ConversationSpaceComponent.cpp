#include "../../../async/Promises.h"
#include "../../../containers/Optional.h"
#include "../../../containers/String.h"
#include "../../CallbackDeclarations.h"
#include "../../PromiseDeclarations.h"

#include "CSP/Common/Optional.h"
#include "CSP/Common/String.h"
#include "CSP/Common/Vector.h"
#include "CSP/Multiplayer/Components/ConversationSpaceComponent.h"
#include "CSP/Systems/Assets/Asset.h"

#include "emscripten/bind.h"
#include "emscripten/val.h"

EMSCRIPTEN_BINDINGS(CSPConversationSpaceComponent)
{
    emscripten::enum_<csp::multiplayer::ConversationPropertyKeys>("ConversationPropertyKeys", emscripten::enum_value_type::number)
        .value("ConversationId", csp::multiplayer::ConversationPropertyKeys::ConversationId)
        .value("IsVisible", csp::multiplayer::ConversationPropertyKeys::IsVisible)
        .value("IsActive", csp::multiplayer::ConversationPropertyKeys::IsActive)
        .value("Position", csp::multiplayer::ConversationPropertyKeys::Position)
        .value("Rotation", csp::multiplayer::ConversationPropertyKeys::Rotation)
        .value("Title", csp::multiplayer::ConversationPropertyKeys::Title)
        .value("Resolved", csp::multiplayer::ConversationPropertyKeys::Resolved)
        .value("ConversationCameraPosition", csp::multiplayer::ConversationPropertyKeys::ConversationCameraPosition)
        .value("ConversationCameraRotation", csp::multiplayer::ConversationPropertyKeys::ConversationCameraRotation)
        .value("Num", csp::multiplayer::ConversationPropertyKeys::Num);

    emscripten::class_<csp::multiplayer::ConversationSpaceComponent, emscripten::base<csp::multiplayer::ComponentBase>>("ConversationSpaceComponent")
        .property("position", &csp::multiplayer::ConversationSpaceComponent::GetPosition, &csp::multiplayer::ConversationSpaceComponent::SetPosition)
        .property("rotation", &csp::multiplayer::ConversationSpaceComponent::GetRotation, &csp::multiplayer::ConversationSpaceComponent::SetRotation)
        .property("isVisible", &csp::multiplayer::ConversationSpaceComponent::GetIsVisible, &csp::multiplayer::ConversationSpaceComponent::SetIsVisible)
        .property("isActive", &csp::multiplayer::ConversationSpaceComponent::GetIsActive, &csp::multiplayer::ConversationSpaceComponent::SetIsActive)
        .property("title", &csp::multiplayer::ConversationSpaceComponent::GetTitle, &csp::multiplayer::ConversationSpaceComponent::SetTitle)
        .property("resolved", &csp::multiplayer::ConversationSpaceComponent::GetResolved, &csp::multiplayer::ConversationSpaceComponent::SetResolved)
        .property("conversationCameraPosition", &csp::multiplayer::ConversationSpaceComponent::GetConversationCameraPosition,
            &csp::multiplayer::ConversationSpaceComponent::SetConversationCameraPosition)
        .property("conversationCameraRotation", &csp::multiplayer::ConversationSpaceComponent::GetConversationCameraRotation,
            &csp::multiplayer::ConversationSpaceComponent::SetConversationCameraRotation)
        .function(
            "setConversationUpdateCallback(callback)",
            +[](csp::multiplayer::ConversationSpaceComponent& self, ConversationEventCallback callback) { self.SetConversationUpdateCallback(ToNativeCallback(callback)); })
        .function(
            "createConversation(message)",
            +[](csp::multiplayer::ConversationSpaceComponent& self, const csp::common::String& message) {
                return Promisify<PromiseOfStringResult>([&](emscripten::val cb) { self.CreateConversation(message, ToNativeCallback(cb.as<StringResultCallback>())); });
            })
        .function(
            "deleteConversation",
            +[](csp::multiplayer::ConversationSpaceComponent& self) {
                return Promisify<PromiseOfNullResult>([&](emscripten::val cb) { self.DeleteConversation(ToNativeCallback(cb.as<NullResultCallback>())); });
            })
        .function(
            "addMessage(message)",
            +[](csp::multiplayer::ConversationSpaceComponent& self, const csp::common::String& message) {
                return Promisify<PromiseOfMessageResult>([&](emscripten::val cb) { self.AddMessage(message, ToNativeCallback(cb.as<MessageResultCallback>())); });
            })
        .function(
            "deleteMessage(messageId)",
            +[](csp::multiplayer::ConversationSpaceComponent& self, const csp::common::String& messageId) {
                return Promisify<PromiseOfNullResult>([&](emscripten::val cb) { self.DeleteMessage(messageId, ToNativeCallback(cb.as<NullResultCallback>())); });
            })
        .function(
            "getMessagesFromConversation(resultsSkipNumber, resultsMaxNumber)",
            +[](csp::multiplayer::ConversationSpaceComponent& self, const csp::common::Optional<int>& resultsSkipNumber, const csp::common::Optional<int>& resultsMaxNumber) {
                return Promisify<PromiseOfMessageCollectionResult>(
                    [&](emscripten::val cb) { self.GetMessagesFromConversation(resultsSkipNumber, resultsMaxNumber, ToNativeCallback(cb.as<MessageCollectionResultCallback>())); });
            })
        .function(
            "getConversationInfo",
            +[](csp::multiplayer::ConversationSpaceComponent& self) {
                return Promisify<PromiseOfConversationResult>([&](emscripten::val cb) { self.GetConversationInfo(ToNativeCallback(cb.as<ConversationResultCallback>())); });
            })
        .function(
            "updateConversation(newData)",
            +[](csp::multiplayer::ConversationSpaceComponent& self, const csp::multiplayer::MessageUpdateParams& newData) {
                return Promisify<PromiseOfConversationResult>([&](emscripten::val cb) { self.UpdateConversation(newData, ToNativeCallback(cb.as<ConversationResultCallback>())); });
            })
        .function(
            "getMessageInfo(messageId)",
            +[](csp::multiplayer::ConversationSpaceComponent& self, const csp::common::String& messageId) {
                return Promisify<PromiseOfMessageResult>([&](emscripten::val cb) { self.GetMessageInfo(messageId, ToNativeCallback(cb.as<MessageResultCallback>())); });
            })
        .function(
            "updateMessage(messageId, newData)",
            +[](csp::multiplayer::ConversationSpaceComponent& self, const csp::common::String& messageId, const csp::multiplayer::MessageUpdateParams& newData) {
                return Promisify<PromiseOfMessageResult>([&](emscripten::val cb) { self.UpdateMessage(messageId, newData, ToNativeCallback(cb.as<MessageResultCallback>())); });
            })
        .function(
            "getNumberOfReplies",
            +[](csp::multiplayer::ConversationSpaceComponent& self) {
                return Promisify<PromiseOfNumberOfRepliesResult>([&](emscripten::val cb) { self.GetNumberOfReplies(ToNativeCallback(cb.as<NumberOfRepliesResultCallback>())); });
            })
        .function(
            "getConversationAnnotation",
            +[](csp::multiplayer::ConversationSpaceComponent& self) {
                return Promisify<PromiseOfAnnotationResult>([&](emscripten::val cb) { self.GetConversationAnnotation(ToNativeCallback(cb.as<AnnotationResultCallback>())); });
            })
        .function(
            "setConversationAnnotation(annotationParams, annotation, annotationThumbnail)",
            +[](csp::multiplayer::ConversationSpaceComponent& self, const csp::multiplayer::AnnotationUpdateParams& annotationParams,
                 const csp::systems::BufferAssetDataSource& annotation, const csp::systems::BufferAssetDataSource& annotationThumbnail) {
                return Promisify<PromiseOfAnnotationResult>([&](emscripten::val cb) {
                    self.SetConversationAnnotation(annotationParams, annotation, annotationThumbnail, ToNativeCallback(cb.as<AnnotationResultCallback>()));
                });
            })
        .function(
            "deleteConversationAnnotation",
            +[](csp::multiplayer::ConversationSpaceComponent& self) {
                return Promisify<PromiseOfNullResult>([&](emscripten::val cb) { self.DeleteConversationAnnotation(ToNativeCallback(cb.as<NullResultCallback>())); });
            })
        .function(
            "getAnnotation(messageId)",
            +[](csp::multiplayer::ConversationSpaceComponent& self, const csp::common::String& messageId) {
                return Promisify<PromiseOfAnnotationResult>([&](emscripten::val cb) { self.GetAnnotation(messageId, ToNativeCallback(cb.as<AnnotationResultCallback>())); });
            })
        .function(
            "setAnnotation(messageId, updateParams, annotation, annotationThumbnail)",
            +[](csp::multiplayer::ConversationSpaceComponent& self, const csp::common::String& messageId, const csp::multiplayer::AnnotationUpdateParams& updateParams,
                 const csp::systems::BufferAssetDataSource& annotation, const csp::systems::BufferAssetDataSource& annotationThumbnail) {
                return Promisify<PromiseOfAnnotationResult>(
                    [&](emscripten::val cb) { self.SetAnnotation(messageId, updateParams, annotation, annotationThumbnail, ToNativeCallback(cb.as<AnnotationResultCallback>())); });
            })
        .function(
            "deleteAnnotation(messageId)",
            +[](csp::multiplayer::ConversationSpaceComponent& self, const csp::common::String& messageId) {
                return Promisify<PromiseOfNullResult>([&](emscripten::val cb) { self.DeleteAnnotation(messageId, ToNativeCallback(cb.as<NullResultCallback>())); });
            })
        .function(
            "getAnnotationThumbnailsForConversation", +[](csp::multiplayer::ConversationSpaceComponent& self) {
                return Promisify<PromiseOfAnnotationThumbnailCollectionResult>(
                    [&](emscripten::val cb) { self.GetAnnotationThumbnailsForConversation(ToNativeCallback(cb.as<AnnotationThumbnailCollectionResultCallback>())); });
            });
}
