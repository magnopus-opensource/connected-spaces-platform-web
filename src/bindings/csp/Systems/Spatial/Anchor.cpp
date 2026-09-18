#include "../../../containers/Array.h"
#include "../../../containers/Map.h"
#include "../../../containers/String.h"
#include "../../../utils/JSDisposable.h"

#include "CSP/Common/Array.h"
#include "CSP/Common/Map.h"
#include "CSP/Common/String.h"
#include "CSP/Systems/Spatial/Anchor.h"
#include "CSP/Systems/Spatial/SpatialDataTypes.h"
#include "CSP/Systems/WebService.h"

#include "emscripten/bind.h"
#include <utility>

EMSCRIPTEN_BINDINGS(CSPAnchor)
{
    emscripten::enum_<csp::systems::AnchorProvider>("AnchorProvider", emscripten::enum_value_type::number)
        .value("GoogleCloudAnchors", csp::systems::AnchorProvider::GoogleCloudAnchors);

    emscripten::class_<csp::systems::OlyAnchorPosition>("OlyAnchorPosition")
        .class_function(
            "create", +[]() { return csp::systems::OlyAnchorPosition(); })
        .class_function(
            "createWithComponents(x, y, z)", +[](double x, double y, double z) { return csp::systems::OlyAnchorPosition(x, y, z); })
        .property("x", &csp::systems::OlyAnchorPosition::X)
        .property("y", &csp::systems::OlyAnchorPosition::Y)
        .property("z", &csp::systems::OlyAnchorPosition::Z)
        .function("equals(other)", &csp::systems::OlyAnchorPosition::operator==);

    emscripten::class_<csp::systems::Anchor>("Anchor")
        .class_function(
            "create", +[]() { return csp::systems::Anchor(); })
        .property("id", &csp::systems::Anchor::Id)
        .property("thirdPartyAnchorProvider", &csp::systems::Anchor::ThirdPartyAnchorProvider)
        .property("thirdPartyAnchorId", &csp::systems::Anchor::ThirdPartyAnchorId)
        .property("createdBy", &csp::systems::Anchor::CreatedBy)
        .property("createdAt", &csp::systems::Anchor::CreatedAt)
        .property("spaceId", &csp::systems::Anchor::SpaceId)
        .property("spaceEntityId", &csp::systems::Anchor::SpaceEntityId)
        .property("assetCollectionId", &csp::systems::Anchor::AssetCollectionId)
        .property("tags", &csp::systems::Anchor::Tags)
        .property("spatialKeyValue", &csp::systems::Anchor::SpatialKeyValue)
        .function(
            "getLocation", +[](const csp::systems::Anchor& self) { return self.Location; })
        .function(
            "setLocation(value)", +[](csp::systems::Anchor& self, csp::systems::GeoLocation value) { self.Location = std::move(value); })
        .function(
            "getPosition", +[](const csp::systems::Anchor& self) { return self.Position; })
        .function(
            "setPosition(value)", +[](csp::systems::Anchor& self, csp::systems::OlyAnchorPosition value) { self.Position = std::move(value); })
        .function(
            "getRotation", +[](const csp::systems::Anchor& self) { return self.Rotation; })
        .function(
            "setRotation(value)", +[](csp::systems::Anchor& self, csp::systems::OlyRotation value) { self.Rotation = std::move(value); })
        .function("equals(other)", &csp::systems::Anchor::operator==);

    emscripten::class_<csp::systems::AnchorResolution>("AnchorResolution")
        .class_function(
            "create", +[]() { return csp::systems::AnchorResolution(); })
        .property("id", &csp::systems::AnchorResolution::Id)
        .property("anchorId", &csp::systems::AnchorResolution::AnchorId)
        .property("successfullyResolved", &csp::systems::AnchorResolution::SuccessfullyResolved)
        .property("resolveAttempted", &csp::systems::AnchorResolution::ResolveAttempted)
        .property("resolveTime", &csp::systems::AnchorResolution::ResolveTime)
        .property("tags", &csp::systems::AnchorResolution::Tags)
        .function("equals(other)", &csp::systems::AnchorResolution::operator==);

    emscripten::class_<csp::systems::AnchorResult, emscripten::base<csp::systems::ResultBase>>("AnchorResult")
        .function("getAnchor", +[](const csp::systems::AnchorResult& self) { return self.GetAnchor(); });

    emscripten::class_<csp::systems::AnchorCollectionResult, emscripten::base<csp::systems::ResultBase>>("AnchorCollectionResult")
        .function(
            "getAnchors",
            +[](const csp::systems::AnchorCollectionResult& self) { return bindings::utils::JSDisposable<csp::common::Array<csp::systems::Anchor>> { self.GetAnchors() }; });

    emscripten::class_<csp::systems::AnchorResolutionResult, emscripten::base<csp::systems::ResultBase>>("AnchorResolutionResult")
        .function("getAnchorResolution", +[](const csp::systems::AnchorResolutionResult& self) { return self.GetAnchorResolution(); });
}
