#include "../../../containers/Array.h"
#include "../../../containers/String.h"
#include "../../../utils/JSDisposable.h"

#include "CSP/Common/Array.h"
#include "CSP/Common/String.h"
#include "CSP/Systems/Spaces/Site.h"
#include "CSP/Systems/Spatial/SpatialDataTypes.h"
#include "CSP/Systems/WebService.h"

#include "emscripten/bind.h"
#include <utility>

EMSCRIPTEN_BINDINGS(CSPSite)
{
    emscripten::class_<csp::systems::Site>("Site")
        .class_function(
            "create", +[]() { return csp::systems::Site(); })
        .property("id", &csp::systems::Site::Id)
        .property("name", &csp::systems::Site::Name)
        .property("spaceId", &csp::systems::Site::SpaceId)
        .function(
            "getLocation", +[](const csp::systems::Site& self) { return self.Location; })
        .function(
            "setLocation(value)", +[](csp::systems::Site& self, csp::systems::GeoLocation value) { self.Location = std::move(value); })
        .function(
            "getRotation", +[](const csp::systems::Site& self) { return self.Rotation; })
        .function(
            "setRotation(value)", +[](csp::systems::Site& self, csp::systems::OlyRotation value) { self.Rotation = std::move(value); })
        .function("equals(other)", &csp::systems::Site::operator==);

    emscripten::class_<csp::systems::SiteResult, emscripten::base<csp::systems::ResultBase>>("SiteResult")
        .function(
            "getSite", +[](const csp::systems::SiteResult& self) { return self.GetSite(); });

    emscripten::class_<csp::systems::SitesCollectionResult, emscripten::base<csp::systems::ResultBase>>("SitesCollectionResult")
        .function(
            "getSites", +[](const csp::systems::SitesCollectionResult& self) { return bindings::utils::JSDisposable<csp::common::Array<csp::systems::Site>> { self.GetSites() }; });
}
