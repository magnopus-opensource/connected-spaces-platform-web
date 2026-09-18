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
#include "CSP/Systems/Assets/Asset.h"
#include "CSP/Systems/Assets/AssetCollection.h"
#include "CSP/Systems/Assets/AssetSystem.h"
#include "CSP/Systems/Assets/LOD.h"
#include "CSP/Systems/Assets/Material.h"
#include "CSP/Systems/ECommerce/ECommerce.h"
#include "CSP/Systems/EventTicketing/EventTicketing.h"
#include "CSP/Systems/GraphQL/GraphQL.h"
#include "CSP/Systems/HotspotSequence/HotspotGroup.h"
#include "CSP/Systems/Maintenance/Maintenance.h"
#include "CSP/Systems/Quota/Quota.h"
#include "CSP/Systems/Sequence/Sequence.h"
#include "CSP/Systems/Settings/ApplicationSettings.h"
#include "CSP/Systems/Settings/SettingsCollection.h"
#include "CSP/Systems/Spaces/Site.h"
#include "CSP/Systems/Spaces/Space.h"
#include "CSP/Systems/Spaces/UserRoles.h"
#include "CSP/Systems/Spatial/Anchor.h"
#include "CSP/Systems/Spatial/PointOfInterest.h"
#include "CSP/Systems/SystemsResult.h"
#include "CSP/Systems/Users/Authentication.h"
#include "CSP/Systems/Users/Profile.h"

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
DECLARE_CALLBACK(csp::systems::BooleanResultCallback, BooleanResultCallback)
DECLARE_CALLBACK(csp::systems::StringResultCallback, StringResultCallback)
DECLARE_CALLBACK(csp::systems::StringArrayResultCallback, StringArrayResultCallback)
DECLARE_CALLBACK(csp::systems::UInt64ResultCallback, UInt64ResultCallback)
DECLARE_CALLBACK(csp::systems::AssetResultCallback, AssetResultCallback)
DECLARE_CALLBACK(csp::systems::AssetsResultCallback, AssetsResultCallback)
DECLARE_CALLBACK(csp::systems::UriResultCallback, UriResultCallback)
DECLARE_CALLBACK(csp::systems::AssetDataResultCallback, AssetDataResultCallback)
DECLARE_CALLBACK(csp::systems::AssetCollectionResultCallback, AssetCollectionResultCallback)
DECLARE_CALLBACK(csp::systems::AssetCollectionsResultCallback, AssetCollectionsResultCallback)
DECLARE_CALLBACK(csp::systems::AssetCollectionCountResultCallback, AssetCollectionCountResultCallback)
DECLARE_CALLBACK(csp::systems::LODChainResultCallback, LODChainResultCallback)
DECLARE_CALLBACK(csp::systems::MaterialResultCallback, MaterialResultCallback)
DECLARE_CALLBACK(csp::systems::MaterialsResultCallback, MaterialsResultCallback)
DECLARE_CALLBACK(csp::systems::AssetSystem::MaterialChangedCallbackHandler, MaterialChangedCallback)
DECLARE_CALLBACK(csp::systems::SpaceResultCallback, SpaceResultCallback)
DECLARE_CALLBACK(csp::systems::SpacesResultCallback, SpacesResultCallback)
DECLARE_CALLBACK(csp::systems::BasicSpaceResultCallback, BasicSpaceResultCallback)
DECLARE_CALLBACK(csp::systems::BasicSpacesResultCallback, BasicSpacesResultCallback)
DECLARE_CALLBACK(csp::systems::SpaceMetadataResultCallback, SpaceMetadataResultCallback)
DECLARE_CALLBACK(csp::systems::SpacesMetadataResultCallback, SpacesMetadataResultCallback)
DECLARE_CALLBACK(csp::systems::PendingInvitesResultCallback, PendingInvitesResultCallback)
DECLARE_CALLBACK(csp::systems::AcceptedInvitesResultCallback, AcceptedInvitesResultCallback)
DECLARE_CALLBACK(csp::systems::SpaceGeoLocationResultCallback, SpaceGeoLocationResultCallback)
DECLARE_CALLBACK(csp::systems::SiteResultCallback, SiteResultCallback)
DECLARE_CALLBACK(csp::systems::SitesCollectionResultCallback, SitesCollectionResultCallback)
DECLARE_CALLBACK(csp::systems::UserRoleCollectionCallback, UserRoleCollectionCallback)
DECLARE_CALLBACK(csp::systems::ProfileResultCallback, ProfileResultCallback)
DECLARE_CALLBACK(csp::systems::BasicProfilesResultCallback, BasicProfilesResultCallback)
DECLARE_CALLBACK(csp::systems::LoginStateResultCallback, LoginStateResultCallback)
DECLARE_CALLBACK(csp::systems::LoginTokenInfoResultCallback, LoginTokenInfoResultCallback)
DECLARE_CALLBACK(csp::systems::FeaturesLimitCallback, FeaturesLimitCallback)
DECLARE_CALLBACK(csp::systems::FeatureLimitCallback, FeatureLimitCallback)
DECLARE_CALLBACK(csp::systems::UserTierCallback, UserTierCallback)
DECLARE_CALLBACK(csp::systems::FeatureQuotaCallback, FeatureQuotaCallback)
DECLARE_CALLBACK(csp::systems::FeaturesQuotaCallback, FeaturesQuotaCallback)
DECLARE_CALLBACK(csp::systems::SequenceResultCallback, SequenceResultCallback)
DECLARE_CALLBACK(csp::systems::SequencesResultCallback, SequencesResultCallback)
DECLARE_CALLBACK(csp::systems::HotspotGroupResultCallback, HotspotGroupResultCallback)
DECLARE_CALLBACK(csp::systems::HotspotGroupsResultCallback, HotspotGroupsResultCallback)
DECLARE_CALLBACK(csp::systems::TicketedEventResultCallback, TicketedEventResultCallback)
DECLARE_CALLBACK(csp::systems::TicketedEventCollectionResultCallback, TicketedEventCollectionResultCallback)
DECLARE_CALLBACK(csp::systems::EventTicketResultCallback, EventTicketResultCallback)
DECLARE_CALLBACK(csp::systems::SpaceIsTicketedResultCallback, SpaceIsTicketedResultCallback)
DECLARE_CALLBACK(csp::systems::TicketedEventVendorAuthorizeInfoCallback, TicketedEventVendorAuthorizeInfoCallback)
DECLARE_CALLBACK(csp::systems::GraphQLReceivedCallback, GraphQLReceivedCallback)
DECLARE_CALLBACK(csp::systems::MaintenanceInfoCallback, MaintenanceInfoCallback)
DECLARE_CALLBACK(csp::systems::ProductInfoResultCallback, ProductInfoResultCallback)
DECLARE_CALLBACK(csp::systems::ProductInfoCollectionResultCallback, ProductInfoCollectionResultCallback)
DECLARE_CALLBACK(csp::systems::CheckoutInfoResultCallback, CheckoutInfoResultCallback)
DECLARE_CALLBACK(csp::systems::CartInfoResultCallback, CartInfoResultCallback)
DECLARE_CALLBACK(csp::systems::AddShopifyStoreResultCallback, AddShopifyStoreResultCallback)
DECLARE_CALLBACK(csp::systems::ValidateShopifyStoreResultCallback, ValidateShopifyStoreResultCallback)
DECLARE_CALLBACK(csp::systems::GetShopifyStoresResultCallback, GetShopifyStoresResultCallback)
DECLARE_CALLBACK(csp::systems::ApplicationSettingsResultCallback, ApplicationSettingsResultCallback)
DECLARE_CALLBACK(csp::systems::AvatarInfoResultCallback, AvatarInfoResultCallback)
DECLARE_CALLBACK(csp::systems::AnchorResultCallback, AnchorResultCallback)
DECLARE_CALLBACK(csp::systems::AnchorCollectionResultCallback, AnchorCollectionResultCallback)
DECLARE_CALLBACK(csp::systems::AnchorResolutionResultCallback, AnchorResolutionResultCallback)
DECLARE_CALLBACK(csp::systems::POIResultCallback, POIResultCallback)
DECLARE_CALLBACK(csp::systems::POICollectionResultCallback, POICollectionResultCallback)

EMSCRIPTEN_DECLARE_VAL_TYPE(ProgressCallback);
