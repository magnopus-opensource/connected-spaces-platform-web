#include "../../../containers/Array.h"
#include "../../../containers/Map.h"
#include "../../../containers/String.h"
#include "../../../utils/JSDisposable.h"

#include "CSP/Common/Array.h"
#include "CSP/Common/Map.h"
#include "CSP/Common/String.h"
#include "CSP/Systems/Spatial/PointOfInterest.h"
#include "CSP/Systems/Spatial/SpatialDataTypes.h"
#include "CSP/Systems/WebService.h"

#include "emscripten/bind.h"
#include <utility>

EMSCRIPTEN_BINDINGS(CSPPointOfInterest)
{
    emscripten::enum_<csp::systems::EPointOfInterestType>("EPointOfInterestType", emscripten::enum_value_type::number)
        .value("DEFAULT", csp::systems::EPointOfInterestType::DEFAULT)
        .value("SPACE", csp::systems::EPointOfInterestType::SPACE);

    emscripten::class_<csp::systems::PointOfInterest>("PointOfInterest")
        .class_function(
            "create", +[]() { return csp::systems::PointOfInterest(); })
        .property("id", &csp::systems::PointOfInterest::Id)
        .property("createdBy", &csp::systems::PointOfInterest::CreatedBy)
        .property("createdAt", &csp::systems::PointOfInterest::CreatedAt)
        .property("title", &csp::systems::PointOfInterest::Title)
        .property("description", &csp::systems::PointOfInterest::Description)
        .property("name", &csp::systems::PointOfInterest::Name)
        .property("type", &csp::systems::PointOfInterest::Type)
        .property("tags", &csp::systems::PointOfInterest::Tags)
        .property("owner", &csp::systems::PointOfInterest::Owner)
        .property("assetCollectionId", &csp::systems::PointOfInterest::AssetCollectionId)
        .property("spaceId", &csp::systems::PointOfInterest::SpaceId)
        .function(
            "getLocation", +[](const csp::systems::PointOfInterest& self) { return self.Location; })
        .function(
            "setLocation(value)", +[](csp::systems::PointOfInterest& self, csp::systems::GeoLocation value) { self.Location = std::move(value); })
        .function("equals(other)", &csp::systems::PointOfInterest::operator==);

    emscripten::class_<csp::systems::POIResult, emscripten::base<csp::systems::ResultBase>>("POIResult")
        .function(
            "getPointOfInterest", +[](const csp::systems::POIResult& self) { return self.GetPointOfInterest(); });

    emscripten::class_<csp::systems::POICollectionResult, emscripten::base<csp::systems::ResultBase>>("POICollectionResult")
        .function(
            "getPois",
            +[](const csp::systems::POICollectionResult& self) { return bindings::utils::JSDisposable<csp::common::Array<csp::systems::PointOfInterest>> { self.GetPOIs() }; });
}
