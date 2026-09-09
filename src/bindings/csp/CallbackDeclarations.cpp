#include "CallbackDeclarations.h"

#include "CSP/Common/String.h"
#include "CSP/Common/Systems/Log/LogLevels.h"
#include "CSP/Multiplayer/SpaceEntity.h"

#include "emscripten/bind.h"
#include <cstdint>
#include <functional>

/*
 * Define all the callbacks used in the CSP api. Pairs with declarations in CallbackDeclarations.h
 *
 * The TypeScript signature given to each `DEFINE_CALLBACK` is what appears in the generated .d.ts
 * The Embind name, (ie, BooleanCallback), never appears in generated output, it's just for embind to know about.
 */

DEFINE_CALLBACK(std::function<void()>, VoidCallback, "() => void")

/* Common */
DEFINE_CALLBACK(csp::common::LogSystem::EventCallbackHandler, StringCallback, "(value: string) => void")
DEFINE_CALLBACK(csp::common::EntityFetchCompleteCallback, EntityFetchCompleteCallback, "(entityCount: number) => void")
DEFINE_CALLBACK(csp::common::LogSystem::LogCallbackHandler, LogCallback, "(level: LogLevel, message: string) => void")

/* Multiplayer */
DEFINE_CALLBACK(csp::multiplayer::EntityCreatedCallback, EntityCreatedCallback, "(entity: SpaceEntity | null) => void")
DEFINE_CALLBACK(csp::multiplayer::CallbackHandler, BooleanCallback, "(success: boolean) => void")
DEFINE_CALLBACK(
    csp::multiplayer::ComponentBase::EntityActionHandler, EntityActionHandlerCallback, "(component: ComponentBase | null, action: string, actionParams: string) => void")
DEFINE_CALLBACK(
    csp::multiplayer::SpaceEntity::UpdateCallback, SpaceEntityUpdateCallback, "(entity: SpaceEntity | null, updateFlags: number, componentUpdates: ComponentUpdateInfo[]) => void")
DEFINE_CALLBACK(csp::multiplayer::MultiplayerConnection::ErrorCodeCallbackHandler, ErrorCodeCallback, "(errorCode: ErrorCode) => void")
DEFINE_CALLBACK(csp::multiplayer::MultiplayerConnection::ConnectionCallbackHandler, ConnectionStatusCallback, "(status: string) => void")
DEFINE_CALLBACK(csp::multiplayer::OnlineRealtimeEngine::ScopeLeaderCallback, ScopeLeaderCallback, "(scopeId: string, userId: string) => void")
DEFINE_CALLBACK(csp::multiplayer::CustomNetworkEventCallback, CustomNetworkEventCallback, "(networkEventData: NetworkEventData) => void")
DEFINE_CALLBACK(
    csp::multiplayer::AccessControlChangedEventCallback, AccessControlChangedEventCallback, "(accessControlChangedEventData: AccessControlChangedNetworkEventData) => void")
DEFINE_CALLBACK(
    csp::multiplayer::AssetDetailBlobChangedEventCallback, AssetDetailBlobChangedEventCallback, "(assetDetailBlobChangedEventData: AssetDetailBlobChangedNetworkEventData) => void")
DEFINE_CALLBACK(csp::multiplayer::AsyncCallCompletedEventCallback, AsyncCallCompletedEventCallback, "(asyncCallCompletedEventData: AsyncCallCompletedEventData) => void")
DEFINE_CALLBACK(csp::multiplayer::ConversationEventCallback, ConversationEventCallback, "(conversationNetworkEventData: ConversationNetworkEventData) => void")
DEFINE_CALLBACK(csp::multiplayer::SequenceChangedEventCallback, SequenceChangedEventCallback, "(sequenceChangedEventData: SequenceChangedNetworkEventData) => void")
DEFINE_CALLBACK(csp::multiplayer::MessageResultCallback, MessageResultCallback, "(result: MessageResult) => void")
DEFINE_CALLBACK(csp::multiplayer::MessageCollectionResultCallback, MessageCollectionResultCallback, "(result: MessageCollectionResult) => void")
DEFINE_CALLBACK(csp::multiplayer::ConversationResultCallback, ConversationResultCallback, "(result: ConversationResult) => void")
DEFINE_CALLBACK(csp::multiplayer::NumberOfRepliesResultCallback, NumberOfRepliesResultCallback, "(result: NumberOfRepliesResult) => void")
DEFINE_CALLBACK(csp::multiplayer::AnnotationResultCallback, AnnotationResultCallback, "(result: AnnotationResult) => void")
DEFINE_CALLBACK(csp::multiplayer::AnnotationThumbnailCollectionResultCallback, AnnotationThumbnailCollectionResultCallback, "(result: AnnotationThumbnailCollectionResult) => void")

/* Systems */
DEFINE_CALLBACK(csp::systems::NullResultCallback, NullResultCallback, "(result: NullResult) => void")
DEFINE_CALLBACK(csp::systems::StringResultCallback, StringResultCallback, "(result: StringResult) => void")
