#include "../../../containers/String.h"

#include "CSP/Common/String.h"
#include "CSP/Common/Vector.h"
#include "CSP/Multiplayer/Components/GaussianSplatSpaceComponent.h"

#include "emscripten/bind.h"
#include <CSP/Multiplayer/Components/Interfaces/IShadowCasterComponent.h>

EMSCRIPTEN_BINDINGS(CSPGaussianSplatSpaceComponent)
{
    emscripten::enum_<csp::multiplayer::GaussianSplatPropertyKeys>("GaussianSplatPropertyKeys", emscripten::enum_value_type::number)
        .value("ExternalResourceAssetId", csp::multiplayer::GaussianSplatPropertyKeys::ExternalResourceAssetId)
        .value("ExternalResourceAssetCollectionId", csp::multiplayer::GaussianSplatPropertyKeys::ExternalResourceAssetCollectionId)
        .value("Position", csp::multiplayer::GaussianSplatPropertyKeys::Position)
        .value("Rotation", csp::multiplayer::GaussianSplatPropertyKeys::Rotation)
        .value("Scale", csp::multiplayer::GaussianSplatPropertyKeys::Scale)
        .value("IsVisible", csp::multiplayer::GaussianSplatPropertyKeys::IsVisible)
        .value("IsARVisible", csp::multiplayer::GaussianSplatPropertyKeys::IsARVisible)
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
        // This calls through the the underlying interface specifically because the concrete type has shadowing methods (deprecated) of the same name
        .property("isShadowCaster", &csp::multiplayer::IShadowCasterComponent::GetIsShadowCaster, &csp::multiplayer::IShadowCasterComponent::SetIsShadowCaster)
        .property("tint", &csp::multiplayer::GaussianSplatSpaceComponent::GetTint, &csp::multiplayer::GaussianSplatSpaceComponent::SetTint);
}
