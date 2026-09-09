#include "../../../containers/String.h"

#include "CSP/Common/String.h"
#include "CSP/Common/Vector.h"
#include "CSP/Multiplayer/Components/FiducialMarkerSpaceComponent.h"

#include "emscripten/bind.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"

EMSCRIPTEN_BINDINGS(CSPFiducialMarkerSpaceComponent)
{
    emscripten::enum_<csp::multiplayer::FiducialMarkerPropertyKeys>("FiducialMarkerPropertyKeys", emscripten::enum_value_type::number)
        .value("Name_DEPRECATED", csp::multiplayer::FiducialMarkerPropertyKeys::Name_DEPRECATED)
        .value("MarkerAssetId", csp::multiplayer::FiducialMarkerPropertyKeys::MarkerAssetId)
        .value("AssetCollectionId", csp::multiplayer::FiducialMarkerPropertyKeys::AssetCollectionId)
        .value("Position", csp::multiplayer::FiducialMarkerPropertyKeys::Position)
        .value("Rotation", csp::multiplayer::FiducialMarkerPropertyKeys::Rotation)
        .value("Scale", csp::multiplayer::FiducialMarkerPropertyKeys::Scale)
        .value("IsVisible", csp::multiplayer::FiducialMarkerPropertyKeys::IsVisible)
        .value("IsARVisible", csp::multiplayer::FiducialMarkerPropertyKeys::IsARVisible)
        .value("IsVirtualVisible", csp::multiplayer::FiducialMarkerPropertyKeys::IsVirtualVisible)
        .value("Num", csp::multiplayer::FiducialMarkerPropertyKeys::Num);

    emscripten::class_<csp::multiplayer::FiducialMarkerSpaceComponent, emscripten::base<csp::multiplayer::ComponentBase>>("FiducialMarkerSpaceComponent")
        .property("name", &csp::multiplayer::FiducialMarkerSpaceComponent::GetName, &csp::multiplayer::FiducialMarkerSpaceComponent::SetName)
        .property("markerAssetId", &csp::multiplayer::FiducialMarkerSpaceComponent::GetMarkerAssetId, &csp::multiplayer::FiducialMarkerSpaceComponent::SetMarkerAssetId)
        .property("assetCollectionId", &csp::multiplayer::FiducialMarkerSpaceComponent::GetAssetCollectionId, &csp::multiplayer::FiducialMarkerSpaceComponent::SetAssetCollectionId)
        .property("position", &csp::multiplayer::FiducialMarkerSpaceComponent::GetPosition, &csp::multiplayer::FiducialMarkerSpaceComponent::SetPosition)
        .property("rotation", &csp::multiplayer::FiducialMarkerSpaceComponent::GetRotation, &csp::multiplayer::FiducialMarkerSpaceComponent::SetRotation)
        .property("scale", &csp::multiplayer::FiducialMarkerSpaceComponent::GetScale, &csp::multiplayer::FiducialMarkerSpaceComponent::SetScale)
        .property("transform", &csp::multiplayer::FiducialMarkerSpaceComponent::GetTransform, &csp::multiplayer::FiducialMarkerSpaceComponent::SetTransform)
        .property("isVisible", &csp::multiplayer::FiducialMarkerSpaceComponent::GetIsVisible, &csp::multiplayer::FiducialMarkerSpaceComponent::SetIsVisible)
        .property("isARVisible", &csp::multiplayer::FiducialMarkerSpaceComponent::GetIsARVisible, &csp::multiplayer::FiducialMarkerSpaceComponent::SetIsARVisible)
        .property("isVirtualVisible", &csp::multiplayer::FiducialMarkerSpaceComponent::GetIsVirtualVisible, &csp::multiplayer::FiducialMarkerSpaceComponent::SetIsVirtualVisible);
}

#pragma clang diagnostic pop
