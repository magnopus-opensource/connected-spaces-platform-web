#include "../../../containers/Map.h"
#include "../../../containers/String.h"

#include "CSP/Common/Map.h"
#include "CSP/Common/String.h"
#include "CSP/Common/Vector.h"
#include "CSP/Multiplayer/Components/StaticModelSpaceComponent.h"

#include "emscripten/bind.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"

EMSCRIPTEN_BINDINGS(CSPStaticModelSpaceComponent)
{
    emscripten::enum_<csp::multiplayer::StaticModelPropertyKeys>("StaticModelPropertyKeys", emscripten::enum_value_type::number)
        .value("ExternalResourceAssetId", csp::multiplayer::StaticModelPropertyKeys::ExternalResourceAssetId)
        .value("ExternalResourceAssetCollectionId", csp::multiplayer::StaticModelPropertyKeys::ExternalResourceAssetCollectionId)
        .value("Position", csp::multiplayer::StaticModelPropertyKeys::Position)
        .value("Rotation", csp::multiplayer::StaticModelPropertyKeys::Rotation)
        .value("Scale", csp::multiplayer::StaticModelPropertyKeys::Scale)
        .value("IsVisible", csp::multiplayer::StaticModelPropertyKeys::IsVisible)
        .value("IsARVisible", csp::multiplayer::StaticModelPropertyKeys::IsARVisible)
        .value("ThirdPartyComponentRef", csp::multiplayer::StaticModelPropertyKeys::ThirdPartyComponentRef)
        .value("IsShadowCaster", csp::multiplayer::StaticModelPropertyKeys::IsShadowCaster)
        .value("MaterialOverrides", csp::multiplayer::StaticModelPropertyKeys::MaterialOverrides)
        .value("IsVirtualVisible", csp::multiplayer::StaticModelPropertyKeys::IsVirtualVisible)
        .value("ShowAsHoldoutInAR", csp::multiplayer::StaticModelPropertyKeys::ShowAsHoldoutInAR)
        .value("ShowAsHoldoutInVirtual", csp::multiplayer::StaticModelPropertyKeys::ShowAsHoldoutInVirtual)
        .value("Num", csp::multiplayer::StaticModelPropertyKeys::Num);

    emscripten::class_<csp::multiplayer::StaticModelSpaceComponent, emscripten::base<csp::multiplayer::ComponentBase>>("StaticModelSpaceComponent")
        .property("externalResourceAssetId", &csp::multiplayer::StaticModelSpaceComponent::GetExternalResourceAssetId,
            &csp::multiplayer::StaticModelSpaceComponent::SetExternalResourceAssetId)
        .property("externalResourceAssetCollectionId", &csp::multiplayer::StaticModelSpaceComponent::GetExternalResourceAssetCollectionId,
            &csp::multiplayer::StaticModelSpaceComponent::SetExternalResourceAssetCollectionId)
        .property("position", &csp::multiplayer::StaticModelSpaceComponent::GetPosition, &csp::multiplayer::StaticModelSpaceComponent::SetPosition)
        .property("rotation", &csp::multiplayer::StaticModelSpaceComponent::GetRotation, &csp::multiplayer::StaticModelSpaceComponent::SetRotation)
        .property("scale", &csp::multiplayer::StaticModelSpaceComponent::GetScale, &csp::multiplayer::StaticModelSpaceComponent::SetScale)
        .property("transform", &csp::multiplayer::StaticModelSpaceComponent::GetTransform, &csp::multiplayer::StaticModelSpaceComponent::SetTransform)
        .property("isVisible", &csp::multiplayer::StaticModelSpaceComponent::GetIsVisible, &csp::multiplayer::StaticModelSpaceComponent::SetIsVisible)
        .property("isARVisible", &csp::multiplayer::StaticModelSpaceComponent::GetIsARVisible, &csp::multiplayer::StaticModelSpaceComponent::SetIsARVisible)
        .property("isVirtualVisible", &csp::multiplayer::StaticModelSpaceComponent::GetIsVirtualVisible, &csp::multiplayer::StaticModelSpaceComponent::SetIsVirtualVisible)
        .property("thirdPartyComponentRef", &csp::multiplayer::StaticModelSpaceComponent::GetThirdPartyComponentRef,
            &csp::multiplayer::StaticModelSpaceComponent::SetThirdPartyComponentRef)
        .property("isShadowCaster", &csp::multiplayer::StaticModelSpaceComponent::GetIsShadowCaster, &csp::multiplayer::StaticModelSpaceComponent::SetIsShadowCaster)
        .property("showAsHoldoutInAR", &csp::multiplayer::StaticModelSpaceComponent::GetShowAsHoldoutInAR, &csp::multiplayer::StaticModelSpaceComponent::SetShowAsHoldoutInAR)
        .property("showAsHoldoutInVirtual", &csp::multiplayer::StaticModelSpaceComponent::GetShowAsHoldoutInVirtual,
            &csp::multiplayer::StaticModelSpaceComponent::SetShowAsHoldoutInVirtual)
        .property("materialOverrides", &csp::multiplayer::StaticModelSpaceComponent::GetMaterialOverrides)
        .function("addMaterialOverride(modelPath, materialAssetId)", &csp::multiplayer::StaticModelSpaceComponent::AddMaterialOverride)
        .function("removeMaterialOverride(modelPath)", &csp::multiplayer::StaticModelSpaceComponent::RemoveMaterialOverride);
}

#pragma clang diagnostic pop
