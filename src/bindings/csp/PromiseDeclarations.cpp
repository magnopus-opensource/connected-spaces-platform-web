#include "PromiseDeclarations.h"

#include "emscripten/bind.h"

/*
 * Registrations for the promise types
 * Typescript sigs are defined in PromiseDeclarations.cpp
 */

EMSCRIPTEN_BINDINGS(CSPPromiseDeclarations)
{
    emscripten::register_type<PromiseOfBoolean>("Promise<boolean>");
    emscripten::register_type<PromiseOfSpaceEntityPointer>("Promise<SpaceEntity | null>");
    emscripten::register_type<PromiseOfErrorCode>("Promise<ErrorCode>");
    emscripten::register_type<PromiseOfNullResult>("Promise<NullResult>");
    emscripten::register_type<PromiseOfStringResult>("Promise<StringResult>");
    emscripten::register_type<PromiseOfMessageResult>("Promise<MessageResult>");
    emscripten::register_type<PromiseOfMessageCollectionResult>("Promise<MessageCollectionResult>");
    emscripten::register_type<PromiseOfConversationResult>("Promise<ConversationResult>");
    emscripten::register_type<PromiseOfNumberOfRepliesResult>("Promise<NumberOfRepliesResult>");
    emscripten::register_type<PromiseOfAnnotationResult>("Promise<AnnotationResult>");
    emscripten::register_type<PromiseOfAnnotationThumbnailCollectionResult>("Promise<AnnotationThumbnailCollectionResult>");

    emscripten::register_type<PromiseOfBooleanResult>("Promise<BooleanResult>");
    emscripten::register_type<PromiseOfStringArrayResult>("Promise<StringArrayResult>");
    emscripten::register_type<PromiseOfUInt64Result>("Promise<UInt64Result>");
    emscripten::register_type<PromiseOfAssetResult>("Promise<AssetResult>");
    emscripten::register_type<PromiseOfAssetsResult>("Promise<AssetsResult>");
    emscripten::register_type<PromiseOfUriResult>("Promise<UriResult>");
    emscripten::register_type<PromiseOfAssetDataResult>("Promise<AssetDataResult>");
    emscripten::register_type<PromiseOfAssetCollectionResult>("Promise<AssetCollectionResult>");
    emscripten::register_type<PromiseOfAssetCollectionsResult>("Promise<AssetCollectionsResult>");
    emscripten::register_type<PromiseOfAssetCollectionCountResult>("Promise<AssetCollectionCountResult>");
    emscripten::register_type<PromiseOfLODChainResult>("Promise<LODChainResult>");
    emscripten::register_type<PromiseOfMaterialResult>("Promise<MaterialResult>");
    emscripten::register_type<PromiseOfMaterialsResult>("Promise<MaterialsResult>");
    emscripten::register_type<PromiseOfSpaceResult>("Promise<SpaceResult>");
    emscripten::register_type<PromiseOfSpacesResult>("Promise<SpacesResult>");
    emscripten::register_type<PromiseOfBasicSpaceResult>("Promise<BasicSpaceResult>");
    emscripten::register_type<PromiseOfBasicSpacesResult>("Promise<BasicSpacesResult>");
    emscripten::register_type<PromiseOfSpaceMetadataResult>("Promise<SpaceMetadataResult>");
    emscripten::register_type<PromiseOfSpacesMetadataResult>("Promise<SpacesMetadataResult>");
    emscripten::register_type<PromiseOfPendingInvitesResult>("Promise<PendingInvitesResult>");
    emscripten::register_type<PromiseOfAcceptedInvitesResult>("Promise<AcceptedInvitesResult>");
    emscripten::register_type<PromiseOfSpaceGeoLocationResult>("Promise<SpaceGeoLocationResult>");
    emscripten::register_type<PromiseOfSiteResult>("Promise<SiteResult>");
    emscripten::register_type<PromiseOfSitesCollectionResult>("Promise<SitesCollectionResult>");
    emscripten::register_type<PromiseOfUserRoleCollectionResult>("Promise<UserRoleCollectionResult>");
    emscripten::register_type<PromiseOfProfileResult>("Promise<ProfileResult>");
    emscripten::register_type<PromiseOfBasicProfilesResult>("Promise<BasicProfilesResult>");
    emscripten::register_type<PromiseOfLoginStateResult>("Promise<LoginStateResult>");
    emscripten::register_type<PromiseOfFeaturesLimitResult>("Promise<FeaturesLimitResult>");
    emscripten::register_type<PromiseOfFeatureLimitResult>("Promise<FeatureLimitResult>");
    emscripten::register_type<PromiseOfUserTierResult>("Promise<UserTierResult>");
    emscripten::register_type<PromiseOfFeatureQuotaResult>("Promise<FeatureQuotaResult>");
    emscripten::register_type<PromiseOfFeaturesQuotaResult>("Promise<FeaturesQuotaResult>");
    emscripten::register_type<PromiseOfSequenceResult>("Promise<SequenceResult>");
    emscripten::register_type<PromiseOfSequencesResult>("Promise<SequencesResult>");
    emscripten::register_type<PromiseOfHotspotGroupResult>("Promise<HotspotGroupResult>");
    emscripten::register_type<PromiseOfHotspotGroupsResult>("Promise<HotspotGroupsResult>");
    emscripten::register_type<PromiseOfTicketedEventResult>("Promise<TicketedEventResult>");
    emscripten::register_type<PromiseOfTicketedEventCollectionResult>("Promise<TicketedEventCollectionResult>");
    emscripten::register_type<PromiseOfEventTicketResult>("Promise<EventTicketResult>");
    emscripten::register_type<PromiseOfSpaceIsTicketedResult>("Promise<SpaceIsTicketedResult>");
    emscripten::register_type<PromiseOfTicketedEventVendorAuthInfoResult>("Promise<TicketedEventVendorAuthInfoResult>");
    emscripten::register_type<PromiseOfGraphQLResult>("Promise<GraphQLResult>");
    emscripten::register_type<PromiseOfMaintenanceInfoResult>("Promise<MaintenanceInfoResult>");
    emscripten::register_type<PromiseOfProductInfoResult>("Promise<ProductInfoResult>");
    emscripten::register_type<PromiseOfProductInfoCollectionResult>("Promise<ProductInfoCollectionResult>");
    emscripten::register_type<PromiseOfCheckoutInfoResult>("Promise<CheckoutInfoResult>");
    emscripten::register_type<PromiseOfCartInfoResult>("Promise<CartInfoResult>");
    emscripten::register_type<PromiseOfAddShopifyStoreResult>("Promise<AddShopifyStoreResult>");
    emscripten::register_type<PromiseOfValidateShopifyStoreResult>("Promise<ValidateShopifyStoreResult>");
    emscripten::register_type<PromiseOfGetShopifyStoresResult>("Promise<GetShopifyStoresResult>");
    emscripten::register_type<PromiseOfApplicationSettingsResult>("Promise<ApplicationSettingsResult>");
    emscripten::register_type<PromiseOfAvatarInfoResult>("Promise<AvatarInfoResult>");
    emscripten::register_type<PromiseOfAnchorResult>("Promise<AnchorResult>");
    emscripten::register_type<PromiseOfAnchorCollectionResult>("Promise<AnchorCollectionResult>");
    emscripten::register_type<PromiseOfAnchorResolutionResult>("Promise<AnchorResolutionResult>");
    emscripten::register_type<PromiseOfPOIResult>("Promise<POIResult>");
    emscripten::register_type<PromiseOfPOICollectionResult>("Promise<POICollectionResult>");
}
