#include "../../../containers/Array.h"
#include "../../../containers/Disposal.h"
#include "../../../containers/String.h"
#include "../../../utils/JSDisposable.h"
#include "../../PointerDeclarations.h"

#include "CSP/Common/Array.h"
#include "CSP/Common/String.h"
#include "CSP/Systems/Assets/Material.h"
#include "CSP/Systems/WebService.h"

#include "emscripten/bind.h"
#include "emscripten/val.h"
#include <cstddef>

EMSCRIPTEN_DECLARE_VAL_TYPE(OwnedMaterialArray);

EMSCRIPTEN_BINDINGS(CSPMaterial)
{
    // Unique to materials, since they are the only place we have owning pointers returned from the
    // API, along OwnedMaterialPointer which is shared with the asset system.
    emscripten::register_type<OwnedMaterialArray>("(Material[] & Disposable)");

    emscripten::enum_<csp::systems::EShaderType>("EShaderType", emscripten::enum_value_type::number)
        .value("Standard", csp::systems::EShaderType::Standard)
        .value("AlphaVideo", csp::systems::EShaderType::AlphaVideo);

    emscripten::enum_<csp::systems::EAlphaMode>("EAlphaMode", emscripten::enum_value_type::number)
        .value("Opaque", csp::systems::EAlphaMode::Opaque)
        .value("Mask", csp::systems::EAlphaMode::Mask)
        .value("Blend", csp::systems::EAlphaMode::Blend);

    emscripten::enum_<csp::systems::EBlendMode>("EBlendMode", emscripten::enum_value_type::number)
        .value("Normal", csp::systems::EBlendMode::Normal)
        .value("Additive", csp::systems::EBlendMode::Additive);

    emscripten::enum_<csp::systems::EColorChannel>("EColorChannel", emscripten::enum_value_type::number)
        .value("R", csp::systems::EColorChannel::R)
        .value("G", csp::systems::EColorChannel::G)
        .value("B", csp::systems::EColorChannel::B)
        .value("A", csp::systems::EColorChannel::A);

    emscripten::class_<csp::systems::Material>("Material")
        .property("name", &csp::systems::Material::GetName)
        .property("shaderType", &csp::systems::Material::GetShaderType)
        .property("version", &csp::systems::Material::GetVersion)
        .property("materialCollectionId", &csp::systems::Material::GetMaterialCollectionId)
        .property("materialId", &csp::systems::Material::GetMaterialId);

    emscripten::class_<csp::systems::MaterialResult, emscripten::base<csp::systems::ResultBase>>("MaterialResult")
        .function(
            "getMaterial",
            +[](csp::systems::MaterialResult& self) { return OwnedMaterialPointer { emscripten::val(self.GetMaterial(), emscripten::return_value_policy::take_ownership()) }; });

    emscripten::class_<csp::systems::MaterialsResult, emscripten::base<csp::systems::ResultBase>>("MaterialsResult")
        .function(
            "getMaterials", +[](csp::systems::MaterialsResult& self) {
                csp::common::Array<csp::systems::Material*>* materials = self.GetMaterials();
                emscripten::val jsArray = emscripten::val::array();

                if (materials != nullptr) {
                    for (std::size_t i = 0; i < materials->Size(); ++i) {
                        jsArray.set(i, emscripten::val((*materials)[i], emscripten::return_value_policy::take_ownership()));
                    }
                }

                bindings::containers::attach_disposer_to_container(jsArray.as_handle());

                return OwnedMaterialArray { jsArray };
            });

    emscripten::class_<csp::systems::MaterialInfo>("MaterialInfo")
        .class_function(
            "create", +[]() { return csp::systems::MaterialInfo(); })
        .property("uri", &csp::systems::MaterialInfo::Uri)
        .property("shaderType", &csp::systems::MaterialInfo::ShaderType)
        .property("materialCollectionId", &csp::systems::MaterialInfo::MaterialCollectionId)
        .property("materialId", &csp::systems::MaterialInfo::MaterialId)
        .function("equals(other)", &csp::systems::MaterialInfo::operator==);

    emscripten::class_<csp::systems::MaterialInfoResult, emscripten::base<csp::systems::ResultBase>>("MaterialInfoResult")
        .function(
            "getMaterialInfo", +[](const csp::systems::MaterialInfoResult& self) { return self.GetMaterialInfo(); });

    emscripten::class_<csp::systems::MaterialInfosResult, emscripten::base<csp::systems::ResultBase>>("MaterialInfosResult")
        .function(
            "getMaterialInfos", +[](const csp::systems::MaterialInfosResult& self) {
                return bindings::utils::JSDisposable<csp::common::Array<csp::systems::MaterialInfo>> { self.GetMaterialInfos() };
            });
}
