#include "../../../async/Promises.h"
#include "../../../containers/Array.h"
#include "../../../containers/Map.h"
#include "../../../containers/Optional.h"
#include "../../../containers/String.h"
#include "../../CallbackDeclarations.h"
#include "../../PromiseDeclarations.h"

#include "CSP/Common/Array.h"
#include "CSP/Common/CancellationToken.h"
#include "CSP/Common/Map.h"
#include "CSP/Common/Optional.h"
#include "CSP/Common/SharedEnums.h"
#include "CSP/Common/String.h"
#include "CSP/Systems/Assets/Asset.h"
#include "CSP/Systems/Assets/AssetCollection.h"
#include "CSP/Systems/Assets/AssetSystem.h"
#include "CSP/Systems/Assets/Material.h"
#include "CSP/Systems/SystemBase.h"

#include "emscripten/bind.h"
#include "emscripten/val.h"

namespace emscripten::internal {
template <> void raw_destructor<csp::systems::AssetSystem>(csp::systems::AssetSystem*) { }
}

EMSCRIPTEN_BINDINGS(CSPAssetSystem)
{
    emscripten::class_<csp::systems::AssetSystem, emscripten::base<csp::systems::SystemBase>>("AssetSystem")
        .function(
            "createAssetCollection(spaceId, parentAssetCollectionId, assetCollectionName, metadata, type, tags)",
            +[](csp::systems::AssetSystem& self, const csp::common::Optional<csp::common::String>& spaceId,
                 const csp::common::Optional<csp::common::String>& parentAssetCollectionId, const csp::common::String& assetCollectionName,
                 const csp::common::Optional<csp::common::Map<csp::common::String, csp::common::String>>& metadata, csp::systems::EAssetCollectionType type,
                 const csp::common::Optional<csp::common::Array<csp::common::String>>& tags) {
                return Promisify<PromiseOfAssetCollectionResult>([&](emscripten::val cb) {
                    self.CreateAssetCollection(
                        spaceId, parentAssetCollectionId, assetCollectionName, metadata, type, tags, ToNativeCallback(cb.as<AssetCollectionResultCallback>()));
                });
            })
        .function(
            "deleteAssetCollection(assetCollection)",
            +[](csp::systems::AssetSystem& self, const csp::systems::AssetCollection& assetCollection) {
                return Promisify<PromiseOfNullResult>(
                    [&](emscripten::val cb) { self.DeleteAssetCollection(assetCollection, ToNativeCallback(cb.as<NullResultCallback>())); });
            })
        .function(
            "deleteMultipleAssetCollections(assetCollections)",
            +[](csp::systems::AssetSystem& self, const csp::common::Array<csp::systems::AssetCollection>& assetCollections) {
                return Promisify<PromiseOfNullResult>(
                    [&](emscripten::val cb) { self.DeleteMultipleAssetCollections(assetCollections, ToNativeCallback(cb.as<NullResultCallback>())); });
            })
        .function(
            "copyAssetCollectionsToSpace(sourceAssetCollections, destSpaceId, copyAsync)",
            +[](csp::systems::AssetSystem& self, const csp::common::Array<csp::systems::AssetCollection>& sourceAssetCollections, const csp::common::String& destSpaceId,
                 bool copyAsync) {
                return Promisify<PromiseOfAssetCollectionsResult>([&](emscripten::val cb) {
                    self.CopyAssetCollectionsToSpace(sourceAssetCollections, destSpaceId, copyAsync, ToNativeCallback(cb.as<AssetCollectionsResultCallback>()));
                });
            })
        .function(
            "getAssetCollectionById(assetCollectionId)",
            +[](csp::systems::AssetSystem& self, const csp::common::String& assetCollectionId) {
                return Promisify<PromiseOfAssetCollectionResult>(
                    [&](emscripten::val cb) { self.GetAssetCollectionById(assetCollectionId, ToNativeCallback(cb.as<AssetCollectionResultCallback>())); });
            })
        .function(
            "getAssetCollectionByName(assetCollectionName)",
            +[](csp::systems::AssetSystem& self, const csp::common::String& assetCollectionName) {
                return Promisify<PromiseOfAssetCollectionResult>(
                    [&](emscripten::val cb) { self.GetAssetCollectionByName(assetCollectionName, ToNativeCallback(cb.as<AssetCollectionResultCallback>())); });
            })
        .function(
            "findAssetCollections(assetCollectionIds, parentId, names, types, tags, spaceIds, resultsSkipNumber, resultsMaxNumber)",
            +[](csp::systems::AssetSystem& self, const csp::common::Optional<csp::common::Array<csp::common::String>>& assetCollectionIds,
                 const csp::common::Optional<csp::common::String>& parentId, const csp::common::Optional<csp::common::Array<csp::common::String>>& names,
                 const csp::common::Optional<csp::common::Array<csp::systems::EAssetCollectionType>>& types,
                 const csp::common::Optional<csp::common::Array<csp::common::String>>& tags,
                 const csp::common::Optional<csp::common::Array<csp::common::String>>& spaceIds, const csp::common::Optional<int>& resultsSkipNumber,
                 const csp::common::Optional<int>& resultsMaxNumber) {
                return Promisify<PromiseOfAssetCollectionsResult>([&](emscripten::val cb) {
                    self.FindAssetCollections(assetCollectionIds, parentId, names, types, tags, spaceIds, resultsSkipNumber, resultsMaxNumber,
                        ToNativeCallback(cb.as<AssetCollectionsResultCallback>()));
                });
            })
        .function(
            "updateAssetCollectionMetadata(assetCollection, newMetadata, tags)",
            +[](csp::systems::AssetSystem& self, const csp::systems::AssetCollection& assetCollection,
                 const csp::common::Map<csp::common::String, csp::common::String>& newMetadata,
                 const csp::common::Optional<csp::common::Array<csp::common::String>>& tags) {
                return Promisify<PromiseOfAssetCollectionResult>([&](emscripten::val cb) {
                    self.UpdateAssetCollectionMetadata(assetCollection, newMetadata, tags, ToNativeCallback(cb.as<AssetCollectionResultCallback>()));
                });
            })
        .function(
            "getAssetCollectionCount(ids, parentId, names, types, tags, spaceIds)",
            +[](csp::systems::AssetSystem& self, const csp::common::Optional<csp::common::Array<csp::common::String>>& ids,
                 const csp::common::Optional<csp::common::String>& parentId, const csp::common::Optional<csp::common::Array<csp::common::String>>& names,
                 const csp::common::Optional<csp::common::Array<csp::systems::EAssetCollectionType>>& types,
                 const csp::common::Optional<csp::common::Array<csp::common::String>>& tags,
                 const csp::common::Optional<csp::common::Array<csp::common::String>>& spaceIds) {
                return Promisify<PromiseOfAssetCollectionCountResult>([&](emscripten::val cb) {
                    self.GetAssetCollectionCount(ids, parentId, names, types, tags, spaceIds, ToNativeCallback(cb.as<AssetCollectionCountResultCallback>()));
                });
            })
        .function(
            "createAsset(assetCollection, name, thirdPartyPackagedAssetIdentifier, thirdPartyPlatform, type)",
            +[](csp::systems::AssetSystem& self, const csp::systems::AssetCollection& assetCollection, const csp::common::String& name,
                 const csp::common::Optional<csp::common::String>& thirdPartyPackagedAssetIdentifier,
                 const csp::common::Optional<csp::systems::EThirdPartyPlatform>& thirdPartyPlatform, csp::systems::EAssetType type) {
                return Promisify<PromiseOfAssetResult>([&](emscripten::val cb) {
                    self.CreateAsset(
                        assetCollection, name, thirdPartyPackagedAssetIdentifier, thirdPartyPlatform, type, ToNativeCallback(cb.as<AssetResultCallback>()));
                });
            })
        .function(
            "updateAsset(asset)",
            +[](csp::systems::AssetSystem& self, const csp::systems::Asset& asset) {
                return Promisify<PromiseOfAssetResult>([&](emscripten::val cb) { self.UpdateAsset(asset, ToNativeCallback(cb.as<AssetResultCallback>())); });
            })
        .function(
            "deleteAsset(assetCollection, asset)",
            +[](csp::systems::AssetSystem& self, const csp::systems::AssetCollection& assetCollection, const csp::systems::Asset& asset) {
                return Promisify<PromiseOfNullResult>(
                    [&](emscripten::val cb) { self.DeleteAsset(assetCollection, asset, ToNativeCallback(cb.as<NullResultCallback>())); });
            })
        .function(
            "getAssetsInCollection(assetCollection)",
            +[](csp::systems::AssetSystem& self, const csp::systems::AssetCollection& assetCollection) {
                return Promisify<PromiseOfAssetsResult>(
                    [&](emscripten::val cb) { self.GetAssetsInCollection(assetCollection, ToNativeCallback(cb.as<AssetsResultCallback>())); });
            })
        .function(
            "getAssetById(assetCollectionId, assetId)",
            +[](csp::systems::AssetSystem& self, const csp::common::String& assetCollectionId, const csp::common::String& assetId) {
                return Promisify<PromiseOfAssetResult>(
                    [&](emscripten::val cb) { self.GetAssetById(assetCollectionId, assetId, ToNativeCallback(cb.as<AssetResultCallback>())); });
            })
        .function(
            "getAssetsByCollectionIds(assetCollectionIds)",
            +[](csp::systems::AssetSystem& self, const csp::common::Array<csp::common::String>& assetCollectionIds) {
                return Promisify<PromiseOfAssetsResult>(
                    [&](emscripten::val cb) { self.GetAssetsByCollectionIds(assetCollectionIds, ToNativeCallback(cb.as<AssetsResultCallback>())); });
            })
        .function(
            "getAssetsByCriteria(assetCollectionIds, assetIds, assetNames, assetTypes)",
            +[](csp::systems::AssetSystem& self, const csp::common::Array<csp::common::String>& assetCollectionIds,
                 const csp::common::Optional<csp::common::Array<csp::common::String>>& assetIds,
                 const csp::common::Optional<csp::common::Array<csp::common::String>>& assetNames,
                 const csp::common::Optional<csp::common::Array<csp::systems::EAssetType>>& assetTypes) {
                return Promisify<PromiseOfAssetsResult>([&](emscripten::val cb) {
                    self.GetAssetsByCriteria(assetCollectionIds, assetIds, assetNames, assetTypes, ToNativeCallback(cb.as<AssetsResultCallback>()));
                });
            })
        .function(
            "uploadAssetData(assetCollection, asset, assetDataSource, progressCallback)",
            +[](csp::systems::AssetSystem& self, const csp::systems::AssetCollection& assetCollection, const csp::systems::Asset& asset,
                 const csp::systems::AssetDataSource& assetDataSource, const csp::common::Optional<ProgressCallback>& progressCallback) {
                return Promisify<PromiseOfUriResult>(
                    [&](emscripten::val cb) { self.UploadAssetData(assetCollection, asset, assetDataSource, ToNativeCallback(cb.as<UriResultCallback>())); },
                    progressCallback);
            })
        .function(
            "uploadAssetDataEx(assetCollection, asset, assetDataSource, cancellationToken, progressCallback)",
            +[](csp::systems::AssetSystem& self, const csp::systems::AssetCollection& assetCollection, const csp::systems::Asset& asset,
                 const csp::systems::AssetDataSource& assetDataSource, csp::common::CancellationToken& cancellationToken,
                 const csp::common::Optional<ProgressCallback>& progressCallback) {
                return Promisify<PromiseOfUriResult>(
                    [&](emscripten::val cb) {
                        self.UploadAssetDataEx(assetCollection, asset, assetDataSource, cancellationToken, ToNativeCallback(cb.as<UriResultCallback>()));
                    },
                    progressCallback);
            })
        .function(
            "downloadAssetData(asset)",
            +[](csp::systems::AssetSystem& self, const csp::systems::Asset& asset) {
                return Promisify<PromiseOfAssetDataResult>(
                    [&](emscripten::val cb) { self.DownloadAssetData(asset, ToNativeCallback(cb.as<AssetDataResultCallback>())); });
            })
        .function(
            "downloadAssetDataEx(asset, cancellationToken)",
            +[](csp::systems::AssetSystem& self, const csp::systems::Asset& asset, csp::common::CancellationToken& cancellationToken) {
                return Promisify<PromiseOfAssetDataResult>(
                    [&](emscripten::val cb) { self.DownloadAssetDataEx(asset, cancellationToken, ToNativeCallback(cb.as<AssetDataResultCallback>())); });
            })
        .function(
            "getAssetDataSize(asset)",
            +[](csp::systems::AssetSystem& self, const csp::systems::Asset& asset) {
                return Promisify<PromiseOfUInt64Result>([&](emscripten::val cb) { self.GetAssetDataSize(asset, ToNativeCallback(cb.as<UInt64ResultCallback>())); });
            })
        .function(
            "getLodChain(assetCollection)",
            +[](csp::systems::AssetSystem& self, const csp::systems::AssetCollection& assetCollection) {
                return Promisify<PromiseOfLODChainResult>(
                    [&](emscripten::val cb) { self.GetLODChain(assetCollection, ToNativeCallback(cb.as<LODChainResultCallback>())); });
            })
        .function(
            "registerAssetToLodChain(assetCollection, asset, lodLevel, progressCallback)",
            +[](csp::systems::AssetSystem& self, const csp::systems::AssetCollection& assetCollection, const csp::systems::Asset& asset, int lodLevel,
                 const csp::common::Optional<ProgressCallback>& progressCallback) {
                return Promisify<PromiseOfAssetResult>(
                    [&](emscripten::val cb) { self.RegisterAssetToLODChain(assetCollection, asset, lodLevel, ToNativeCallback(cb.as<AssetResultCallback>())); },
                    progressCallback);
            })
        .function(
            "createMaterial(name, shaderType, spaceId, metadata, assetTags)",
            +[](csp::systems::AssetSystem& self, const csp::common::String& name, csp::systems::EShaderType shaderType, const csp::common::String& spaceId,
                 const csp::common::Map<csp::common::String, csp::common::String>& metadata, const csp::common::Array<csp::common::String>& assetTags) {
                return Promisify<PromiseOfMaterialResult>([&](emscripten::val cb) {
                    self.CreateMaterial(name, shaderType, spaceId, metadata, assetTags, ToNativeCallback(cb.as<MaterialResultCallback>()));
                });
            })
        .function(
            "updateMaterial(material)",
            +[](csp::systems::AssetSystem& self, const csp::systems::Material& material) {
                return Promisify<PromiseOfNullResult>([&](emscripten::val cb) { self.UpdateMaterial(material, ToNativeCallback(cb.as<NullResultCallback>())); });
            })
        .function(
            "deleteMaterial(material)",
            +[](csp::systems::AssetSystem& self, const csp::systems::Material& material) {
                return Promisify<PromiseOfNullResult>([&](emscripten::val cb) { self.DeleteMaterial(material, ToNativeCallback(cb.as<NullResultCallback>())); });
            })
        .function(
            "getMaterials(spaceId)",
            +[](csp::systems::AssetSystem& self, const csp::common::String& spaceId) {
                return Promisify<PromiseOfMaterialsResult>([&](emscripten::val cb) { self.GetMaterials(spaceId, ToNativeCallback(cb.as<MaterialsResultCallback>())); });
            })
        .function(
            "getMaterial(assetCollectionId, assetId)",
            +[](csp::systems::AssetSystem& self, const csp::common::String& assetCollectionId, const csp::common::String& assetId) {
                return Promisify<PromiseOfMaterialResult>(
                    [&](emscripten::val cb) { self.GetMaterial(assetCollectionId, assetId, ToNativeCallback(cb.as<MaterialResultCallback>())); });
            })
        .function(
            "getMaterialFromUri(assetCollection, assetId, uri)",
            +[](csp::systems::AssetSystem& self, const csp::systems::AssetCollection& assetCollection, const csp::common::String& assetId,
                 const csp::common::String& uri) {
                return Promisify<PromiseOfMaterialResult>(
                    [&](emscripten::val cb) { self.GetMaterialFromUri(assetCollection, assetId, uri, ToNativeCallback(cb.as<MaterialResultCallback>())); });
            })
        .function(
            "setAssetDetailBlobChangedCallback(callback)",
            +[](csp::systems::AssetSystem& self, AssetDetailBlobChangedEventCallback callback) { self.SetAssetDetailBlobChangedCallback(ToNativeCallback(callback)); })
        .function(
            "setMaterialChangedCallback(callback)",
            +[](csp::systems::AssetSystem& self, MaterialChangedCallback callback) { self.SetMaterialChangedCallback(ToNativeCallback(callback)); });
}
