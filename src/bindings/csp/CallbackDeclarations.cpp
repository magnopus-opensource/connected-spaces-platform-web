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
DEFINE_CALLBACK(csp::systems::BooleanResultCallback, BooleanResultCallback, "(result: BooleanResult) => void")
DEFINE_CALLBACK(csp::systems::StringResultCallback, StringResultCallback, "(result: StringResult) => void")
DEFINE_CALLBACK(csp::systems::StringArrayResultCallback, StringArrayResultCallback, "(result: StringArrayResult) => void")
DEFINE_CALLBACK(csp::systems::UInt64ResultCallback, UInt64ResultCallback, "(result: UInt64Result) => void")
DEFINE_CALLBACK(csp::systems::AssetResultCallback, AssetResultCallback, "(result: AssetResult) => void")
DEFINE_CALLBACK(csp::systems::AssetsResultCallback, AssetsResultCallback, "(result: AssetsResult) => void")
DEFINE_CALLBACK(csp::systems::UriResultCallback, UriResultCallback, "(result: UriResult) => void")
DEFINE_CALLBACK(csp::systems::AssetDataResultCallback, AssetDataResultCallback, "(result: AssetDataResult) => void")
DEFINE_CALLBACK(csp::systems::AssetCollectionResultCallback, AssetCollectionResultCallback, "(result: AssetCollectionResult) => void")
DEFINE_CALLBACK(csp::systems::AssetCollectionsResultCallback, AssetCollectionsResultCallback, "(result: AssetCollectionsResult) => void")
DEFINE_CALLBACK(csp::systems::AssetCollectionCountResultCallback, AssetCollectionCountResultCallback, "(result: AssetCollectionCountResult) => void")
DEFINE_CALLBACK(csp::systems::LODChainResultCallback, LODChainResultCallback, "(result: LODChainResult) => void")
DEFINE_CALLBACK(csp::systems::MaterialResultCallback, MaterialResultCallback, "(result: MaterialResult) => void")
DEFINE_CALLBACK(csp::systems::MaterialsResultCallback, MaterialsResultCallback, "(result: MaterialsResult) => void")
DEFINE_CALLBACK(csp::systems::AssetSystem::MaterialChangedCallbackHandler, MaterialChangedCallback, "(materialChangedParams: MaterialChangedParams) => void")
DEFINE_CALLBACK(csp::systems::SpaceResultCallback, SpaceResultCallback, "(result: SpaceResult) => void")
DEFINE_CALLBACK(csp::systems::SpacesResultCallback, SpacesResultCallback, "(result: SpacesResult) => void")
DEFINE_CALLBACK(csp::systems::BasicSpaceResultCallback, BasicSpaceResultCallback, "(result: BasicSpaceResult) => void")
DEFINE_CALLBACK(csp::systems::BasicSpacesResultCallback, BasicSpacesResultCallback, "(result: BasicSpacesResult) => void")
DEFINE_CALLBACK(csp::systems::SpaceMetadataResultCallback, SpaceMetadataResultCallback, "(result: SpaceMetadataResult) => void")
DEFINE_CALLBACK(csp::systems::SpacesMetadataResultCallback, SpacesMetadataResultCallback, "(result: SpacesMetadataResult) => void")
DEFINE_CALLBACK(csp::systems::PendingInvitesResultCallback, PendingInvitesResultCallback, "(result: PendingInvitesResult) => void")
DEFINE_CALLBACK(csp::systems::AcceptedInvitesResultCallback, AcceptedInvitesResultCallback, "(result: AcceptedInvitesResult) => void")
DEFINE_CALLBACK(csp::systems::SpaceGeoLocationResultCallback, SpaceGeoLocationResultCallback, "(result: SpaceGeoLocationResult) => void")
DEFINE_CALLBACK(csp::systems::SiteResultCallback, SiteResultCallback, "(result: SiteResult) => void")
DEFINE_CALLBACK(csp::systems::SitesCollectionResultCallback, SitesCollectionResultCallback, "(result: SitesCollectionResult) => void")
DEFINE_CALLBACK(csp::systems::UserRoleCollectionCallback, UserRoleCollectionCallback, "(result: UserRoleCollectionResult) => void")
DEFINE_CALLBACK(csp::systems::ProfileResultCallback, ProfileResultCallback, "(result: ProfileResult) => void")
DEFINE_CALLBACK(csp::systems::BasicProfilesResultCallback, BasicProfilesResultCallback, "(result: BasicProfilesResult) => void")
DEFINE_CALLBACK(csp::systems::LoginStateResultCallback, LoginStateResultCallback, "(result: LoginStateResult) => void")
DEFINE_CALLBACK(csp::systems::LoginTokenInfoResultCallback, LoginTokenInfoResultCallback, "(result: LoginTokenInfoResult) => void")
DEFINE_CALLBACK(csp::systems::FeaturesLimitCallback, FeaturesLimitCallback, "(result: FeaturesLimitResult) => void")
DEFINE_CALLBACK(csp::systems::FeatureLimitCallback, FeatureLimitCallback, "(result: FeatureLimitResult) => void")
DEFINE_CALLBACK(csp::systems::UserTierCallback, UserTierCallback, "(result: UserTierResult) => void")
DEFINE_CALLBACK(csp::systems::FeatureQuotaCallback, FeatureQuotaCallback, "(result: FeatureQuotaResult) => void")
DEFINE_CALLBACK(csp::systems::FeaturesQuotaCallback, FeaturesQuotaCallback, "(result: FeaturesQuotaResult) => void")
DEFINE_CALLBACK(csp::systems::SequenceResultCallback, SequenceResultCallback, "(result: SequenceResult) => void")
DEFINE_CALLBACK(csp::systems::SequencesResultCallback, SequencesResultCallback, "(result: SequencesResult) => void")
DEFINE_CALLBACK(csp::systems::HotspotGroupResultCallback, HotspotGroupResultCallback, "(result: HotspotGroupResult) => void")
DEFINE_CALLBACK(csp::systems::HotspotGroupsResultCallback, HotspotGroupsResultCallback, "(result: HotspotGroupsResult) => void")
DEFINE_CALLBACK(csp::systems::TicketedEventResultCallback, TicketedEventResultCallback, "(result: TicketedEventResult) => void")
DEFINE_CALLBACK(csp::systems::TicketedEventCollectionResultCallback, TicketedEventCollectionResultCallback, "(result: TicketedEventCollectionResult) => void")
DEFINE_CALLBACK(csp::systems::EventTicketResultCallback, EventTicketResultCallback, "(result: EventTicketResult) => void")
DEFINE_CALLBACK(csp::systems::SpaceIsTicketedResultCallback, SpaceIsTicketedResultCallback, "(result: SpaceIsTicketedResult) => void")
DEFINE_CALLBACK(csp::systems::TicketedEventVendorAuthorizeInfoCallback, TicketedEventVendorAuthorizeInfoCallback, "(result: TicketedEventVendorAuthInfoResult) => void")
DEFINE_CALLBACK(csp::systems::GraphQLReceivedCallback, GraphQLReceivedCallback, "(result: GraphQLResult) => void")
DEFINE_CALLBACK(csp::systems::MaintenanceInfoCallback, MaintenanceInfoCallback, "(result: MaintenanceInfoResult) => void")
DEFINE_CALLBACK(csp::systems::ProductInfoResultCallback, ProductInfoResultCallback, "(result: ProductInfoResult) => void")
DEFINE_CALLBACK(csp::systems::ProductInfoCollectionResultCallback, ProductInfoCollectionResultCallback, "(result: ProductInfoCollectionResult) => void")
DEFINE_CALLBACK(csp::systems::CheckoutInfoResultCallback, CheckoutInfoResultCallback, "(result: CheckoutInfoResult) => void")
DEFINE_CALLBACK(csp::systems::CartInfoResultCallback, CartInfoResultCallback, "(result: CartInfoResult) => void")
DEFINE_CALLBACK(csp::systems::AddShopifyStoreResultCallback, AddShopifyStoreResultCallback, "(result: AddShopifyStoreResult) => void")
DEFINE_CALLBACK(csp::systems::ValidateShopifyStoreResultCallback, ValidateShopifyStoreResultCallback, "(result: ValidateShopifyStoreResult) => void")
DEFINE_CALLBACK(csp::systems::GetShopifyStoresResultCallback, GetShopifyStoresResultCallback, "(result: GetShopifyStoresResult) => void")
DEFINE_CALLBACK(csp::systems::ApplicationSettingsResultCallback, ApplicationSettingsResultCallback, "(result: ApplicationSettingsResult) => void")
DEFINE_CALLBACK(csp::systems::AvatarInfoResultCallback, AvatarInfoResultCallback, "(result: AvatarInfoResult) => void")
DEFINE_CALLBACK(csp::systems::AnchorResultCallback, AnchorResultCallback, "(result: AnchorResult) => void")
DEFINE_CALLBACK(csp::systems::AnchorCollectionResultCallback, AnchorCollectionResultCallback, "(result: AnchorCollectionResult) => void")
DEFINE_CALLBACK(csp::systems::AnchorResolutionResultCallback, AnchorResolutionResultCallback, "(result: AnchorResolutionResult) => void")
DEFINE_CALLBACK(csp::systems::POIResultCallback, POIResultCallback, "(result: POIResult) => void")
DEFINE_CALLBACK(csp::systems::POICollectionResultCallback, POICollectionResultCallback, "(result: POICollectionResult) => void")

EMSCRIPTEN_BINDINGS(CSPProgressCallbackDeclaration)
{
    emscripten::register_type<ProgressCallback>("((requestProgress: number, responseProgress: number) => void)");
    emscripten::register_optional<ProgressCallback>();
}
