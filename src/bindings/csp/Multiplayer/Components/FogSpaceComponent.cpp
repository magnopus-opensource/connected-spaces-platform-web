#include "../../../containers/String.h"

#include "CSP/Common/String.h"
#include "CSP/Common/Vector.h"
#include "CSP/Multiplayer/Components/FogSpaceComponent.h"

#include "emscripten/bind.h"

EMSCRIPTEN_BINDINGS(CSPFogSpaceComponent)
{
    emscripten::enum_<csp::multiplayer::FogPropertyKeys>("FogPropertyKeys", emscripten::enum_value_type::number)
        .value("FogMode", csp::multiplayer::FogPropertyKeys::FogMode)
        .value("Position", csp::multiplayer::FogPropertyKeys::Position)
        .value("Rotation", csp::multiplayer::FogPropertyKeys::Rotation)
        .value("Scale", csp::multiplayer::FogPropertyKeys::Scale)
        .value("StartDistance", csp::multiplayer::FogPropertyKeys::StartDistance)
        .value("EndDistance", csp::multiplayer::FogPropertyKeys::EndDistance)
        .value("Color", csp::multiplayer::FogPropertyKeys::Color)
        .value("Density", csp::multiplayer::FogPropertyKeys::Density)
        .value("HeightFalloff", csp::multiplayer::FogPropertyKeys::HeightFalloff)
        .value("MaxOpacity", csp::multiplayer::FogPropertyKeys::MaxOpacity)
        .value("IsVolumetric", csp::multiplayer::FogPropertyKeys::IsVolumetric)
        .value("IsVisible", csp::multiplayer::FogPropertyKeys::IsVisible)
        .value("IsARVisible", csp::multiplayer::FogPropertyKeys::IsARVisible)
        .value("ThirdPartyComponentRef", csp::multiplayer::FogPropertyKeys::ThirdPartyComponentRef)
        .value("IsVirtualVisible", csp::multiplayer::FogPropertyKeys::IsVirtualVisible)
        .value("Num", csp::multiplayer::FogPropertyKeys::Num);

    emscripten::enum_<csp::multiplayer::FogMode>("FogMode", emscripten::enum_value_type::number)
        .value("Linear", csp::multiplayer::FogMode::Linear)
        .value("Exponential", csp::multiplayer::FogMode::Exponential)
        .value("Exponential2", csp::multiplayer::FogMode::Exponential2);

    emscripten::class_<csp::multiplayer::FogSpaceComponent, emscripten::base<csp::multiplayer::ComponentBase>>("FogSpaceComponent")
        .property("fogMode", &csp::multiplayer::FogSpaceComponent::GetFogMode, &csp::multiplayer::FogSpaceComponent::SetFogMode)
        .property("position", &csp::multiplayer::FogSpaceComponent::GetPosition, &csp::multiplayer::FogSpaceComponent::SetPosition)
        .property("rotation", &csp::multiplayer::FogSpaceComponent::GetRotation, &csp::multiplayer::FogSpaceComponent::SetRotation)
        .property("scale", &csp::multiplayer::FogSpaceComponent::GetScale, &csp::multiplayer::FogSpaceComponent::SetScale)
        .property("transform", &csp::multiplayer::FogSpaceComponent::GetTransform, &csp::multiplayer::FogSpaceComponent::SetTransform)
        .property("startDistance", &csp::multiplayer::FogSpaceComponent::GetStartDistance, &csp::multiplayer::FogSpaceComponent::SetStartDistance)
        .property("endDistance", &csp::multiplayer::FogSpaceComponent::GetEndDistance, &csp::multiplayer::FogSpaceComponent::SetEndDistance)
        .property("color", &csp::multiplayer::FogSpaceComponent::GetColor, &csp::multiplayer::FogSpaceComponent::SetColor)
        .property("density", &csp::multiplayer::FogSpaceComponent::GetDensity, &csp::multiplayer::FogSpaceComponent::SetDensity)
        .property("heightFalloff", &csp::multiplayer::FogSpaceComponent::GetHeightFalloff, &csp::multiplayer::FogSpaceComponent::SetHeightFalloff)
        .property("maxOpacity", &csp::multiplayer::FogSpaceComponent::GetMaxOpacity, &csp::multiplayer::FogSpaceComponent::SetMaxOpacity)
        .property("isVolumetric", &csp::multiplayer::FogSpaceComponent::GetIsVolumetric, &csp::multiplayer::FogSpaceComponent::SetIsVolumetric)
        .property("isVisible", &csp::multiplayer::FogSpaceComponent::GetIsVisible, &csp::multiplayer::FogSpaceComponent::SetIsVisible)
        .property("isARVisible", &csp::multiplayer::FogSpaceComponent::GetIsARVisible, &csp::multiplayer::FogSpaceComponent::SetIsARVisible)
        .property("isVirtualVisible", &csp::multiplayer::FogSpaceComponent::GetIsVirtualVisible, &csp::multiplayer::FogSpaceComponent::SetIsVirtualVisible)
        .property("thirdPartyComponentRef", &csp::multiplayer::FogSpaceComponent::GetThirdPartyComponentRef, &csp::multiplayer::FogSpaceComponent::SetThirdPartyComponentRef);
}
