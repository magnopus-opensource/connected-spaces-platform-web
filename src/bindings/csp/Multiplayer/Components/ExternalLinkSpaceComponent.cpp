#include "../../../containers/String.h"

#include "CSP/Common/String.h"
#include "CSP/Common/Vector.h"
#include "CSP/Multiplayer/Components/ExternalLinkSpaceComponent.h"

#include "emscripten/bind.h"

EMSCRIPTEN_BINDINGS(CSPExternalLinkSpaceComponent)
{
    emscripten::enum_<csp::multiplayer::ExternalLinkActions>("ExternalLinkActions", emscripten::enum_value_type::number)
        .value("LinkClicked", csp::multiplayer::ExternalLinkActions::LinkClicked)
        .value("Num", csp::multiplayer::ExternalLinkActions::Num);

    emscripten::enum_<csp::multiplayer::ExternalLinkPropertyKeys>("ExternalLinkPropertyKeys", emscripten::enum_value_type::number)
        .value("LinkUrl", csp::multiplayer::ExternalLinkPropertyKeys::LinkUrl)
        .value("Position", csp::multiplayer::ExternalLinkPropertyKeys::Position)
        .value("Rotation", csp::multiplayer::ExternalLinkPropertyKeys::Rotation)
        .value("Scale", csp::multiplayer::ExternalLinkPropertyKeys::Scale)
        .value("DisplayText", csp::multiplayer::ExternalLinkPropertyKeys::DisplayText)
        .value("IsEnabled", csp::multiplayer::ExternalLinkPropertyKeys::IsEnabled)
        .value("IsVisible", csp::multiplayer::ExternalLinkPropertyKeys::IsVisible)
        .value("IsARVisible", csp::multiplayer::ExternalLinkPropertyKeys::IsARVisible)
        .value("IsVirtualVisible", csp::multiplayer::ExternalLinkPropertyKeys::IsVirtualVisible)
        .value("Num", csp::multiplayer::ExternalLinkPropertyKeys::Num);

    emscripten::class_<csp::multiplayer::ExternalLinkSpaceComponent, emscripten::base<csp::multiplayer::ComponentBase>>("ExternalLinkSpaceComponent")
        .property("linkUrl", &csp::multiplayer::ExternalLinkSpaceComponent::GetLinkUrl, &csp::multiplayer::ExternalLinkSpaceComponent::SetLinkUrl)
        .property("displayText", &csp::multiplayer::ExternalLinkSpaceComponent::GetDisplayText, &csp::multiplayer::ExternalLinkSpaceComponent::SetDisplayText)
        .property("position", &csp::multiplayer::ExternalLinkSpaceComponent::GetPosition, &csp::multiplayer::ExternalLinkSpaceComponent::SetPosition)
        .property("rotation", &csp::multiplayer::ExternalLinkSpaceComponent::GetRotation, &csp::multiplayer::ExternalLinkSpaceComponent::SetRotation)
        .property("scale", &csp::multiplayer::ExternalLinkSpaceComponent::GetScale, &csp::multiplayer::ExternalLinkSpaceComponent::SetScale)
        .property("transform", &csp::multiplayer::ExternalLinkSpaceComponent::GetTransform, &csp::multiplayer::ExternalLinkSpaceComponent::SetTransform)
        .property("isEnabled", &csp::multiplayer::ExternalLinkSpaceComponent::GetIsEnabled, &csp::multiplayer::ExternalLinkSpaceComponent::SetIsEnabled)
        .property("isVisible", &csp::multiplayer::ExternalLinkSpaceComponent::GetIsVisible, &csp::multiplayer::ExternalLinkSpaceComponent::SetIsVisible)
        .property("isARVisible", &csp::multiplayer::ExternalLinkSpaceComponent::GetIsARVisible, &csp::multiplayer::ExternalLinkSpaceComponent::SetIsARVisible)
        .property("isVirtualVisible", &csp::multiplayer::ExternalLinkSpaceComponent::GetIsVirtualVisible, &csp::multiplayer::ExternalLinkSpaceComponent::SetIsVirtualVisible);
}
