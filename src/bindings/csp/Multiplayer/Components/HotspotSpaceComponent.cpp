#include "../../../containers/String.h"

#include "CSP/Common/String.h"
#include "CSP/Common/Vector.h"
#include "CSP/Multiplayer/Components/HotspotSpaceComponent.h"

#include "emscripten/bind.h"

EMSCRIPTEN_BINDINGS(CSPHotspotSpaceComponent)
{
    emscripten::enum_<csp::multiplayer::HotspotPropertyKeys>("HotspotPropertyKeys", emscripten::enum_value_type::number)
        .value("Position", csp::multiplayer::HotspotPropertyKeys::Position)
        .value("Rotation", csp::multiplayer::HotspotPropertyKeys::Rotation)
        .value("IsTeleportPoint", csp::multiplayer::HotspotPropertyKeys::IsTeleportPoint)
        .value("IsSpawnPoint", csp::multiplayer::HotspotPropertyKeys::IsSpawnPoint)
        .value("IsVisible", csp::multiplayer::HotspotPropertyKeys::IsVisible)
        .value("IsARVisible", csp::multiplayer::HotspotPropertyKeys::IsARVisible)
        .value("IsVirtualVisible", csp::multiplayer::HotspotPropertyKeys::IsVirtualVisible)
        .value("Num", csp::multiplayer::HotspotPropertyKeys::Num);

    emscripten::class_<csp::multiplayer::HotspotSpaceComponent, emscripten::base<csp::multiplayer::ComponentBase>>("HotspotSpaceComponent")
        .property("isTeleportPoint", &csp::multiplayer::HotspotSpaceComponent::GetIsTeleportPoint, &csp::multiplayer::HotspotSpaceComponent::SetIsTeleportPoint)
        .property("isSpawnPoint", &csp::multiplayer::HotspotSpaceComponent::GetIsSpawnPoint, &csp::multiplayer::HotspotSpaceComponent::SetIsSpawnPoint)
        .property("uniqueComponentId", &csp::multiplayer::HotspotSpaceComponent::GetUniqueComponentId)
        .property("position", &csp::multiplayer::HotspotSpaceComponent::GetPosition, &csp::multiplayer::HotspotSpaceComponent::SetPosition)
        .property("rotation", &csp::multiplayer::HotspotSpaceComponent::GetRotation, &csp::multiplayer::HotspotSpaceComponent::SetRotation)
        .property("isVisible", &csp::multiplayer::HotspotSpaceComponent::GetIsVisible, &csp::multiplayer::HotspotSpaceComponent::SetIsVisible)
        .property("isARVisible", &csp::multiplayer::HotspotSpaceComponent::GetIsARVisible, &csp::multiplayer::HotspotSpaceComponent::SetIsARVisible)
        .property("isVirtualVisible", &csp::multiplayer::HotspotSpaceComponent::GetIsVirtualVisible, &csp::multiplayer::HotspotSpaceComponent::SetIsVirtualVisible);
}
