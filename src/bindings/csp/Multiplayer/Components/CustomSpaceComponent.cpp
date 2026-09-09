#include "../../../containers/List.h"
#include "../../../containers/String.h"

#include "CSP/Common/List.h"
#include "CSP/Common/ReplicatedValue.h"
#include "CSP/Common/String.h"
#include "CSP/Multiplayer/Components/CustomSpaceComponent.h"

#include "emscripten/bind.h"
#include <cstdint>

EMSCRIPTEN_BINDINGS(CSPCustomSpaceComponent)
{
    emscripten::enum_<csp::multiplayer::CustomComponentPropertyKeys>("CustomComponentPropertyKeys", emscripten::enum_value_type::number)
        .value("ApplicationOrigin", csp::multiplayer::CustomComponentPropertyKeys::ApplicationOrigin)
        .value("CustomPropertyList", csp::multiplayer::CustomComponentPropertyKeys::CustomPropertyList)
        .value("Num", csp::multiplayer::CustomComponentPropertyKeys::Num);

    emscripten::class_<csp::multiplayer::CustomSpaceComponent, emscripten::base<csp::multiplayer::ComponentBase>>("CustomSpaceComponent")
        .property("applicationOrigin", &csp::multiplayer::CustomSpaceComponent::GetApplicationOrigin, &csp::multiplayer::CustomSpaceComponent::SetApplicationOrigin)
        .property("customPropertyKeys", &csp::multiplayer::CustomSpaceComponent::GetCustomPropertyKeys)
        .property("numProperties", &csp::multiplayer::CustomSpaceComponent::GetNumProperties)
        .function("hasCustomProperty(key)", &csp::multiplayer::CustomSpaceComponent::HasCustomProperty)
        .function(
            "getCustomProperty(key)", +[](const csp::multiplayer::CustomSpaceComponent& self, const csp::common::String& key) { return self.GetCustomProperty(key); })
        .function("setCustomProperty(key, value)", &csp::multiplayer::CustomSpaceComponent::SetCustomProperty)
        .function("removeCustomProperty(key)", &csp::multiplayer::CustomSpaceComponent::RemoveCustomProperty)
        .function("getCustomPropertySubscriptionKey(key)", &csp::multiplayer::CustomSpaceComponent::GetCustomPropertySubscriptionKey);
}
