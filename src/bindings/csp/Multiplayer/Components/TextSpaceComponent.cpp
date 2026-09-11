#include "../../../containers/String.h"

#include "CSP/Common/String.h"
#include "CSP/Common/Vector.h"
#include "CSP/Multiplayer/Components/TextSpaceComponent.h"

#include "emscripten/bind.h"

EMSCRIPTEN_BINDINGS(CSPTextSpaceComponent)
{
    emscripten::enum_<csp::multiplayer::TextPropertyKeys>("TextPropertyKeys", emscripten::enum_value_type::number)
        .value("Position", csp::multiplayer::TextPropertyKeys::Position)
        .value("Rotation", csp::multiplayer::TextPropertyKeys::Rotation)
        .value("Scale", csp::multiplayer::TextPropertyKeys::Scale)
        .value("Text", csp::multiplayer::TextPropertyKeys::Text)
        .value("TextColor", csp::multiplayer::TextPropertyKeys::TextColor)
        .value("BackgroundColor", csp::multiplayer::TextPropertyKeys::BackgroundColor)
        .value("IsBackgroundVisible", csp::multiplayer::TextPropertyKeys::IsBackgroundVisible)
        .value("Width", csp::multiplayer::TextPropertyKeys::Width)
        .value("Height", csp::multiplayer::TextPropertyKeys::Height)
        .value("BillboardMode", csp::multiplayer::TextPropertyKeys::BillboardMode)
        .value("IsVisible", csp::multiplayer::TextPropertyKeys::IsVisible)
        .value("IsARVisible", csp::multiplayer::TextPropertyKeys::IsARVisible)
        .value("IsVirtualVisible", csp::multiplayer::TextPropertyKeys::IsVirtualVisible)
        .value("Num", csp::multiplayer::TextPropertyKeys::Num);

    emscripten::class_<csp::multiplayer::TextSpaceComponent, emscripten::base<csp::multiplayer::ComponentBase>>("TextSpaceComponent")
        .property("text", &csp::multiplayer::TextSpaceComponent::GetText, &csp::multiplayer::TextSpaceComponent::SetText)
        .property("textColor", &csp::multiplayer::TextSpaceComponent::GetTextColor, &csp::multiplayer::TextSpaceComponent::SetTextColor)
        .property("backgroundColor", &csp::multiplayer::TextSpaceComponent::GetBackgroundColor, &csp::multiplayer::TextSpaceComponent::SetBackgroundColor)
        .property("isBackgroundVisible", &csp::multiplayer::TextSpaceComponent::GetIsBackgroundVisible, &csp::multiplayer::TextSpaceComponent::SetIsBackgroundVisible)
        .property("width", &csp::multiplayer::TextSpaceComponent::GetWidth, &csp::multiplayer::TextSpaceComponent::SetWidth)
        .property("height", &csp::multiplayer::TextSpaceComponent::GetHeight, &csp::multiplayer::TextSpaceComponent::SetHeight)
        .property("position", &csp::multiplayer::TextSpaceComponent::GetPosition, &csp::multiplayer::TextSpaceComponent::SetPosition)
        .property("rotation", &csp::multiplayer::TextSpaceComponent::GetRotation, &csp::multiplayer::TextSpaceComponent::SetRotation)
        .property("scale", &csp::multiplayer::TextSpaceComponent::GetScale, &csp::multiplayer::TextSpaceComponent::SetScale)
        .property("transform", &csp::multiplayer::TextSpaceComponent::GetTransform, &csp::multiplayer::TextSpaceComponent::SetTransform)
        .property("billboardMode", &csp::multiplayer::TextSpaceComponent::GetBillboardMode, &csp::multiplayer::TextSpaceComponent::SetBillboardMode)
        .property("isVisible", &csp::multiplayer::TextSpaceComponent::GetIsVisible, &csp::multiplayer::TextSpaceComponent::SetIsVisible)
        .property("isARVisible", &csp::multiplayer::TextSpaceComponent::GetIsARVisible, &csp::multiplayer::TextSpaceComponent::SetIsARVisible)
        .property("isVirtualVisible", &csp::multiplayer::TextSpaceComponent::GetIsVirtualVisible, &csp::multiplayer::TextSpaceComponent::SetIsVirtualVisible);
}
