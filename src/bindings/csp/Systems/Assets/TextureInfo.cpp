#include "../../../containers/String.h"

#include "CSP/Common/SharedEnums.h"
#include "CSP/Common/String.h"
#include "CSP/Common/Vector.h"
#include "CSP/Systems/Assets/TextureInfo.h"

#include "emscripten/bind.h"

EMSCRIPTEN_BINDINGS(CSPTextureInfo)
{
    emscripten::enum_<csp::systems::ETextureResourceType>("ETextureResourceType", emscripten::enum_value_type::number)
        .value("Component", csp::systems::ETextureResourceType::Component)
        .value("ImageAsset", csp::systems::ETextureResourceType::ImageAsset);

    emscripten::class_<csp::systems::TextureInfo>("TextureInfo")
        .class_function(
            "create", +[]() { return csp::systems::TextureInfo(); })
        .class_function(
            "createFromAsset(assetCollectionId, assetId)",
            +[](const csp::common::String& assetCollectionId, const csp::common::String& assetId) { return csp::systems::TextureInfo(assetCollectionId, assetId); })
        .class_function(
            "createFromComponent(componentId)", +[](const csp::common::String& componentId) { return csp::systems::TextureInfo(componentId); })
        .property("assetCollectionId", &csp::systems::TextureInfo::GetAssetCollectionId)
        .property("assetId", &csp::systems::TextureInfo::GetAssetId)
        .property("entityComponentId", &csp::systems::TextureInfo::GetEntityComponentId, &csp::systems::TextureInfo::SetEntityComponentId)
        .property("sourceType", &csp::systems::TextureInfo::GetSourceType)
        .property("uvOffset", &csp::systems::TextureInfo::GetUVOffset, &csp::systems::TextureInfo::SetUVOffset)
        .property("uvRotation", &csp::systems::TextureInfo::GetUVRotation, &csp::systems::TextureInfo::SetUVRotation)
        .property("uvScale", &csp::systems::TextureInfo::GetUVScale, &csp::systems::TextureInfo::SetUVScale)
        .property("texCoord", &csp::systems::TextureInfo::GetTexCoord, &csp::systems::TextureInfo::SetTexCoord)
        .property("isSet", &csp::systems::TextureInfo::IsSet, &csp::systems::TextureInfo::SetTexture)
        .property("stereoVideoType", &csp::systems::TextureInfo::GetStereoVideoType, &csp::systems::TextureInfo::SetStereoVideoType)
        .property("isStereoFlipped", &csp::systems::TextureInfo::GetIsStereoFlipped, &csp::systems::TextureInfo::SetIsStereoFlipped)
        .function("setCollectionAndAssetId(assetCollectionId, assetId)", &csp::systems::TextureInfo::SetCollectionAndAssetId);
}
