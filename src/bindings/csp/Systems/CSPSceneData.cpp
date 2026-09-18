#include "../../containers/Array.h"
#include "../../containers/List.h"
#include "../../containers/String.h"
#include "../../utils/JSDisposable.h"

#include "CSP/Common/Array.h"
#include "CSP/Common/List.h"
#include "CSP/Common/String.h"
#include "CSP/Systems/CSPSceneData.h"

#include "emscripten/bind.h"
#include <utility>

EMSCRIPTEN_BINDINGS(CSPSceneData)
{
    emscripten::class_<csp::systems::CSPSceneData>("CSPSceneData")
        .class_function(
            "createFromJson(sceneDescriptionJson)",
            +[](const csp::common::List<csp::common::String>& sceneDescriptionJson) { return csp::systems::CSPSceneData(sceneDescriptionJson); })
        .function(
            "getSpace", +[](const csp::systems::CSPSceneData& self) { return self.Space; })
        .function(
            "setSpace(value)", +[](csp::systems::CSPSceneData& self, csp::systems::Space value) { self.Space = std::move(value); })
        .function(
            "getAssetCollections",
            +[](const csp::systems::CSPSceneData& self) { return bindings::utils::JSDisposable<csp::common::Array<csp::systems::AssetCollection>> { self.AssetCollections }; })
        .function(
            "setAssetCollections(value)",
            +[](csp::systems::CSPSceneData& self, csp::common::Array<csp::systems::AssetCollection> value) { self.AssetCollections = std::move(value); })
        .function(
            "getAssets", +[](const csp::systems::CSPSceneData& self) { return bindings::utils::JSDisposable<csp::common::Array<csp::systems::Asset>> { self.Assets }; })
        .function(
            "setAssets(value)", +[](csp::systems::CSPSceneData& self, csp::common::Array<csp::systems::Asset> value) { self.Assets = std::move(value); })
        .function(
            "getSequences", +[](const csp::systems::CSPSceneData& self) { return bindings::utils::JSDisposable<csp::common::Array<csp::systems::Sequence>> { self.Sequences }; })
        .function(
            "setSequences(value)", +[](csp::systems::CSPSceneData& self, csp::common::Array<csp::systems::Sequence> value) { self.Sequences = std::move(value); })
        .function(
            "getAnchors", +[](const csp::systems::CSPSceneData& self) { return bindings::utils::JSDisposable<csp::common::Array<csp::systems::Anchor>> { self.Anchors }; })
        .function(
            "setAnchors(value)", +[](csp::systems::CSPSceneData& self, csp::common::Array<csp::systems::Anchor> value) { self.Anchors = std::move(value); });
}
