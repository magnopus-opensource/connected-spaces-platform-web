
#include "../containers/Array.h"
#include "../containers/List.h"
#include "../containers/Map.h"
#include "../containers/Optional.h"
#include "../containers/String.h"
#include "../utils/JSDisposable.h"

#include "CSP/CSPFoundation.h"
#include "CSP/Common/Array.h"
#include "CSP/Common/List.h"
#include "CSP/Common/Map.h"
#include "CSP/Common/ReplicatedValue.h"
#include "CSP/Common/Settings.h"
#include "CSP/Common/SharedEnums.h"
#include "CSP/Common/String.h"
#include "CSP/Common/Vector.h"
#include "CSP/Multiplayer/ComponentBase.h"
#include "CSP/Multiplayer/Conversation/Conversation.h"
#include "CSP/Multiplayer/NetworkEventBus.h"
#include "CSP/Multiplayer/PatchTypes.h"
#include "CSP/Multiplayer/SpaceEntity.h"
#include "CSP/Systems/Assets/Asset.h"
#include "CSP/Systems/Assets/AssetCollection.h"
#include "CSP/Systems/Assets/LOD.h"
#include "CSP/Systems/ECommerce/ECommerce.h"
#include "CSP/Systems/EventTicketing/EventTicketing.h"
#include "CSP/Systems/HotspotSequence/HotspotGroup.h"
#include "CSP/Systems/Maintenance/Maintenance.h"
#include "CSP/Systems/Multiplayer/Scope.h"
#include "CSP/Systems/Quota/Quota.h"
#include "CSP/Systems/Sequence/Sequence.h"
#include "CSP/Systems/ServiceStatus.h"
#include "CSP/Systems/Spaces/Site.h"
#include "CSP/Systems/Spaces/Space.h"
#include "CSP/Systems/Spaces/UserRoles.h"
#include "CSP/Systems/Spatial/Anchor.h"
#include "CSP/Systems/Spatial/PointOfInterest.h"
#include "CSP/Systems/Spatial/SpatialDataTypes.h"
#include "CSP/Systems/Users/Authentication.h"
#include "CSP/Systems/Users/Profile.h"
#include "CSP/Systems/Users/ThirdPartyAuthentication.h"

#include "emscripten/bind.h"
#include <cstdint>

/*
 * Registrations for custom container types used in the bindings.
 * No paired header for this one, as there's no val types we need to declare, containers are fully mapped objects (See bindings/containers)
 *
 * Be aware that you need to declare a JSDisposable version of any container that is used in a return path. See the container mapping
 * headers themselves for more, or reference Interop-Types.md
 */

EMSCRIPTEN_BINDINGS(CSPContainerDeclarations)
{

    /* CspFoundation */

    // Array
    emscripten::register_type<csp::common::Array<csp::FeatureFlag>>("FeatureFlag[]");
    emscripten::register_type<bindings::utils::JSDisposable<csp::common::Array<csp::FeatureFlag>>>("(FeatureFlag[] & Disposable)");

    // Optional
    emscripten::register_optional<csp::common::Array<csp::FeatureFlag>>();

    /* Common */

    // List
    emscripten::register_type<csp::common::List<csp::common::ApplicationSettings>>("ApplicationSettings[]");
    emscripten::register_type<bindings::utils::JSDisposable<csp::common::List<csp::common::ApplicationSettings>>>("(ApplicationSettings[] & Disposable)");
    emscripten::register_type<csp::common::List<csp::common::SettingsCollection>>("SettingsCollection[]");
    emscripten::register_type<bindings::utils::JSDisposable<csp::common::List<csp::common::SettingsCollection>>>("(SettingsCollection[] & Disposable)");
    emscripten::register_type<csp::common::List<csp::common::String>>("string[]");
    emscripten::register_type<csp::common::List<csp::common::Vector3>>("Vector3[]");

    // Array
    emscripten::register_type<csp::common::Array<csp::common::String>>("string[]");
    emscripten::register_type<csp::common::Array<csp::common::ReplicatedValue>>("ReplicatedValue[]");
    emscripten::register_type<bindings::utils::JSDisposable<csp::common::Array<csp::common::ReplicatedValue>>>("(ReplicatedValue[] & Disposable)");

    // Map
    emscripten::register_type<csp::common::Map<csp::common::String, csp::common::String>>("Map<string, string>");
    emscripten::register_type<csp::common::Map<csp::common::String, csp::common::ReplicatedValue>>("Map<string, ReplicatedValue>");
    emscripten::register_type<bindings::utils::JSDisposable<csp::common::Map<csp::common::String, csp::common::ReplicatedValue>>>("(Map<string, ReplicatedValue> & Disposable)");
    emscripten::register_type<csp::common::Map<csp::common::String, csp::common::Map<csp::common::String, csp::common::String>>>("Map<string, Map<string, string>>");
    emscripten::register_type<csp::common::Map<csp::common::String, csp::common::Array<csp::common::String>>>("Map<string, string[]>");

    // Optional
    emscripten::register_optional<bool>();
    emscripten::register_optional<int>();
    emscripten::register_optional<float>();
    emscripten::register_optional<uint64_t>();
    emscripten::register_optional<csp::common::String>();
    emscripten::register_optional<csp::common::Array<csp::common::String>>();
    emscripten::register_optional<csp::common::Map<csp::common::String, csp::common::String>>();

    /* Multiplayer*/

    // List
    emscripten::register_type<csp::common::List<csp::multiplayer::SpaceEntity*>>("(SpaceEntity | null)[]");
    // Array
    emscripten::register_type<csp::common::Array<csp::multiplayer::ComponentUpdateInfo>>("ComponentUpdateInfo[]");
    emscripten::register_type<csp::common::Array<csp::multiplayer::MessageInfo>>("MessageInfo[]");
    emscripten::register_type<bindings::utils::JSDisposable<csp::common::Array<csp::multiplayer::MessageInfo>>>("(MessageInfo[] & Disposable)");
    emscripten::register_type<csp::common::Array<csp::multiplayer::NetworkEventRegistration>>("NetworkEventRegistration[]");
    emscripten::register_type<bindings::utils::JSDisposable<csp::common::Array<csp::multiplayer::NetworkEventRegistration>>>("(NetworkEventRegistration[] & Disposable)");
    // Map
    emscripten::register_type<csp::common::Map<uint32_t, csp::common::ReplicatedValue>>("Map<number, ReplicatedValue>");
    emscripten::register_type<bindings::utils::JSDisposable<csp::common::Map<uint32_t, csp::common::ReplicatedValue>>>("(Map<number, ReplicatedValue> & Disposable)");
    emscripten::register_type<csp::common::Map<uint16_t, csp::multiplayer::ComponentBase*>>("Map<number, (ComponentBase | null)>");
    // Optional

    /* Systems */

    // List
    // Array
    emscripten::register_type<csp::common::Array<csp::systems::SpaceUserRole>>("SpaceUserRole[]");
    emscripten::register_type<csp::common::Array<csp::systems::EAssetPlatform>>("EAssetPlatform[]");
    emscripten::register_type<csp::common::Array<csp::systems::EAssetType>>("EAssetType[]");
    emscripten::register_type<csp::common::Array<csp::systems::EAssetCollectionType>>("EAssetCollectionType[]");
    emscripten::register_type<csp::common::Array<csp::systems::EThirdPartyAuthenticationProviders>>("EThirdPartyAuthenticationProviders[]");
    emscripten::register_type<csp::common::Array<csp::systems::TierFeatures>>("TierFeatures[]");

    emscripten::register_type<csp::common::Array<csp::systems::Asset>>("Asset[]");
    emscripten::register_type<bindings::utils::JSDisposable<csp::common::Array<csp::systems::Asset>>>("(Asset[] & Disposable)");
    emscripten::register_type<csp::common::Array<csp::systems::AssetCollection>>("AssetCollection[]");
    emscripten::register_type<bindings::utils::JSDisposable<csp::common::Array<csp::systems::AssetCollection>>>("(AssetCollection[] & Disposable)");
    emscripten::register_type<csp::common::Array<csp::systems::LODAsset>>("LODAsset[]");
    emscripten::register_type<bindings::utils::JSDisposable<csp::common::Array<csp::systems::LODAsset>>>("(LODAsset[] & Disposable)");
    emscripten::register_type<csp::common::Array<csp::systems::GeoLocation>>("GeoLocation[]");
    emscripten::register_type<bindings::utils::JSDisposable<csp::common::Array<csp::systems::GeoLocation>>>("(GeoLocation[] & Disposable)");
    emscripten::register_type<csp::common::Array<csp::systems::Space>>("Space[]");
    emscripten::register_type<bindings::utils::JSDisposable<csp::common::Array<csp::systems::Space>>>("(Space[] & Disposable)");
    emscripten::register_type<csp::common::Array<csp::systems::BasicSpace>>("BasicSpace[]");
    emscripten::register_type<bindings::utils::JSDisposable<csp::common::Array<csp::systems::BasicSpace>>>("(BasicSpace[] & Disposable)");
    emscripten::register_type<csp::common::Array<csp::systems::Site>>("Site[]");
    emscripten::register_type<bindings::utils::JSDisposable<csp::common::Array<csp::systems::Site>>>("(Site[] & Disposable)");
    emscripten::register_type<csp::common::Array<csp::systems::UserRoleInfo>>("UserRoleInfo[]");
    emscripten::register_type<bindings::utils::JSDisposable<csp::common::Array<csp::systems::UserRoleInfo>>>("(UserRoleInfo[] & Disposable)");
    emscripten::register_type<csp::common::Array<csp::systems::InviteUserRoleInfo>>("InviteUserRoleInfo[]");
    emscripten::register_type<bindings::utils::JSDisposable<csp::common::Array<csp::systems::InviteUserRoleInfo>>>("(InviteUserRoleInfo[] & Disposable)");
    emscripten::register_type<csp::common::Array<csp::systems::BasicProfile>>("BasicProfile[]");
    emscripten::register_type<csp::common::Array<csp::systems::FeatureLimitInfo>>("FeatureLimitInfo[]");
    emscripten::register_type<bindings::utils::JSDisposable<csp::common::Array<csp::systems::FeatureLimitInfo>>>("(FeatureLimitInfo[] & Disposable)");
    emscripten::register_type<csp::common::Array<csp::systems::FeatureQuotaInfo>>("FeatureQuotaInfo[]");
    emscripten::register_type<bindings::utils::JSDisposable<csp::common::Array<csp::systems::FeatureQuotaInfo>>>("(FeatureQuotaInfo[] & Disposable)");
    emscripten::register_type<csp::common::Array<csp::systems::Sequence>>("Sequence[]");
    emscripten::register_type<bindings::utils::JSDisposable<csp::common::Array<csp::systems::Sequence>>>("(Sequence[] & Disposable)");
    emscripten::register_type<csp::common::Array<csp::systems::HotspotGroup>>("HotspotGroup[]");
    emscripten::register_type<bindings::utils::JSDisposable<csp::common::Array<csp::systems::HotspotGroup>>>("(HotspotGroup[] & Disposable)");
    emscripten::register_type<csp::common::Array<csp::systems::TicketedEvent>>("TicketedEvent[]");
    emscripten::register_type<bindings::utils::JSDisposable<csp::common::Array<csp::systems::TicketedEvent>>>("(TicketedEvent[] & Disposable)");
    emscripten::register_type<csp::common::Array<csp::systems::MaintenanceInfo>>("MaintenanceInfo[]");
    emscripten::register_type<bindings::utils::JSDisposable<csp::common::Array<csp::systems::MaintenanceInfo>>>("(MaintenanceInfo[] & Disposable)");
    emscripten::register_type<csp::common::Array<csp::systems::VariantOptionInfo>>("VariantOptionInfo[]");
    emscripten::register_type<bindings::utils::JSDisposable<csp::common::Array<csp::systems::VariantOptionInfo>>>("(VariantOptionInfo[] & Disposable)");
    emscripten::register_type<csp::common::Array<csp::systems::ProductMediaInfo>>("ProductMediaInfo[]");
    emscripten::register_type<bindings::utils::JSDisposable<csp::common::Array<csp::systems::ProductMediaInfo>>>("(ProductMediaInfo[] & Disposable)");
    emscripten::register_type<csp::common::Array<csp::systems::ProductVariantInfo>>("ProductVariantInfo[]");
    emscripten::register_type<bindings::utils::JSDisposable<csp::common::Array<csp::systems::ProductVariantInfo>>>("(ProductVariantInfo[] & Disposable)");
    emscripten::register_type<csp::common::Array<csp::systems::ProductInfo>>("ProductInfo[]");
    emscripten::register_type<bindings::utils::JSDisposable<csp::common::Array<csp::systems::ProductInfo>>>("(ProductInfo[] & Disposable)");
    emscripten::register_type<csp::common::Array<csp::systems::CartLine>>("CartLine[]");
    emscripten::register_type<bindings::utils::JSDisposable<csp::common::Array<csp::systems::CartLine>>>("(CartLine[] & Disposable)");
    emscripten::register_type<csp::common::Array<csp::systems::ShopifyStoreInfo>>("ShopifyStoreInfo[]");
    emscripten::register_type<bindings::utils::JSDisposable<csp::common::Array<csp::systems::ShopifyStoreInfo>>>("(ShopifyStoreInfo[] & Disposable)");
    emscripten::register_type<csp::common::Array<csp::systems::Anchor>>("Anchor[]");
    emscripten::register_type<bindings::utils::JSDisposable<csp::common::Array<csp::systems::Anchor>>>("(Anchor[] & Disposable)");
    emscripten::register_type<csp::common::Array<csp::systems::AnchorResolution>>("AnchorResolution[]");
    emscripten::register_type<bindings::utils::JSDisposable<csp::common::Array<csp::systems::AnchorResolution>>>("(AnchorResolution[] & Disposable)");
    emscripten::register_type<csp::common::Array<csp::systems::PointOfInterest>>("PointOfInterest[]");
    emscripten::register_type<bindings::utils::JSDisposable<csp::common::Array<csp::systems::PointOfInterest>>>("(PointOfInterest[] & Disposable)");
    emscripten::register_type<csp::common::Array<csp::systems::Scope>>("Scope[]");
    emscripten::register_type<bindings::utils::JSDisposable<csp::common::Array<csp::systems::Scope>>>("(Scope[] & Disposable)");
    emscripten::register_type<csp::common::Array<csp::systems::VersionMetadata>>("VersionMetadata[]");
    emscripten::register_type<bindings::utils::JSDisposable<csp::common::Array<csp::systems::VersionMetadata>>>("(VersionMetadata[] & Disposable)");
    emscripten::register_type<csp::common::Array<csp::systems::ServiceStatus>>("ServiceStatus[]");
    emscripten::register_type<bindings::utils::JSDisposable<csp::common::Array<csp::systems::ServiceStatus>>>("(ServiceStatus[] & Disposable)");

    // Map
    emscripten::register_type<csp::common::Map<csp::common::String, csp::systems::Asset>>("Map<string, Asset>");
    emscripten::register_type<bindings::utils::JSDisposable<csp::common::Map<csp::common::String, csp::systems::Asset>>>("(Map<string, Asset> & Disposable)");

    // Optional
    emscripten::register_optional<csp::common::Array<csp::systems::EAssetType>>();
    emscripten::register_optional<csp::common::Array<csp::systems::EAssetCollectionType>>();
    emscripten::register_optional<csp::common::Array<csp::systems::GeoLocation>>();
    emscripten::register_optional<csp::systems::EThirdPartyPlatform>();
    emscripten::register_optional<csp::systems::SpaceAttributes>();
    emscripten::register_optional<csp::systems::EPointOfInterestType>();
    emscripten::register_optional<csp::systems::EventTicketingVendor>();
    emscripten::register_optional<csp::systems::GeoLocation>();
    emscripten::register_optional<csp::systems::FileAssetDataSource>();
    emscripten::register_optional<csp::systems::TokenOptions>();
    emscripten::register_optional<csp::systems::InviteUserRoleInfoCollection>();
}
