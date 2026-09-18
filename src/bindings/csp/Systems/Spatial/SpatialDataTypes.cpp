#include "CSP/Systems/Spatial/SpatialDataTypes.h"

#include "emscripten/bind.h"

EMSCRIPTEN_BINDINGS(CSPSpatialDataTypes)
{
    emscripten::class_<csp::systems::GeoLocation>("GeoLocation")
        .class_function(
            "create", +[]() { return csp::systems::GeoLocation(); })
        .class_function(
            "createWithCoordinates(longitude, latitude)", +[](double longitude, double latitude) { return csp::systems::GeoLocation(longitude, latitude); })
        .property("longitude", &csp::systems::GeoLocation::Longitude)
        .property("latitude", &csp::systems::GeoLocation::Latitude)
        .property("isValid", &csp::systems::GeoLocation::IsValid)
        .function("equals(other)", &csp::systems::GeoLocation::operator==);

    emscripten::class_<csp::systems::OlyRotation>("OlyRotation")
        .class_function(
            "create", +[]() { return csp::systems::OlyRotation(); })
        .class_function(
            "createWithComponents(x, y, z, w)", +[](double x, double y, double z, double w) { return csp::systems::OlyRotation(x, y, z, w); })
        .property("x", &csp::systems::OlyRotation::X)
        .property("y", &csp::systems::OlyRotation::Y)
        .property("z", &csp::systems::OlyRotation::Z)
        .property("w", &csp::systems::OlyRotation::W)
        .function("equals(other)", &csp::systems::OlyRotation::operator==);
}
