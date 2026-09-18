#include "../../../async/Promises.h"
#include "../../../containers/Array.h"
#include "../../../containers/Map.h"
#include "../../../containers/Optional.h"
#include "../../../containers/String.h"
#include "../../CallbackDeclarations.h"
#include "../../PromiseDeclarations.h"

#include "CSP/Common/Array.h"
#include "CSP/Common/Map.h"
#include "CSP/Common/Optional.h"
#include "CSP/Common/String.h"
#include "CSP/Systems/Spatial/Anchor.h"
#include "CSP/Systems/Spatial/AnchorSystem.h"
#include "CSP/Systems/Spatial/SpatialDataTypes.h"
#include "CSP/Systems/SystemBase.h"

#include "emscripten/bind.h"
#include "emscripten/val.h"
#include <cstdint>

namespace emscripten::internal {
template <> void raw_destructor<csp::systems::AnchorSystem>(csp::systems::AnchorSystem*) { }
}

EMSCRIPTEN_BINDINGS(CSPAnchorSystem)
{
    emscripten::class_<csp::systems::AnchorSystem, emscripten::base<csp::systems::SystemBase>>("AnchorSystem")
        .function(
            "createAnchor(thirdPartyAnchorProvider, thirdPartyAnchorId, assetCollectionId, location, position, rotation, spatialKeyValue, tags)",
            +[](csp::systems::AnchorSystem& self, csp::systems::AnchorProvider thirdPartyAnchorProvider, const csp::common::String& thirdPartyAnchorId,
                 const csp::common::String& assetCollectionId, const csp::systems::GeoLocation& location, const csp::systems::OlyAnchorPosition& position,
                 const csp::systems::OlyRotation& rotation, const csp::common::Optional<csp::common::Map<csp::common::String, csp::common::String>>& spatialKeyValue,
                 const csp::common::Optional<csp::common::Array<csp::common::String>>& tags) {
                return Promisify<PromiseOfAnchorResult>([&](emscripten::val cb) {
                    self.CreateAnchor(thirdPartyAnchorProvider, thirdPartyAnchorId, assetCollectionId, location, position, rotation, spatialKeyValue, tags,
                        ToNativeCallback(cb.as<AnchorResultCallback>()));
                });
            })
        .function(
            "createAnchorInSpace(thirdPartyAnchorProvider, thirdPartyAnchorId, spaceId, spaceEntityId, assetCollectionId, location, position, rotation, "
            "spatialKeyValue, tags)",
            +[](csp::systems::AnchorSystem& self, csp::systems::AnchorProvider thirdPartyAnchorProvider, const csp::common::String& thirdPartyAnchorId,
                 const csp::common::String& spaceId, uint64_t spaceEntityId, const csp::common::String& assetCollectionId, const csp::systems::GeoLocation& location,
                 const csp::systems::OlyAnchorPosition& position, const csp::systems::OlyRotation& rotation,
                 const csp::common::Optional<csp::common::Map<csp::common::String, csp::common::String>>& spatialKeyValue,
                 const csp::common::Optional<csp::common::Array<csp::common::String>>& tags) {
                return Promisify<PromiseOfAnchorResult>([&](emscripten::val cb) {
                    self.CreateAnchorInSpace(thirdPartyAnchorProvider, thirdPartyAnchorId, spaceId, spaceEntityId, assetCollectionId, location, position, rotation, spatialKeyValue,
                        tags, ToNativeCallback(cb.as<AnchorResultCallback>()));
                });
            })
        .function(
            "deleteAnchors(anchorIds)",
            +[](csp::systems::AnchorSystem& self, const csp::common::Array<csp::common::String>& anchorIds) {
                return Promisify<PromiseOfNullResult>([&](emscripten::val cb) { self.DeleteAnchors(anchorIds, ToNativeCallback(cb.as<NullResultCallback>())); });
            })
        .function(
            "getAnchorsInArea(originLocation, areaRadius, spatialKeys, spatialValues, tags, allTags, spaceIds, skip, limit)",
            +[](csp::systems::AnchorSystem& self, const csp::systems::GeoLocation& originLocation, double areaRadius,
                 const csp::common::Optional<csp::common::Array<csp::common::String>>& spatialKeys,
                 const csp::common::Optional<csp::common::Array<csp::common::String>>& spatialValues, const csp::common::Optional<csp::common::Array<csp::common::String>>& tags,
                 const csp::common::Optional<bool>& allTags, const csp::common::Optional<csp::common::Array<csp::common::String>>& spaceIds, const csp::common::Optional<int>& skip,
                 const csp::common::Optional<int>& limit) {
                return Promisify<PromiseOfAnchorCollectionResult>([&](emscripten::val cb) {
                    self.GetAnchorsInArea(
                        originLocation, areaRadius, spatialKeys, spatialValues, tags, allTags, spaceIds, skip, limit, ToNativeCallback(cb.as<AnchorCollectionResultCallback>()));
                });
            })
        .function(
            "getAnchorsInSpace(spaceId, skip, limit)",
            +[](csp::systems::AnchorSystem& self, const csp::common::String& spaceId, const csp::common::Optional<int>& skip, const csp::common::Optional<int>& limit) {
                return Promisify<PromiseOfAnchorCollectionResult>(
                    [&](emscripten::val cb) { self.GetAnchorsInSpace(spaceId, skip, limit, ToNativeCallback(cb.as<AnchorCollectionResultCallback>())); });
            })
        .function(
            "getAnchorsByAssetCollectionId(assetCollectionId, skip, limit)",
            +[](csp::systems::AnchorSystem& self, const csp::common::String& assetCollectionId, const csp::common::Optional<int>& skip, const csp::common::Optional<int>& limit) {
                return Promisify<PromiseOfAnchorCollectionResult>(
                    [&](emscripten::val cb) { self.GetAnchorsByAssetCollectionId(assetCollectionId, skip, limit, ToNativeCallback(cb.as<AnchorCollectionResultCallback>())); });
            })
        .function(
            "createAnchorResolution(anchorId, successfullyResolved, resolveAttempted, resolveTime, tags)",
            +[](csp::systems::AnchorSystem& self, const csp::common::String& anchorId, bool successfullyResolved, int resolveAttempted, double resolveTime,
                 const csp::common::Array<csp::common::String>& tags) {
                return Promisify<PromiseOfAnchorResolutionResult>([&](emscripten::val cb) {
                    self.CreateAnchorResolution(anchorId, successfullyResolved, resolveAttempted, resolveTime, tags, ToNativeCallback(cb.as<AnchorResolutionResultCallback>()));
                });
            });
}
