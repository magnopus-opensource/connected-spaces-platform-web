#include "../../../containers/String.h"

#include "CSP/Common/String.h"
#include "CSP/Common/Vector.h"
#include "CSP/Systems/Assets/GLTFMaterial.h"
#include "CSP/Systems/Assets/Material.h"
#include "CSP/Systems/Assets/TextureInfo.h"

#include "emscripten/bind.h"

EMSCRIPTEN_BINDINGS(CSPGLTFMaterial)
{
    emscripten::class_<csp::systems::GLTFMaterial, emscripten::base<csp::systems::Material>>("GLTFMaterial")
        .class_function(
            "create", +[]() { return csp::systems::GLTFMaterial(); })
        .class_function(
            "createWithIds(name, materialCollectionId, materialId)",
            +[](const csp::common::String& name, const csp::common::String& materialCollectionId, const csp::common::String& materialId) {
                return csp::systems::GLTFMaterial(name, materialCollectionId, materialId);
            })
        .property("alphaMode", &csp::systems::GLTFMaterial::GetAlphaMode, &csp::systems::GLTFMaterial::SetAlphaMode)
        .property("alphaCutoff", &csp::systems::GLTFMaterial::GetAlphaCutoff, &csp::systems::GLTFMaterial::SetAlphaCutoff)
        .property("doubleSided", &csp::systems::GLTFMaterial::GetDoubleSided, &csp::systems::GLTFMaterial::SetDoubleSided)
        .property("baseColorFactor", &csp::systems::GLTFMaterial::GetBaseColorFactor, &csp::systems::GLTFMaterial::SetBaseColorFactor)
        .property("metallicFactor", &csp::systems::GLTFMaterial::GetMetallicFactor, &csp::systems::GLTFMaterial::SetMetallicFactor)
        .property("roughnessFactor", &csp::systems::GLTFMaterial::GetRoughnessFactor, &csp::systems::GLTFMaterial::SetRoughnessFactor)
        .property("emissiveFactor", &csp::systems::GLTFMaterial::GetEmissiveFactor, &csp::systems::GLTFMaterial::SetEmissiveFactor)
        .property("emissiveStrength", &csp::systems::GLTFMaterial::GetEmissiveStrength, &csp::systems::GLTFMaterial::SetEmissiveStrength)
        .function(
            "getBaseColorTexture", +[](const csp::systems::GLTFMaterial& self) { return self.GetBaseColorTexture(); })
        .function("setBaseColorTexture(texture)", &csp::systems::GLTFMaterial::SetBaseColorTexture)
        .function(
            "getMetallicRoughnessTexture", +[](const csp::systems::GLTFMaterial& self) { return self.GetMetallicRoughnessTexture(); })
        .function("setMetallicRoughnessTexture(texture)", &csp::systems::GLTFMaterial::SetMetallicRoughnessTexture)
        .function(
            "getNormalTexture", +[](const csp::systems::GLTFMaterial& self) { return self.GetNormalTexture(); })
        .function("setNormalTexture(texture)", &csp::systems::GLTFMaterial::SetNormalTexture)
        .function(
            "getOcclusionTexture", +[](const csp::systems::GLTFMaterial& self) { return self.GetOcclusionTexture(); })
        .function("setOcclusionTexture(texture)", &csp::systems::GLTFMaterial::SetOcclusionTexture)
        .function(
            "getEmissiveTexture", +[](const csp::systems::GLTFMaterial& self) { return self.GetEmissiveTexture(); })
        .function("setEmissiveTexture(texture)", &csp::systems::GLTFMaterial::SetEmissiveTexture);
}
