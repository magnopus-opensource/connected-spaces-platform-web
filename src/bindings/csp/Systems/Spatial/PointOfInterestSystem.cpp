#include "../../../async/Promises.h"
#include "../../../containers/Array.h"
#include "../../../containers/Optional.h"
#include "../../../containers/String.h"
#include "../../CallbackDeclarations.h"
#include "../../PromiseDeclarations.h"

#include "CSP/Common/Array.h"
#include "CSP/Common/Optional.h"
#include "CSP/Common/String.h"
#include "CSP/Systems/Assets/AssetCollection.h"
#include "CSP/Systems/Spatial/PointOfInterest.h"
#include "CSP/Systems/Spatial/PointOfInterestSystem.h"
#include "CSP/Systems/Spatial/SpatialDataTypes.h"
#include "CSP/Systems/SystemBase.h"

#include "emscripten/bind.h"
#include "emscripten/val.h"

namespace emscripten::internal {
template <> void raw_destructor<csp::systems::PointOfInterestSystem>(csp::systems::PointOfInterestSystem*) { }
}

EMSCRIPTEN_BINDINGS(CSPPointOfInterestSystem)
{
    emscripten::class_<csp::systems::PointOfInterestSystem, emscripten::base<csp::systems::SystemBase>>("PointOfInterestSystem")
        .function(
            "createPoi(title, description, name, tags, type, owner, location, assetCollection)",
            +[](csp::systems::PointOfInterestSystem& self, const csp::common::String& title, const csp::common::String& description, const csp::common::String& name,
                 const csp::common::Optional<csp::common::Array<csp::common::String>>& tags, csp::systems::EPointOfInterestType type, const csp::common::String& owner,
                 const csp::systems::GeoLocation& location, const csp::systems::AssetCollection& assetCollection) {
                return Promisify<PromiseOfPOIResult>([&](emscripten::val cb) {
                    self.CreatePOI(title, description, name, tags, type, owner, location, assetCollection, ToNativeCallback(cb.as<POIResultCallback>()));
                });
            })
        .function(
            "deletePoi(poi)",
            +[](csp::systems::PointOfInterestSystem& self, const csp::systems::PointOfInterest& poi) {
                return Promisify<PromiseOfNullResult>([&](emscripten::val cb) { self.DeletePOI(poi, ToNativeCallback(cb.as<NullResultCallback>())); });
            })
        .function(
            "getPoisInArea(originLocation, areaRadius, type)",
            +[](csp::systems::PointOfInterestSystem& self, const csp::systems::GeoLocation& originLocation, double areaRadius,
                 const csp::common::Optional<csp::systems::EPointOfInterestType>& type) {
                return Promisify<PromiseOfPOICollectionResult>(
                    [&](emscripten::val cb) { self.GetPOIsInArea(originLocation, areaRadius, type, ToNativeCallback(cb.as<POICollectionResultCallback>())); });
            });
}
