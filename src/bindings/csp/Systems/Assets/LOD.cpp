#include "../../../containers/Array.h"
#include "../../../containers/String.h"
#include "../../../utils/JSDisposable.h"

#include "CSP/Common/Array.h"
#include "CSP/Common/String.h"
#include "CSP/Systems/Assets/Asset.h"
#include "CSP/Systems/Assets/LOD.h"
#include "CSP/Systems/WebService.h"

#include "emscripten/bind.h"
#include <utility>

EMSCRIPTEN_BINDINGS(CSPLOD)
{
    emscripten::class_<csp::systems::LODAsset>("LODAsset")
        .class_function(
            "create", +[]() { return csp::systems::LODAsset(); })
        .property("level", &csp::systems::LODAsset::Level)
        .function(
            "getAsset", +[](const csp::systems::LODAsset& self) { return self.Asset; })
        .function(
            "setAsset(value)", +[](csp::systems::LODAsset& self, csp::systems::Asset value) { self.Asset = std::move(value); })
        .function("equals(other)", &csp::systems::LODAsset::operator==);

    emscripten::class_<csp::systems::LODChain>("LODChain")
        .class_function(
            "create", +[]() { return csp::systems::LODChain(); })
        .property("assetCollectionId", &csp::systems::LODChain::AssetCollectionId)
        .function(
            "getLodAssets", +[](const csp::systems::LODChain& self) { return bindings::utils::JSDisposable<csp::common::Array<csp::systems::LODAsset>> { self.LODAssets }; })
        .function(
            "setLodAssets(value)", +[](csp::systems::LODChain& self, csp::common::Array<csp::systems::LODAsset> value) { self.LODAssets = std::move(value); });

    emscripten::class_<csp::systems::LODChainResult, emscripten::base<csp::systems::ResultBase>>("LODChainResult")
        .function(
            "getLodChain", +[](const csp::systems::LODChainResult& self) { return self.GetLODChain(); });
}
