#include "../../../containers/String.h"

#include "CSP/Common/String.h"
#include "CSP/Common/Vector.h"
#include "CSP/Multiplayer/Components/GaussianSplatSpaceComponent.h"

#include "emscripten/bind.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"

EMSCRIPTEN_BINDINGS(CSPGaussianSplatSpaceComponent)
{
    emscripten::enum_<csp::multiplayer::GaussianSplatPropertyKeys>("GaussianSplatPropertyKeys", emscripten::enum_value_type::number)
        .value("Name_DEPRECATED", csp::multiplayer::GaussianSplatPropertyKeys::Name_DEPRECATED)
        .value("ExternalResourceAssetId", csp::multiplayer::GaussianSplatPropertyKeys::ExternalResourceAssetId)
        .value("ExternalResourceAssetCollectionId", csp::multiplayer::GaussianSplatPropertyKeys::ExternalResourceAssetCollectionId)
        .value("Position", csp::multiplayer::GaussianSplatPropertyKeys::Position)
        .value("Rotation", csp::multiplayer::GaussianSplatPropertyKeys::Rotation)
        .value("Scale", csp::multiplayer::GaussianSplatPropertyKeys::Scale)
        .value("IsVisible", csp::multiplayer::GaussianSplatPropertyKeys::IsVisible)
        .value("IsARVisible", csp::multiplayer::GaussianSplatPropertyKeys::IsARVisible)
        .value("IsShadowCaster_DEPRECATED", csp::multiplayer::GaussianSplatPropertyKeys::IsShadowCaster_DEPRECATED)
        .value("Tint", csp::multiplayer::GaussianSplatPropertyKeys::Tint)
        .value("IsVirtualVisible", csp::multiplayer::GaussianSplatPropertyKeys::IsVirtualVisible)
        .value("Num", csp::multiplayer::GaussianSplatPropertyKeys::Num);

    emscripten::class_<csp::multiplayer::GaussianSplatSpaceComponent, emscripten::base<csp::multiplayer::ComponentBase>>("GaussianSplatSpaceComponent")
        .property("externalResourceAssetId", &csp::multiplayer::GaussianSplatSpaceComponent::GetExternalResourceAssetId,
            &csp::multiplayer::GaussianSplatSpaceComponent::SetExternalResourceAssetId)
        .property("externalResourceAssetCollectionId", &csp::multiplayer::GaussianSplatSpaceComponent::GetExternalResourceAssetCollectionId,
            &csp::multiplayer::GaussianSplatSpaceComponent::SetExternalResourceAssetCollectionId)
        .property("position", &csp::multiplayer::GaussianSplatSpaceComponent::GetPosition, &csp::multiplayer::GaussianSplatSpaceComponent::SetPosition)
        .property("rotation", &csp::multiplayer::GaussianSplatSpaceComponent::GetRotation, &csp::multiplayer::GaussianSplatSpaceComponent::SetRotation)
        .property("scale", &csp::multiplayer::GaussianSplatSpaceComponent::GetScale, &csp::multiplayer::GaussianSplatSpaceComponent::SetScale)
        .property("transform", &csp::multiplayer::GaussianSplatSpaceComponent::GetTransform, &csp::multiplayer::GaussianSplatSpaceComponent::SetTransform)
        .property("isVisible", &csp::multiplayer::GaussianSplatSpaceComponent::GetIsVisible, &csp::multiplayer::GaussianSplatSpaceComponent::SetIsVisible)
        .property("isARVisible", &csp::multiplayer::GaussianSplatSpaceComponent::GetIsARVisible, &csp::multiplayer::GaussianSplatSpaceComponent::SetIsARVisible)
        .property("isVirtualVisible", &csp::multiplayer::GaussianSplatSpaceComponent::GetIsVirtualVisible, &csp::multiplayer::GaussianSplatSpaceComponent::SetIsVirtualVisible)
        .property("isShadowCaster", &csp::multiplayer::GaussianSplatSpaceComponent::GetIsShadowCaster, &csp::multiplayer::GaussianSplatSpaceComponent::SetIsShadowCaster)
        .property("tint", &csp::multiplayer::GaussianSplatSpaceComponent::GetTint, &csp::multiplayer::GaussianSplatSpaceComponent::SetTint);
}

#pragma clang diagnostic pop
