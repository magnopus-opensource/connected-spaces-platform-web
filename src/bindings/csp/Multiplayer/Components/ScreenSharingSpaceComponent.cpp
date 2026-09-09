#include "../../../containers/String.h"

#include "CSP/Common/String.h"
#include "CSP/Common/Vector.h"
#include "CSP/Multiplayer/Components/ScreenSharingSpaceComponent.h"

#include "emscripten/bind.h"

EMSCRIPTEN_BINDINGS(CSPScreenSharingSpaceComponent)
{
    emscripten::enum_<csp::multiplayer::ScreenSharingPropertyKeys>("ScreenSharingPropertyKeys", emscripten::enum_value_type::number)
        .value("Position", csp::multiplayer::ScreenSharingPropertyKeys::Position)
        .value("Rotation", csp::multiplayer::ScreenSharingPropertyKeys::Rotation)
        .value("Scale", csp::multiplayer::ScreenSharingPropertyKeys::Scale)
        .value("IsVisible", csp::multiplayer::ScreenSharingPropertyKeys::IsVisible)
        .value("IsARVisible", csp::multiplayer::ScreenSharingPropertyKeys::IsARVisible)
        .value("IsShadowCaster", csp::multiplayer::ScreenSharingPropertyKeys::IsShadowCaster)
        .value("UserId", csp::multiplayer::ScreenSharingPropertyKeys::UserId)
        .value("DefaultImageCollectionId", csp::multiplayer::ScreenSharingPropertyKeys::DefaultImageCollectionId)
        .value("DefaultImageAssetId", csp::multiplayer::ScreenSharingPropertyKeys::DefaultImageAssetId)
        .value("AttenuationRadius", csp::multiplayer::ScreenSharingPropertyKeys::AttenuationRadius)
        .value("IsVirtualVisible", csp::multiplayer::ScreenSharingPropertyKeys::IsVirtualVisible)
        .value("Num", csp::multiplayer::ScreenSharingPropertyKeys::Num);

    emscripten::class_<csp::multiplayer::ScreenSharingSpaceComponent, emscripten::base<csp::multiplayer::ComponentBase>>("ScreenSharingSpaceComponent")
        .property("userId", &csp::multiplayer::ScreenSharingSpaceComponent::GetUserId, &csp::multiplayer::ScreenSharingSpaceComponent::SetUserId)
        .property("defaultImageCollectionId", &csp::multiplayer::ScreenSharingSpaceComponent::GetDefaultImageCollectionId,
            &csp::multiplayer::ScreenSharingSpaceComponent::SetDefaultImageCollectionId)
        .property(
            "defaultImageAssetId", &csp::multiplayer::ScreenSharingSpaceComponent::GetDefaultImageAssetId, &csp::multiplayer::ScreenSharingSpaceComponent::SetDefaultImageAssetId)
        .property("attenuationRadius", &csp::multiplayer::ScreenSharingSpaceComponent::GetAttenuationRadius, &csp::multiplayer::ScreenSharingSpaceComponent::SetAttenuationRadius)
        .property("position", &csp::multiplayer::ScreenSharingSpaceComponent::GetPosition, &csp::multiplayer::ScreenSharingSpaceComponent::SetPosition)
        .property("rotation", &csp::multiplayer::ScreenSharingSpaceComponent::GetRotation, &csp::multiplayer::ScreenSharingSpaceComponent::SetRotation)
        .property("scale", &csp::multiplayer::ScreenSharingSpaceComponent::GetScale, &csp::multiplayer::ScreenSharingSpaceComponent::SetScale)
        .property("transform", &csp::multiplayer::ScreenSharingSpaceComponent::GetTransform, &csp::multiplayer::ScreenSharingSpaceComponent::SetTransform)
        .property("isVisible", &csp::multiplayer::ScreenSharingSpaceComponent::GetIsVisible, &csp::multiplayer::ScreenSharingSpaceComponent::SetIsVisible)
        .property("isARVisible", &csp::multiplayer::ScreenSharingSpaceComponent::GetIsARVisible, &csp::multiplayer::ScreenSharingSpaceComponent::SetIsARVisible)
        .property("isVirtualVisible", &csp::multiplayer::ScreenSharingSpaceComponent::GetIsVirtualVisible, &csp::multiplayer::ScreenSharingSpaceComponent::SetIsVirtualVisible)
        .property("isShadowCaster", &csp::multiplayer::ScreenSharingSpaceComponent::GetIsShadowCaster, &csp::multiplayer::ScreenSharingSpaceComponent::SetIsShadowCaster);
}
