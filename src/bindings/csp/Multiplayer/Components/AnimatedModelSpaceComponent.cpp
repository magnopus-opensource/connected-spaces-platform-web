#include "../../../containers/Map.h"
#include "../../../containers/String.h"

#include "CSP/Common/Map.h"
#include "CSP/Common/String.h"
#include "CSP/Common/Vector.h"
#include "CSP/Multiplayer/Components/AnimatedModelSpaceComponent.h"

#include "emscripten/bind.h"
#include <cstdint>

// We disagree with the deprecation of one of the properties here (SetExternalResourceAssetId)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"

EMSCRIPTEN_BINDINGS(CSPAnimatedModelSpaceComponent)
{
    emscripten::enum_<csp::multiplayer::AnimatedModelPropertyKeys>("AnimatedModelPropertyKeys", emscripten::enum_value_type::number)
        .value("ExternalResourceAssetId", csp::multiplayer::AnimatedModelPropertyKeys::ExternalResourceAssetId)
        .value("ExternalResourceAssetCollectionId", csp::multiplayer::AnimatedModelPropertyKeys::ExternalResourceAssetCollectionId)
        .value("Position", csp::multiplayer::AnimatedModelPropertyKeys::Position)
        .value("Rotation", csp::multiplayer::AnimatedModelPropertyKeys::Rotation)
        .value("Scale", csp::multiplayer::AnimatedModelPropertyKeys::Scale)
        .value("IsLoopPlayback", csp::multiplayer::AnimatedModelPropertyKeys::IsLoopPlayback)
        .value("IsPlaying", csp::multiplayer::AnimatedModelPropertyKeys::IsPlaying)
        .value("IsVisible", csp::multiplayer::AnimatedModelPropertyKeys::IsVisible)
        .value("AnimationIndex", csp::multiplayer::AnimatedModelPropertyKeys::AnimationIndex)
        .value("IsARVisible", csp::multiplayer::AnimatedModelPropertyKeys::IsARVisible)
        .value("ThirdPartyComponentRef", csp::multiplayer::AnimatedModelPropertyKeys::ThirdPartyComponentRef)
        .value("IsShadowCaster", csp::multiplayer::AnimatedModelPropertyKeys::IsShadowCaster)
        .value("MaterialOverrides", csp::multiplayer::AnimatedModelPropertyKeys::MaterialOverrides)
        .value("IsVirtualVisible", csp::multiplayer::AnimatedModelPropertyKeys::IsVirtualVisible)
        .value("ShowAsHoldoutInAR", csp::multiplayer::AnimatedModelPropertyKeys::ShowAsHoldoutInAR)
        .value("ShowAsHoldoutInVirtual", csp::multiplayer::AnimatedModelPropertyKeys::ShowAsHoldoutInVirtual)
        .value("Num", csp::multiplayer::AnimatedModelPropertyKeys::Num);

    emscripten::class_<csp::multiplayer::AnimatedModelSpaceComponent, emscripten::base<csp::multiplayer::ComponentBase>>("AnimatedModelSpaceComponent")
        .property("externalResourceAssetId", &csp::multiplayer::AnimatedModelSpaceComponent::GetExternalResourceAssetId,
            &csp::multiplayer::AnimatedModelSpaceComponent::SetExternalResourceAssetId)
        .property("externalResourceAssetCollectionId", &csp::multiplayer::AnimatedModelSpaceComponent::GetExternalResourceAssetCollectionId,
            &csp::multiplayer::AnimatedModelSpaceComponent::SetExternalResourceAssetCollectionId)
        .property("position", &csp::multiplayer::AnimatedModelSpaceComponent::GetPosition, &csp::multiplayer::AnimatedModelSpaceComponent::SetPosition)
        .property("rotation", &csp::multiplayer::AnimatedModelSpaceComponent::GetRotation, &csp::multiplayer::AnimatedModelSpaceComponent::SetRotation)
        .property("scale", &csp::multiplayer::AnimatedModelSpaceComponent::GetScale, &csp::multiplayer::AnimatedModelSpaceComponent::SetScale)
        .property("transform", &csp::multiplayer::AnimatedModelSpaceComponent::GetTransform, &csp::multiplayer::AnimatedModelSpaceComponent::SetTransform)
        .property("isLoopPlayback", &csp::multiplayer::AnimatedModelSpaceComponent::GetIsLoopPlayback, &csp::multiplayer::AnimatedModelSpaceComponent::SetIsLoopPlayback)
        .property("isPlaying", &csp::multiplayer::AnimatedModelSpaceComponent::GetIsPlaying, &csp::multiplayer::AnimatedModelSpaceComponent::SetIsPlaying)
        .property("animationIndex", &csp::multiplayer::AnimatedModelSpaceComponent::GetAnimationIndex, &csp::multiplayer::AnimatedModelSpaceComponent::SetAnimationIndex)
        .property("materialOverrides", &csp::multiplayer::AnimatedModelSpaceComponent::GetMaterialOverrides)
        .property("isVisible", &csp::multiplayer::AnimatedModelSpaceComponent::GetIsVisible, &csp::multiplayer::AnimatedModelSpaceComponent::SetIsVisible)
        .property("isARVisible", &csp::multiplayer::AnimatedModelSpaceComponent::GetIsARVisible, &csp::multiplayer::AnimatedModelSpaceComponent::SetIsARVisible)
        .property("isVirtualVisible", &csp::multiplayer::AnimatedModelSpaceComponent::GetIsVirtualVisible, &csp::multiplayer::AnimatedModelSpaceComponent::SetIsVirtualVisible)
        .property("thirdPartyComponentRef", &csp::multiplayer::AnimatedModelSpaceComponent::GetThirdPartyComponentRef,
            &csp::multiplayer::AnimatedModelSpaceComponent::SetThirdPartyComponentRef)
        .property("isShadowCaster", &csp::multiplayer::AnimatedModelSpaceComponent::GetIsShadowCaster, &csp::multiplayer::AnimatedModelSpaceComponent::SetIsShadowCaster)
        .property("showAsHoldoutInAR", &csp::multiplayer::AnimatedModelSpaceComponent::GetShowAsHoldoutInAR, &csp::multiplayer::AnimatedModelSpaceComponent::SetShowAsHoldoutInAR)
        .property("showAsHoldoutInVirtual", &csp::multiplayer::AnimatedModelSpaceComponent::GetShowAsHoldoutInVirtual,
            &csp::multiplayer::AnimatedModelSpaceComponent::SetShowAsHoldoutInVirtual)
        .function("addMaterialOverride(modelPath, materialAssetId)", &csp::multiplayer::AnimatedModelSpaceComponent::AddMaterialOverride)
        .function("removeMaterialOverride(modelPath)", &csp::multiplayer::AnimatedModelSpaceComponent::RemoveMaterialOverride);
}

#pragma clang diagnostic pop
