#include "../../../containers/String.h"

#include "CSP/Common/String.h"
#include "CSP/Common/Vector.h"
#include "CSP/Multiplayer/Components/ButtonSpaceComponent.h"

#include "emscripten/bind.h"

EMSCRIPTEN_BINDINGS(CSPButtonSpaceComponent)
{
    emscripten::enum_<csp::multiplayer::ButtonPropertyKeys>("ButtonPropertyKeys", emscripten::enum_value_type::number)
        .value("Name_DEPRECATED", csp::multiplayer::ButtonPropertyKeys::Name_DEPRECATED)
        .value("LabelText", csp::multiplayer::ButtonPropertyKeys::LabelText)
        .value("IconAssetId", csp::multiplayer::ButtonPropertyKeys::IconAssetId)
        .value("AssetCollectionId", csp::multiplayer::ButtonPropertyKeys::AssetCollectionId)
        .value("Position", csp::multiplayer::ButtonPropertyKeys::Position)
        .value("Rotation", csp::multiplayer::ButtonPropertyKeys::Rotation)
        .value("Scale", csp::multiplayer::ButtonPropertyKeys::Scale)
        .value("IsVisible", csp::multiplayer::ButtonPropertyKeys::IsVisible)
        .value("IsEnabled", csp::multiplayer::ButtonPropertyKeys::IsEnabled)
        .value("IsARVisible", csp::multiplayer::ButtonPropertyKeys::IsARVisible)
        .value("IsVirtualVisible", csp::multiplayer::ButtonPropertyKeys::IsVirtualVisible)
        .value("Num", csp::multiplayer::ButtonPropertyKeys::Num);

    emscripten::class_<csp::multiplayer::ButtonSpaceComponent, emscripten::base<csp::multiplayer::ComponentBase>>("ButtonSpaceComponent")
        .property("labelText", &csp::multiplayer::ButtonSpaceComponent::GetLabelText, &csp::multiplayer::ButtonSpaceComponent::SetLabelText)
        .property("iconAssetId", &csp::multiplayer::ButtonSpaceComponent::GetIconAssetId, &csp::multiplayer::ButtonSpaceComponent::SetIconAssetId)
        .property("assetCollectionId", &csp::multiplayer::ButtonSpaceComponent::GetAssetCollectionId, &csp::multiplayer::ButtonSpaceComponent::SetAssetCollectionId)
        .property("position", &csp::multiplayer::ButtonSpaceComponent::GetPosition, &csp::multiplayer::ButtonSpaceComponent::SetPosition)
        .property("rotation", &csp::multiplayer::ButtonSpaceComponent::GetRotation, &csp::multiplayer::ButtonSpaceComponent::SetRotation)
        .property("scale", &csp::multiplayer::ButtonSpaceComponent::GetScale, &csp::multiplayer::ButtonSpaceComponent::SetScale)
        .property("transform", &csp::multiplayer::ButtonSpaceComponent::GetTransform, &csp::multiplayer::ButtonSpaceComponent::SetTransform)
        .property("isEnabled", &csp::multiplayer::ButtonSpaceComponent::GetIsEnabled, &csp::multiplayer::ButtonSpaceComponent::SetIsEnabled)
        .property("isVisible", &csp::multiplayer::ButtonSpaceComponent::GetIsVisible, &csp::multiplayer::ButtonSpaceComponent::SetIsVisible)
        .property("isARVisible", &csp::multiplayer::ButtonSpaceComponent::GetIsARVisible, &csp::multiplayer::ButtonSpaceComponent::SetIsARVisible)
        .property("isVirtualVisible", &csp::multiplayer::ButtonSpaceComponent::GetIsVirtualVisible, &csp::multiplayer::ButtonSpaceComponent::SetIsVirtualVisible);
}
