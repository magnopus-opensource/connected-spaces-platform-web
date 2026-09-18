#include "../../../async/Promises.h"
#include "../../../containers/Array.h"
#include "../../../containers/Map.h"
#include "../../../containers/Optional.h"
#include "../../../containers/String.h"
#include "../../CallbackDeclarations.h"
#include "../../PromiseDeclarations.h"

#include "CSP/Common/Array.h"
#include "CSP/Common/Interfaces/IRealtimeEngine.h"
#include "CSP/Common/Map.h"
#include "CSP/Common/Optional.h"
#include "CSP/Common/String.h"
#include "CSP/Systems/Assets/Asset.h"
#include "CSP/Systems/Spaces/Site.h"
#include "CSP/Systems/Spaces/Space.h"
#include "CSP/Systems/Spaces/SpaceSystem.h"
#include "CSP/Systems/Spaces/UserRoles.h"
#include "CSP/Systems/Spatial/SpatialDataTypes.h"
#include "CSP/Systems/SystemBase.h"

#include "emscripten/bind.h"
#include "emscripten/val.h"

namespace emscripten::internal {
template <> void raw_destructor<csp::systems::SpaceSystem>(csp::systems::SpaceSystem*) { }
}

EMSCRIPTEN_BINDINGS(CSPSpaceSystem)
{
    emscripten::class_<csp::systems::SpaceSystem, emscripten::base<csp::systems::SystemBase>>("SpaceSystem")
        .function(
            "enterSpace(spaceId, realtimeEngine)",
            +[](csp::systems::SpaceSystem& self, const csp::common::String& spaceId, csp::common::IRealtimeEngine* realtimeEngine) {
                return Promisify<PromiseOfSpaceResult>([&](emscripten::val cb) { self.EnterSpace(spaceId, realtimeEngine, ToNativeCallback(cb.as<SpaceResultCallback>())); });
            },
            emscripten::allow_raw_pointers())
        .function(
            "exitSpace",
            +[](csp::systems::SpaceSystem& self) {
                return Promisify<PromiseOfNullResult>([&](emscripten::val cb) { self.ExitSpace(ToNativeCallback(cb.as<NullResultCallback>())); });
            })
        .function(
            "isInSpace", +[](csp::systems::SpaceSystem& self) { return self.IsInSpace(); })
        .function(
            "getCurrentSpace", +[](const csp::systems::SpaceSystem& self) { return self.GetCurrentSpace(); })
        .function(
            "createSpace(name, description, attributes, inviteUsers, metadata, fileThumbnail, tags)",
            +[](csp::systems::SpaceSystem& self, const csp::common::String& name, const csp::common::String& description, csp::systems::SpaceAttributes attributes,
                 const csp::common::Optional<csp::systems::InviteUserRoleInfoCollection>& inviteUsers, const csp::common::Map<csp::common::String, csp::common::String>& metadata,
                 const csp::common::Optional<csp::systems::FileAssetDataSource>& fileThumbnail, const csp::common::Optional<csp::common::Array<csp::common::String>>& tags) {
                return Promisify<PromiseOfSpaceResult>([&](emscripten::val cb) {
                    self.CreateSpace(name, description, attributes, inviteUsers, metadata, fileThumbnail, tags, ToNativeCallback(cb.as<SpaceResultCallback>()));
                });
            })
        .function(
            "createSpaceWithBuffer(name, description, attributes, inviteUsers, metadata, thumbnail, tags)",
            +[](csp::systems::SpaceSystem& self, const csp::common::String& name, const csp::common::String& description, csp::systems::SpaceAttributes attributes,
                 const csp::common::Optional<csp::systems::InviteUserRoleInfoCollection>& inviteUsers, const csp::common::Map<csp::common::String, csp::common::String>& metadata,
                 const csp::systems::BufferAssetDataSource& thumbnail, const csp::common::Optional<csp::common::Array<csp::common::String>>& tags) {
                return Promisify<PromiseOfSpaceResult>([&](emscripten::val cb) {
                    self.CreateSpaceWithBuffer(name, description, attributes, inviteUsers, metadata, thumbnail, tags, ToNativeCallback(cb.as<SpaceResultCallback>()));
                });
            })
        .function(
            "updateSpace(spaceId, name, description, type, tags)",
            +[](csp::systems::SpaceSystem& self, const csp::common::String& spaceId, const csp::common::Optional<csp::common::String>& name,
                 const csp::common::Optional<csp::common::String>& description, const csp::common::Optional<csp::systems::SpaceAttributes>& type,
                 const csp::common::Optional<csp::common::Array<csp::common::String>>& tags) {
                return Promisify<PromiseOfBasicSpaceResult>(
                    [&](emscripten::val cb) { self.UpdateSpace(spaceId, name, description, type, tags, ToNativeCallback(cb.as<BasicSpaceResultCallback>())); });
            })
        .function(
            "deleteSpace(spaceId)",
            +[](csp::systems::SpaceSystem& self, const csp::common::String& spaceId) {
                return Promisify<PromiseOfNullResult>([&](emscripten::val cb) { self.DeleteSpace(spaceId, ToNativeCallback(cb.as<NullResultCallback>())); });
            })
        .function(
            "getSpaces",
            +[](csp::systems::SpaceSystem& self) {
                return Promisify<PromiseOfSpacesResult>([&](emscripten::val cb) { self.GetSpaces(ToNativeCallback(cb.as<SpacesResultCallback>())); });
            })
        .function(
            "getSpacesByAttributes(isDiscoverable, isArchived, requiresInvite, resultsSkip, resultsMax, mustContainTags, mustExcludeTags, mustIncludeAllTags)",
            +[](csp::systems::SpaceSystem& self, const csp::common::Optional<bool>& isDiscoverable, const csp::common::Optional<bool>& isArchived,
                 const csp::common::Optional<bool>& requiresInvite, const csp::common::Optional<int>& resultsSkip, const csp::common::Optional<int>& resultsMax,
                 const csp::common::Optional<csp::common::Array<csp::common::String>>& mustContainTags,
                 const csp::common::Optional<csp::common::Array<csp::common::String>>& mustExcludeTags, const csp::common::Optional<bool>& mustIncludeAllTags) {
                return Promisify<PromiseOfBasicSpacesResult>([&](emscripten::val cb) {
                    self.GetSpacesByAttributes(isDiscoverable, isArchived, requiresInvite, resultsSkip, resultsMax, mustContainTags, mustExcludeTags, mustIncludeAllTags,
                        ToNativeCallback(cb.as<BasicSpacesResultCallback>()));
                });
            })
        .function(
            "getSpacesByIds(requestedSpaceIds)",
            +[](csp::systems::SpaceSystem& self, const csp::common::Array<csp::common::String>& requestedSpaceIds) {
                return Promisify<PromiseOfSpacesResult>([&](emscripten::val cb) { self.GetSpacesByIds(requestedSpaceIds, ToNativeCallback(cb.as<SpacesResultCallback>())); });
            })
        .function(
            "getSpacesForUserId(userId)",
            +[](csp::systems::SpaceSystem& self, const csp::common::String& userId) {
                return Promisify<PromiseOfSpacesResult>([&](emscripten::val cb) { self.GetSpacesForUserId(userId, ToNativeCallback(cb.as<SpacesResultCallback>())); });
            })
        .function(
            "getSpace(spaceId)",
            +[](csp::systems::SpaceSystem& self, const csp::common::String& spaceId) {
                return Promisify<PromiseOfSpaceResult>([&](emscripten::val cb) { self.GetSpace(spaceId, ToNativeCallback(cb.as<SpaceResultCallback>())); });
            })
        .function(
            "inviteToSpace(spaceId, email, isModeratorRole, emailLinkUrl, signupUrl)",
            +[](csp::systems::SpaceSystem& self, const csp::common::String& spaceId, const csp::common::String& email, const csp::common::Optional<bool>& isModeratorRole,
                 const csp::common::Optional<csp::common::String>& emailLinkUrl, const csp::common::Optional<csp::common::String>& signupUrl) {
                return Promisify<PromiseOfNullResult>(
                    [&](emscripten::val cb) { self.InviteToSpace(spaceId, email, isModeratorRole, emailLinkUrl, signupUrl, ToNativeCallback(cb.as<NullResultCallback>())); });
            })
        .function(
            "bulkInviteToSpace(spaceId, inviteUsers)",
            +[](csp::systems::SpaceSystem& self, const csp::common::String& spaceId, const csp::systems::InviteUserRoleInfoCollection& inviteUsers) {
                return Promisify<PromiseOfNullResult>([&](emscripten::val cb) { self.BulkInviteToSpace(spaceId, inviteUsers, ToNativeCallback(cb.as<NullResultCallback>())); });
            })
        .function(
            "getPendingUserInvites(spaceId)",
            +[](csp::systems::SpaceSystem& self, const csp::common::String& spaceId) {
                return Promisify<PromiseOfPendingInvitesResult>(
                    [&](emscripten::val cb) { self.GetPendingUserInvites(spaceId, ToNativeCallback(cb.as<PendingInvitesResultCallback>())); });
            })
        .function(
            "getAcceptedUserInvites(spaceId)",
            +[](csp::systems::SpaceSystem& self, const csp::common::String& spaceId) {
                return Promisify<PromiseOfAcceptedInvitesResult>(
                    [&](emscripten::val cb) { self.GetAcceptedUserInvites(spaceId, ToNativeCallback(cb.as<AcceptedInvitesResultCallback>())); });
            })
        .function(
            "removeUserFromSpace(spaceId, userId)",
            +[](csp::systems::SpaceSystem& self, const csp::common::String& spaceId, const csp::common::String& userId) {
                return Promisify<PromiseOfNullResult>([&](emscripten::val cb) { self.RemoveUserFromSpace(spaceId, userId, ToNativeCallback(cb.as<NullResultCallback>())); });
            })
        .function(
            "addUserToSpace(spaceId, userId)",
            +[](csp::systems::SpaceSystem& self, const csp::common::String& spaceId, const csp::common::String& userId) {
                return Promisify<PromiseOfSpaceResult>([&](emscripten::val cb) { self.AddUserToSpace(spaceId, userId, ToNativeCallback(cb.as<SpaceResultCallback>())); });
            })
        .function(
            "addSiteInfo(siteInfo)",
            +[](csp::systems::SpaceSystem& self, const csp::systems::Site& siteInfo) {
                return Promisify<PromiseOfSiteResult>([&](emscripten::val cb) { self.AddSiteInfo(siteInfo, ToNativeCallback(cb.as<SiteResultCallback>())); });
            })
        .function(
            "removeSiteInfo(siteInfo)",
            +[](csp::systems::SpaceSystem& self, const csp::systems::Site& siteInfo) {
                return Promisify<PromiseOfNullResult>([&](emscripten::val cb) { self.RemoveSiteInfo(siteInfo, ToNativeCallback(cb.as<NullResultCallback>())); });
            })
        .function(
            "getSitesInfo(spaceId)",
            +[](csp::systems::SpaceSystem& self, const csp::common::String& spaceId) {
                return Promisify<PromiseOfSitesCollectionResult>([&](emscripten::val cb) { self.GetSitesInfo(spaceId, ToNativeCallback(cb.as<SitesCollectionResultCallback>())); });
            })
        .function(
            "updateUserRole(spaceId, newUserRoleInfo)",
            +[](csp::systems::SpaceSystem& self, const csp::common::String& spaceId, const csp::systems::UserRoleInfo& newUserRoleInfo) {
                return Promisify<PromiseOfNullResult>([&](emscripten::val cb) { self.UpdateUserRole(spaceId, newUserRoleInfo, ToNativeCallback(cb.as<NullResultCallback>())); });
            })
        .function(
            "getUsersRoles(spaceId, requestedUserIds)",
            +[](csp::systems::SpaceSystem& self, const csp::common::String& spaceId, const csp::common::Array<csp::common::String>& requestedUserIds) {
                return Promisify<PromiseOfUserRoleCollectionResult>(
                    [&](emscripten::val cb) { self.GetUsersRoles(spaceId, requestedUserIds, ToNativeCallback(cb.as<UserRoleCollectionCallback>())); });
            })
        .function(
            "updateSpaceMetadata(spaceId, newMetadata)",
            +[](csp::systems::SpaceSystem& self, const csp::common::String& spaceId, const csp::common::Map<csp::common::String, csp::common::String>& newMetadata) {
                return Promisify<PromiseOfNullResult>([&](emscripten::val cb) { self.UpdateSpaceMetadata(spaceId, newMetadata, ToNativeCallback(cb.as<NullResultCallback>())); });
            })
        .function(
            "getSpacesMetadata(spaces)",
            +[](csp::systems::SpaceSystem& self, const csp::common::Array<csp::common::String>& spaces) {
                return Promisify<PromiseOfSpacesMetadataResult>(
                    [&](emscripten::val cb) { self.GetSpacesMetadata(spaces, ToNativeCallback(cb.as<SpacesMetadataResultCallback>())); });
            })
        .function(
            "getSpaceMetadata(spaceId)",
            +[](csp::systems::SpaceSystem& self, const csp::common::String& spaceId) {
                return Promisify<PromiseOfSpaceMetadataResult>([&](emscripten::val cb) { self.GetSpaceMetadata(spaceId, ToNativeCallback(cb.as<SpaceMetadataResultCallback>())); });
            })
        .function(
            "updateSpaceThumbnail(spaceId, newThumbnail)",
            +[](csp::systems::SpaceSystem& self, const csp::common::String& spaceId, const csp::systems::FileAssetDataSource& newThumbnail) {
                return Promisify<PromiseOfNullResult>([&](emscripten::val cb) { self.UpdateSpaceThumbnail(spaceId, newThumbnail, ToNativeCallback(cb.as<NullResultCallback>())); });
            })
        .function(
            "updateSpaceThumbnailWithBuffer(spaceId, newThumbnail)",
            +[](csp::systems::SpaceSystem& self, const csp::common::String& spaceId, const csp::systems::BufferAssetDataSource& newThumbnail) {
                return Promisify<PromiseOfNullResult>(
                    [&](emscripten::val cb) { self.UpdateSpaceThumbnailWithBuffer(spaceId, newThumbnail, ToNativeCallback(cb.as<NullResultCallback>())); });
            })
        .function(
            "getSpaceThumbnail(spaceId)",
            +[](csp::systems::SpaceSystem& self, const csp::common::String& spaceId) {
                return Promisify<PromiseOfUriResult>([&](emscripten::val cb) { self.GetSpaceThumbnail(spaceId, ToNativeCallback(cb.as<UriResultCallback>())); });
            })
        .function(
            "addUserToSpaceBanList(spaceId, requestedUserId)",
            +[](csp::systems::SpaceSystem& self, const csp::common::String& spaceId, const csp::common::String& requestedUserId) {
                return Promisify<PromiseOfNullResult>(
                    [&](emscripten::val cb) { self.AddUserToSpaceBanList(spaceId, requestedUserId, ToNativeCallback(cb.as<NullResultCallback>())); });
            })
        .function(
            "deleteUserFromSpaceBanList(spaceId, requestedUserId)",
            +[](csp::systems::SpaceSystem& self, const csp::common::String& spaceId, const csp::common::String& requestedUserId) {
                return Promisify<PromiseOfNullResult>(
                    [&](emscripten::val cb) { self.DeleteUserFromSpaceBanList(spaceId, requestedUserId, ToNativeCallback(cb.as<NullResultCallback>())); });
            })
        .function(
            "updateSpaceGeoLocation(spaceId, location, orientation, geoFence)",
            +[](csp::systems::SpaceSystem& self, const csp::common::String& spaceId, const csp::common::Optional<csp::systems::GeoLocation>& location,
                 const csp::common::Optional<float>& orientation, const csp::common::Optional<csp::common::Array<csp::systems::GeoLocation>>& geoFence) {
                return Promisify<PromiseOfSpaceGeoLocationResult>(
                    [&](emscripten::val cb) { self.UpdateSpaceGeoLocation(spaceId, location, orientation, geoFence, ToNativeCallback(cb.as<SpaceGeoLocationResultCallback>())); });
            })
        .function(
            "getSpaceGeoLocation(spaceId)",
            +[](csp::systems::SpaceSystem& self, const csp::common::String& spaceId) {
                return Promisify<PromiseOfSpaceGeoLocationResult>(
                    [&](emscripten::val cb) { self.GetSpaceGeoLocation(spaceId, ToNativeCallback(cb.as<SpaceGeoLocationResultCallback>())); });
            })
        .function(
            "deleteSpaceGeoLocation(spaceId)",
            +[](csp::systems::SpaceSystem& self, const csp::common::String& spaceId) {
                return Promisify<PromiseOfNullResult>([&](emscripten::val cb) { self.DeleteSpaceGeoLocation(spaceId, ToNativeCallback(cb.as<NullResultCallback>())); });
            })
        .function(
            "duplicateSpace(spaceId, newName, newAttributes, memberGroupIds, shallowCopy)",
            +[](csp::systems::SpaceSystem& self, const csp::common::String& spaceId, const csp::common::String& newName, csp::systems::SpaceAttributes newAttributes,
                 const csp::common::Optional<csp::common::Array<csp::common::String>>& memberGroupIds, bool shallowCopy) {
                return Promisify<PromiseOfSpaceResult>(
                    [&](emscripten::val cb) { self.DuplicateSpace(spaceId, newName, newAttributes, memberGroupIds, shallowCopy, ToNativeCallback(cb.as<SpaceResultCallback>())); });
            })
        .function(
            "duplicateSpaceAsync(spaceId, newName, newAttributes, memberGroupIds, shallowCopy)",
            +[](csp::systems::SpaceSystem& self, const csp::common::String& spaceId, const csp::common::String& newName, csp::systems::SpaceAttributes newAttributes,
                 const csp::common::Optional<csp::common::Array<csp::common::String>>& memberGroupIds, bool shallowCopy) {
                return Promisify<PromiseOfNullResult>([&](emscripten::val cb) {
                    self.DuplicateSpaceAsync(spaceId, newName, newAttributes, memberGroupIds, shallowCopy, ToNativeCallback(cb.as<NullResultCallback>()));
                });
            });
}
