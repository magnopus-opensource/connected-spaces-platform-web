#include "../../../containers/Array.h"
#include "../../../containers/NativeBuffer.h"
#include "../../../containers/String.h"
#include "../../../utils/JSDisposable.h"

#include "CSP/Common/Array.h"
#include "CSP/Common/SharedEnums.h"
#include "CSP/Common/String.h"
#include "CSP/Systems/Assets/Asset.h"
#include "CSP/Systems/WebService.h"

#include "emscripten/bind.h"
#include "emscripten/val.h"

namespace emscripten::internal {
template <> void raw_destructor<csp::systems::AssetDataSource>(csp::systems::AssetDataSource*) { }
}

EMSCRIPTEN_BINDINGS(CSPAsset)
{
    emscripten::enum_<csp::systems::EAssetType>("EAssetType", emscripten::enum_value_type::number)
        .value("IMAGE", csp::systems::EAssetType::IMAGE)
        .value("THUMBNAIL", csp::systems::EAssetType::THUMBNAIL)
        .value("SIMULATION", csp::systems::EAssetType::SIMULATION)
        .value("MODEL", csp::systems::EAssetType::MODEL)
        .value("VIDEO", csp::systems::EAssetType::VIDEO)
        .value("SCRIPT_LIBRARY", csp::systems::EAssetType::SCRIPT_LIBRARY)
        .value("HOLOCAP_VIDEO", csp::systems::EAssetType::HOLOCAP_VIDEO)
        .value("HOLOCAP_AUDIO", csp::systems::EAssetType::HOLOCAP_AUDIO)
        .value("AUDIO", csp::systems::EAssetType::AUDIO)
        .value("GAUSSIAN_SPLAT", csp::systems::EAssetType::GAUSSIAN_SPLAT)
        .value("MATERIAL", csp::systems::EAssetType::MATERIAL)
        .value("ANNOTATION", csp::systems::EAssetType::ANNOTATION)
        .value("ANNOTATION_THUMBNAIL", csp::systems::EAssetType::ANNOTATION_THUMBNAIL)
        .value("TEXT", csp::systems::EAssetType::TEXT);

    emscripten::enum_<csp::systems::EAssetPlatform>("EAssetPlatform", emscripten::enum_value_type::number).value("DEFAULT", csp::systems::EAssetPlatform::DEFAULT);

    emscripten::function("convertDTOAssetDetailType(dtoAssetDetailType)", &csp::systems::ConvertDTOAssetDetailType);
    emscripten::function("convertStringToAssetPlatform(platform)", &csp::systems::ConvertStringToAssetPlatform);
    emscripten::function("convertAssetPlatformToString(platform)", &csp::systems::ConvertAssetPlatformToString);

    emscripten::class_<csp::systems::Asset>("Asset")
        .class_function(
            "create", +[]() { return csp::systems::Asset(); })
        .property("assetCollectionId", &csp::systems::Asset::AssetCollectionId)
        .property("id", &csp::systems::Asset::Id)
        .property("fileName", &csp::systems::Asset::FileName)
        .property("name", &csp::systems::Asset::Name)
        .property("languageCode", &csp::systems::Asset::LanguageCode)
        .property("type", &csp::systems::Asset::Type)
        .property("platforms", &csp::systems::Asset::Platforms)
        .property("styles", &csp::systems::Asset::Styles)
        .property("externalUri", &csp::systems::Asset::ExternalUri)
        .property("uri", &csp::systems::Asset::Uri)
        .property("checksum", &csp::systems::Asset::Checksum)
        .property("version", &csp::systems::Asset::Version)
        .property("mimeType", &csp::systems::Asset::MimeType)
        .property("externalMimeType", &csp::systems::Asset::ExternalMimeType)
        .property("thirdPartyPackagedAssetIdentifier", &csp::systems::Asset::ThirdPartyPackagedAssetIdentifier)
        .property("thirdPartyPlatformType", &csp::systems::Asset::ThirdPartyPlatformType)
        .function("equals(other)", &csp::systems::Asset::operator==);

    emscripten::class_<csp::systems::AssetDataSource>("AssetDataSource")
        .property("mimeType", &csp::systems::AssetDataSource::GetMimeType, &csp::systems::AssetDataSource::SetMimeType);

    emscripten::class_<csp::systems::FileAssetDataSource, emscripten::base<csp::systems::AssetDataSource>>("FileAssetDataSource")
        .class_function(
            "create", +[]() { return csp::systems::FileAssetDataSource(); })
        .property("filePath", &csp::systems::FileAssetDataSource::FilePath);

    emscripten::class_<csp::systems::BufferAssetDataSource, emscripten::base<csp::systems::AssetDataSource>>("BufferAssetDataSource")
        .class_function(
            "create", +[]() { return csp::systems::BufferAssetDataSource(); })
        .property(
            "bufferLength", +[](const csp::systems::BufferAssetDataSource& self) { return self.BufferLength; })
        .function(
            "setBuffer(buffer)",
            +[](csp::systems::BufferAssetDataSource& self, const bindings::containers::NativeBuffer& buffer) {
                self.Buffer = buffer.GetData();
                self.BufferLength = buffer.GetLength();
            })
        .function(
            "getBufferView",
            +[](const csp::systems::BufferAssetDataSource& self) { return bindings::containers::MakeByteArrayView(static_cast<std::uint8_t*>(self.Buffer), self.BufferLength); });

    emscripten::class_<csp::systems::AssetResult, emscripten::base<csp::systems::ResultBase>>("AssetResult")
        .function(
            "getAsset", +[](const csp::systems::AssetResult& self) { return self.GetAsset(); });

    emscripten::class_<csp::systems::AssetsResult, emscripten::base<csp::systems::ResultBase>>("AssetsResult")
        .function(
            "getAssets", +[](const csp::systems::AssetsResult& self) { return bindings::utils::JSDisposable<csp::common::Array<csp::systems::Asset>> { self.GetAssets() }; });

    emscripten::class_<csp::systems::UriResult, emscripten::base<csp::systems::ResultBase>>("UriResult")
        .property("uri", &csp::systems::UriResult::GetUri, &csp::systems::UriResult::SetUri);

    emscripten::class_<csp::systems::AssetDataResult, emscripten::base<csp::systems::ResultBase>>("AssetDataResult")
        .property("dataLength", &csp::systems::AssetDataResult::GetDataLength)
        .function(
            "getData",
            +[](const csp::systems::AssetDataResult& self) {
                return bindings::containers::MakeByteArrayCopy(static_cast<const std::uint8_t*>(self.GetData()), self.GetDataLength());
            })
        .function(
            "getDataView", +[](const csp::systems::AssetDataResult& self) {
                return bindings::containers::MakeByteArrayView(static_cast<const std::uint8_t*>(self.GetData()), self.GetDataLength());
            });
}
