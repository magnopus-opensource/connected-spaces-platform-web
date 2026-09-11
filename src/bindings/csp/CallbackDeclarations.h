#pragma once

#include "../async/Callbacks.h"

#include "CSP/Common/Interfaces/IRealtimeEngine.h"
#include "CSP/Common/Systems/Log/LogSystem.h"
#include "CSP/Multiplayer/ComponentBase.h"
#include "CSP/Multiplayer/Conversation/Conversation.h"
#include "CSP/Multiplayer/MultiPlayerConnection.h"
#include "CSP/Multiplayer/NetworkEventBus.h"
#include "CSP/Multiplayer/OnlineRealtimeEngine.h"
#include "CSP/Multiplayer/SpaceEntity.h"
#include "CSP/Systems/SystemsResult.h"

#include <functional>

/*
 * Declare all the callbacks used in the CSP api. Pairs with definitions in CallbackDeclarations.cpp
 * Often times the Embind name given matches the CSP name, so fully qualify the CSP name declarations here to be safe.
 */

DECLARE_CALLBACK(std::function<void()>, VoidCallback)

/* Common */
DECLARE_CALLBACK(csp::common::LogSystem::EventCallbackHandler, StringCallback)
DECLARE_CALLBACK(csp::common::EntityFetchCompleteCallback, EntityFetchCompleteCallback)
DECLARE_CALLBACK(csp::common::LogSystem::LogCallbackHandler, LogCallback)

/* Multiplayer */
DECLARE_CALLBACK(csp::multiplayer::EntityCreatedCallback, EntityCreatedCallback)
DECLARE_CALLBACK(csp::multiplayer::CallbackHandler, BooleanCallback)
DECLARE_CALLBACK(csp::multiplayer::ComponentBase::EntityActionHandler, EntityActionHandlerCallback)
DECLARE_CALLBACK(csp::multiplayer::SpaceEntity::UpdateCallback, SpaceEntityUpdateCallback)
DECLARE_CALLBACK(csp::multiplayer::MultiplayerConnection::ErrorCodeCallbackHandler, ErrorCodeCallback)
DECLARE_CALLBACK(csp::multiplayer::MultiplayerConnection::ConnectionCallbackHandler, ConnectionStatusCallback)
DECLARE_CALLBACK(csp::multiplayer::OnlineRealtimeEngine::ScopeLeaderCallback, ScopeLeaderCallback)
DECLARE_CALLBACK(csp::multiplayer::CustomNetworkEventCallback, CustomNetworkEventCallback)
DECLARE_CALLBACK(csp::multiplayer::AccessControlChangedEventCallback, AccessControlChangedEventCallback)
DECLARE_CALLBACK(csp::multiplayer::AssetDetailBlobChangedEventCallback, AssetDetailBlobChangedEventCallback)
DECLARE_CALLBACK(csp::multiplayer::AsyncCallCompletedEventCallback, AsyncCallCompletedEventCallback)
DECLARE_CALLBACK(csp::multiplayer::ConversationEventCallback, ConversationEventCallback)
DECLARE_CALLBACK(csp::multiplayer::SequenceChangedEventCallback, SequenceChangedEventCallback)
DECLARE_CALLBACK(csp::multiplayer::MessageResultCallback, MessageResultCallback)
DECLARE_CALLBACK(csp::multiplayer::MessageCollectionResultCallback, MessageCollectionResultCallback)
DECLARE_CALLBACK(csp::multiplayer::ConversationResultCallback, ConversationResultCallback)
DECLARE_CALLBACK(csp::multiplayer::NumberOfRepliesResultCallback, NumberOfRepliesResultCallback)
DECLARE_CALLBACK(csp::multiplayer::AnnotationResultCallback, AnnotationResultCallback)
DECLARE_CALLBACK(csp::multiplayer::AnnotationThumbnailCollectionResultCallback, AnnotationThumbnailCollectionResultCallback)

/* Systems */
DECLARE_CALLBACK(csp::systems::NullResultCallback, NullResultCallback)
DECLARE_CALLBACK(csp::systems::StringResultCallback, StringResultCallback)
