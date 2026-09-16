#include "../../../containers/Array.h"
#include "../../../containers/Map.h"
#include "../../../containers/String.h"
#include "../../../utils/JSDisposable.h"

#include "CSP/Common/Array.h"
#include "CSP/Common/Map.h"
#include "CSP/Common/String.h"
#include "CSP/Systems/Assets/AssetCollection.h"
#include "CSP/Systems/WebService.h"

#include "emscripten/bind.h"

EMSCRIPTEN_BINDINGS(CSPAssetCollection)
{
    emscripten::enum_<csp::systems::EAssetCollectionType>("EAssetCollectionType", emscripten::enum_value_type::number)
        .value("DEFAULT", csp::systems::EAssetCollectionType::DEFAULT)
        .value("FOUNDATION_INTERNAL", csp::systems::EAssetCollectionType::FOUNDATION_INTERNAL)
        .value("COMMENT_CONTAINER", csp::systems::EAssetCollectionType::COMMENT_CONTAINER)
        .value("COMMENT", csp::systems::EAssetCollectionType::COMMENT)
        .value("SPACE_THUMBNAIL", csp::systems::EAssetCollectionType::SPACE_THUMBNAIL);

    emscripten::class_<csp::systems::AssetCollection>("AssetCollection")
        .class_function(
            "create", +[]() { return csp::systems::AssetCollection(); })
        .property("id", &csp::systems::AssetCollection::Id)
        .property("name", &csp::systems::AssetCollection::Name)
        .property("type", &csp::systems::AssetCollection::Type)
        .property("tags", &csp::systems::AssetCollection::Tags)
        .property("pointOfInterestId", &csp::systems::AssetCollection::PointOfInterestId)
        .property("parentId", &csp::systems::AssetCollection::ParentId)
        .property("spaceId", &csp::systems::AssetCollection::SpaceId)
        .property("createdBy", &csp::systems::AssetCollection::CreatedBy)
        .property("createdAt", &csp::systems::AssetCollection::CreatedAt)
        .property("updatedBy", &csp::systems::AssetCollection::UpdatedBy)
        .property("updatedAt", &csp::systems::AssetCollection::UpdatedAt)
        .property("isUnique", &csp::systems::AssetCollection::IsUnique)
        .property("version", &csp::systems::AssetCollection::Version)
        .property(
            "metadata", +[](const csp::systems::AssetCollection& self) { return csp::common::Map<csp::common::String, csp::common::String> { self.GetMetadataImmutable() }; },
            +[](csp::systems::AssetCollection& self, const csp::common::Map<csp::common::String, csp::common::String>& value) { self.GetMetadataMutable() = value; })
        .function("equals(other)", &csp::systems::AssetCollection::operator==);

    emscripten::class_<csp::systems::AssetCollectionResult, emscripten::base<csp::systems::ResultBase>>("AssetCollectionResult")
        .function("getAssetCollection", +[](const csp::systems::AssetCollectionResult& self) { return self.GetAssetCollection(); });

    emscripten::class_<csp::systems::AssetCollectionsResult, emscripten::base<csp::systems::ResultBase>>("AssetCollectionsResult")
        .property("totalCount", &csp::systems::AssetCollectionsResult::GetTotalCount)
        .function(
            "getAssetCollections", +[](const csp::systems::AssetCollectionsResult& self) {
                return bindings::utils::JSDisposable<csp::common::Array<csp::systems::AssetCollection>> { self.GetAssetCollections() };
            });

    emscripten::class_<csp::systems::AssetCollectionCountResult, emscripten::base<csp::systems::ResultBase>>("AssetCollectionCountResult")
        .property("count", &csp::systems::AssetCollectionCountResult::GetCount);
}
