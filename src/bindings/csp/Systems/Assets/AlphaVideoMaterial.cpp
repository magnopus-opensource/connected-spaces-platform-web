#include "../../../containers/String.h"

#include "CSP/Common/String.h"
#include "CSP/Common/Vector.h"
#include "CSP/Systems/Assets/AlphaVideoMaterial.h"
#include "CSP/Systems/Assets/Material.h"
#include "CSP/Systems/Assets/TextureInfo.h"

#include "emscripten/bind.h"

EMSCRIPTEN_BINDINGS(CSPAlphaVideoMaterial)
{
    emscripten::class_<csp::systems::AlphaVideoMaterial, emscripten::base<csp::systems::Material>>("AlphaVideoMaterial")
        .class_function(
            "create", +[]() { return csp::systems::AlphaVideoMaterial(); })
        .class_function(
            "createWithIds(name, materialCollectionId, materialId)",
            +[](const csp::common::String& name, const csp::common::String& materialCollectionId, const csp::common::String& materialId) {
                return csp::systems::AlphaVideoMaterial(name, materialCollectionId, materialId);
            })
        .property("doubleSided", &csp::systems::AlphaVideoMaterial::GetDoubleSided, &csp::systems::AlphaVideoMaterial::SetDoubleSided)
        .property("isEmissive", &csp::systems::AlphaVideoMaterial::GetIsEmissive, &csp::systems::AlphaVideoMaterial::SetIsEmissive)
        .property("readAlphaFromChannel", &csp::systems::AlphaVideoMaterial::GetReadAlphaFromChannel, &csp::systems::AlphaVideoMaterial::SetReadAlphaFromChannel)
        .property("blendMode", &csp::systems::AlphaVideoMaterial::GetBlendMode, &csp::systems::AlphaVideoMaterial::SetBlendMode)
        .property("fresnelFactor", &csp::systems::AlphaVideoMaterial::GetFresnelFactor, &csp::systems::AlphaVideoMaterial::SetFresnelFactor)
        .property("tint", &csp::systems::AlphaVideoMaterial::GetTint, &csp::systems::AlphaVideoMaterial::SetTint)
        .property("alphaFactor", &csp::systems::AlphaVideoMaterial::GetAlphaFactor, &csp::systems::AlphaVideoMaterial::SetAlphaFactor)
        .property("emissiveIntensity", &csp::systems::AlphaVideoMaterial::GetEmissiveIntensity, &csp::systems::AlphaVideoMaterial::SetEmissiveIntensity)
        .property("alphaMask", &csp::systems::AlphaVideoMaterial::GetAlphaMask, &csp::systems::AlphaVideoMaterial::SetAlphaMask)
        .function(
            "getColorTexture", +[](const csp::systems::AlphaVideoMaterial& self) { return self.GetColorTexture(); })
        .function("setColorTexture(texture)", &csp::systems::AlphaVideoMaterial::SetColorTexture);
}
