#include "../../../containers/String.h"

#include "CSP/Common/String.h"
#include "CSP/Common/Vector.h"
#include "CSP/Multiplayer/Components/ImageSpaceComponent.h"

#include "emscripten/bind.h"

EMSCRIPTEN_BINDINGS(CSPImageSpaceComponent)
{
    emscripten::enum_<csp::multiplayer::ImagePropertyKeys>("ImagePropertyKeys", emscripten::enum_value_type::number)
        .value("ImageAssetId", csp::multiplayer::ImagePropertyKeys::ImageAssetId)
        .value("AssetCollectionId", csp::multiplayer::ImagePropertyKeys::AssetCollectionId)
        .value("Position", csp::multiplayer::ImagePropertyKeys::Position)
        .value("Rotation", csp::multiplayer::ImagePropertyKeys::Rotation)
        .value("Scale", csp::multiplayer::ImagePropertyKeys::Scale)
        .value("IsVisible", csp::multiplayer::ImagePropertyKeys::IsVisible)
        .value("BillboardMode", csp::multiplayer::ImagePropertyKeys::BillboardMode)
        .value("DisplayMode", csp::multiplayer::ImagePropertyKeys::DisplayMode)
        .value("IsARVisible", csp::multiplayer::ImagePropertyKeys::IsARVisible)
        .value("IsEmissive", csp::multiplayer::ImagePropertyKeys::IsEmissive)
        .value("IsVirtualVisible", csp::multiplayer::ImagePropertyKeys::IsVirtualVisible)
        .value("Num", csp::multiplayer::ImagePropertyKeys::Num);

    emscripten::enum_<csp::multiplayer::DisplayMode>("DisplayMode", emscripten::enum_value_type::number)
        .value("SingleSided", csp::multiplayer::DisplayMode::SingleSided)
        .value("DoubleSided", csp::multiplayer::DisplayMode::DoubleSided)
        .value("DoubleSidedReversed", csp::multiplayer::DisplayMode::DoubleSidedReversed);

    emscripten::class_<csp::multiplayer::ImageSpaceComponent, emscripten::base<csp::multiplayer::ComponentBase>>("ImageSpaceComponent")
        .property("imageAssetId", &csp::multiplayer::ImageSpaceComponent::GetImageAssetId, &csp::multiplayer::ImageSpaceComponent::SetImageAssetId)
        .property("assetCollectionId", &csp::multiplayer::ImageSpaceComponent::GetAssetCollectionId, &csp::multiplayer::ImageSpaceComponent::SetAssetCollectionId)
        .property("position", &csp::multiplayer::ImageSpaceComponent::GetPosition, &csp::multiplayer::ImageSpaceComponent::SetPosition)
        .property("rotation", &csp::multiplayer::ImageSpaceComponent::GetRotation, &csp::multiplayer::ImageSpaceComponent::SetRotation)
        .property("scale", &csp::multiplayer::ImageSpaceComponent::GetScale, &csp::multiplayer::ImageSpaceComponent::SetScale)
        .property("transform", &csp::multiplayer::ImageSpaceComponent::GetTransform, &csp::multiplayer::ImageSpaceComponent::SetTransform)
        .property("billboardMode", &csp::multiplayer::ImageSpaceComponent::GetBillboardMode, &csp::multiplayer::ImageSpaceComponent::SetBillboardMode)
        .property("displayMode", &csp::multiplayer::ImageSpaceComponent::GetDisplayMode, &csp::multiplayer::ImageSpaceComponent::SetDisplayMode)
        .property("isEmissive", &csp::multiplayer::ImageSpaceComponent::GetIsEmissive, &csp::multiplayer::ImageSpaceComponent::SetIsEmissive)
        .property("isVisible", &csp::multiplayer::ImageSpaceComponent::GetIsVisible, &csp::multiplayer::ImageSpaceComponent::SetIsVisible)
        .property("isARVisible", &csp::multiplayer::ImageSpaceComponent::GetIsARVisible, &csp::multiplayer::ImageSpaceComponent::SetIsARVisible)
        .property("isVirtualVisible", &csp::multiplayer::ImageSpaceComponent::GetIsVirtualVisible, &csp::multiplayer::ImageSpaceComponent::SetIsVirtualVisible);
}
