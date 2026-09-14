#include "../../../containers/Array.h"
#include "../../../containers/Map.h"
#include "../../../containers/String.h"
#include "../../../utils/JSDisposable.h"

#include "CSP/Common/Array.h"
#include "CSP/Common/Map.h"
#include "CSP/Common/String.h"
#include "CSP/Systems/Spaces/Space.h"
#include "CSP/Systems/Spatial/SpatialDataTypes.h"
#include "CSP/Systems/WebService.h"

#include "emscripten/bind.h"
#include <utility>

EMSCRIPTEN_BINDINGS(CSPSpace)
{
    emscripten::enum_<csp::systems::SpaceAttributes>("SpaceAttributes", emscripten::enum_value_type::number)
        .value("None", csp::systems::SpaceAttributes::None)
        .value("IsDiscoverable", csp::systems::SpaceAttributes::IsDiscoverable)
        .value("RequiresInvite", csp::systems::SpaceAttributes::RequiresInvite)
        .value("Unlisted", csp::systems::SpaceAttributes::Unlisted)
        .value("Public", csp::systems::SpaceAttributes::Public)
        .value("Private", csp::systems::SpaceAttributes::Private)
        .value("Gated", csp::systems::SpaceAttributes::Gated);

    emscripten::class_<csp::systems::BasicSpace>("BasicSpace")
        .class_function(
            "create", +[]() { return csp::systems::BasicSpace(); })
        .property("id", &csp::systems::BasicSpace::Id)
        .property("name", &csp::systems::BasicSpace::Name)
        .property("description", &csp::systems::BasicSpace::Description)
        .property("attributes", &csp::systems::BasicSpace::Attributes)
        .property("tags", &csp::systems::BasicSpace::Tags)
        .function("equals(other)", &csp::systems::BasicSpace::operator==);

    emscripten::class_<csp::systems::Space, emscripten::base<csp::systems::BasicSpace>>("Space")
        .class_function(
            "create", +[]() { return csp::systems::Space(); })
        .property("createdBy", &csp::systems::Space::CreatedBy)
        .property("createdAt", &csp::systems::Space::CreatedAt)
        .property("ownerId", &csp::systems::Space::OwnerId)
        .property("userIds", &csp::systems::Space::UserIds)
        .property("moderatorIds", &csp::systems::Space::ModeratorIds)
        .property("bannedUserIds", &csp::systems::Space::BannedUserIds)
        .function("equals(other)", &csp::systems::Space::operator==);

    emscripten::class_<csp::systems::SpaceGeoLocation>("SpaceGeoLocation")
        .class_function(
            "create", +[]() { return csp::systems::SpaceGeoLocation(); })
        .property("spaceId", &csp::systems::SpaceGeoLocation::SpaceId)
        .property("orientation", &csp::systems::SpaceGeoLocation::Orientation)
        .function(
            "getLocation", +[](const csp::systems::SpaceGeoLocation& self) { return self.Location; })
        .function(
            "setLocation(value)", +[](csp::systems::SpaceGeoLocation& self, csp::systems::GeoLocation value) { self.Location = std::move(value); })
        .function(
            "getGeoFence",
            +[](const csp::systems::SpaceGeoLocation& self) { return bindings::utils::JSDisposable<csp::common::Array<csp::systems::GeoLocation>> { self.GeoFence }; })
        .function(
            "setGeoFence(value)", +[](csp::systems::SpaceGeoLocation& self, csp::common::Array<csp::systems::GeoLocation> value) { self.GeoFence = std::move(value); })
        .function("equals(other)", &csp::systems::SpaceGeoLocation::operator==);

    emscripten::class_<csp::systems::SpaceResult, emscripten::base<csp::systems::ResultBase>>("SpaceResult")
        .property("spaceCode", &csp::systems::SpaceResult::GetSpaceCode)
        .function(
            "getSpace", +[](const csp::systems::SpaceResult& self) { return self.GetSpace(); });

    emscripten::class_<csp::systems::SpacesResult, emscripten::base<csp::systems::ResultBase>>("SpacesResult")
        .function(
            "getSpaces", +[](const csp::systems::SpacesResult& self) { return bindings::utils::JSDisposable<csp::common::Array<csp::systems::Space>> { self.GetSpaces() }; });

    emscripten::class_<csp::systems::BasicSpaceResult, emscripten::base<csp::systems::ResultBase>>("BasicSpaceResult")
        .function(
            "getSpace", +[](const csp::systems::BasicSpaceResult& self) { return self.GetSpace(); });

    emscripten::class_<csp::systems::BasicSpacesResult, emscripten::base<csp::systems::ResultBase>>("BasicSpacesResult")
        .property("totalCount", &csp::systems::BasicSpacesResult::GetTotalCount)
        .function(
            "getSpaces",
            +[](const csp::systems::BasicSpacesResult& self) { return bindings::utils::JSDisposable<csp::common::Array<csp::systems::BasicSpace>> { self.GetSpaces() }; });

    emscripten::class_<csp::systems::SpaceMetadataResult, emscripten::base<csp::systems::ResultBase>>("SpaceMetadataResult")
        .property("metadata", &csp::systems::SpaceMetadataResult::GetMetadata);

    emscripten::class_<csp::systems::SpacesMetadataResult, emscripten::base<csp::systems::ResultBase>>("SpacesMetadataResult")
        .property("metadata", &csp::systems::SpacesMetadataResult::GetMetadata)
        .property("tags", &csp::systems::SpacesMetadataResult::GetTags);

    emscripten::class_<csp::systems::PendingInvitesResult, emscripten::base<csp::systems::ResultBase>>("PendingInvitesResult")
        .property(
            "pendingInvitesEmails", +[](const csp::systems::PendingInvitesResult& self) { return csp::common::Array<csp::common::String> { self.GetPendingInvitesEmails() }; });

    emscripten::class_<csp::systems::AcceptedInvitesResult, emscripten::base<csp::systems::ResultBase>>("AcceptedInvitesResult")
        .property(
            "acceptedInvitesUserIds",
            +[](const csp::systems::AcceptedInvitesResult& self) { return csp::common::Array<csp::common::String> { self.GetAcceptedInvitesUserIds() }; });

    emscripten::class_<csp::systems::SpaceGeoLocationResult, emscripten::base<csp::systems::ResultBase>>("SpaceGeoLocationResult")
        .property("hasSpaceGeoLocation", &csp::systems::SpaceGeoLocationResult::HasSpaceGeoLocation)
        .function(
            "getSpaceGeoLocation", +[](const csp::systems::SpaceGeoLocationResult& self) { return self.GetSpaceGeoLocation(); });
}
