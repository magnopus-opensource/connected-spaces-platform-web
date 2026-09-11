#include "../../../containers/String.h"

#include "CSP/Common/String.h"
#include "CSP/Common/Vector.h"
#include "CSP/Multiplayer/Components/PortalSpaceComponent.h"

#include "emscripten/bind.h"

EMSCRIPTEN_BINDINGS(CSPPortalSpaceComponent)
{
    emscripten::enum_<csp::multiplayer::PortalPropertyKeys>("PortalPropertyKeys", emscripten::enum_value_type::number)
        .value("SpaceId", csp::multiplayer::PortalPropertyKeys::SpaceId)
        .value("IsVisible", csp::multiplayer::PortalPropertyKeys::IsVisible)
        .value("IsActive", csp::multiplayer::PortalPropertyKeys::IsActive)
        .value("IsARVisible", csp::multiplayer::PortalPropertyKeys::IsARVisible)
        .value("IsEnabled", csp::multiplayer::PortalPropertyKeys::IsEnabled)
        .value("Position", csp::multiplayer::PortalPropertyKeys::Position)
        .value("Radius", csp::multiplayer::PortalPropertyKeys::Radius)
        .value("Num", csp::multiplayer::PortalPropertyKeys::Num);

    emscripten::class_<csp::multiplayer::PortalSpaceComponent, emscripten::base<csp::multiplayer::ComponentBase>>("PortalSpaceComponent")
        .property("spaceId", &csp::multiplayer::PortalSpaceComponent::GetSpaceId, &csp::multiplayer::PortalSpaceComponent::SetSpaceId)
        .property("position", &csp::multiplayer::PortalSpaceComponent::GetPosition, &csp::multiplayer::PortalSpaceComponent::SetPosition)
        .property("radius", &csp::multiplayer::PortalSpaceComponent::GetRadius, &csp::multiplayer::PortalSpaceComponent::SetRadius)
        .property("isEnabled", &csp::multiplayer::PortalSpaceComponent::GetIsEnabled, &csp::multiplayer::PortalSpaceComponent::SetIsEnabled);
}
