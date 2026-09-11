#include "../../../containers/String.h"

#include "CSP/Common/String.h"
#include "CSP/Common/Vector.h"
#include "CSP/Multiplayer/Components/AIChatbotComponent.h"

#include "emscripten/bind.h"

EMSCRIPTEN_BINDINGS(CSPAIChatbotSpaceComponent)
{
    emscripten::enum_<csp::multiplayer::AIChatbotPropertyKeys>("AIChatbotPropertyKeys", emscripten::enum_value_type::number)
        .value("Position", csp::multiplayer::AIChatbotPropertyKeys::Position)
        .value("Voice", csp::multiplayer::AIChatbotPropertyKeys::Voice)
        .value("GuardrailAssetCollectionId", csp::multiplayer::AIChatbotPropertyKeys::GuardrailAssetCollectionId)
        .value("VisualState", csp::multiplayer::AIChatbotPropertyKeys::VisualState)
        .value("Num", csp::multiplayer::AIChatbotPropertyKeys::Num);

    emscripten::enum_<csp::multiplayer::AIChatbotVisualState>("AIChatbotVisualState", emscripten::enum_value_type::number)
        .value("Waiting", csp::multiplayer::AIChatbotVisualState::Waiting)
        .value("Listening", csp::multiplayer::AIChatbotVisualState::Listening)
        .value("Thinking", csp::multiplayer::AIChatbotVisualState::Thinking)
        .value("Speaking", csp::multiplayer::AIChatbotVisualState::Speaking)
        .value("Unknown", csp::multiplayer::AIChatbotVisualState::Unknown)
        .value("Num", csp::multiplayer::AIChatbotVisualState::Num);

    emscripten::class_<csp::multiplayer::AIChatbotSpaceComponent, emscripten::base<csp::multiplayer::ComponentBase>>("AIChatbotSpaceComponent")
        .property("voice", &csp::multiplayer::AIChatbotSpaceComponent::GetVoice, &csp::multiplayer::AIChatbotSpaceComponent::SetVoice)
        .property("guardrailAssetCollectionId", &csp::multiplayer::AIChatbotSpaceComponent::GetGuardrailAssetCollectionId,
            &csp::multiplayer::AIChatbotSpaceComponent::SetGuardrailAssetCollectionId)
        .property("visualState", &csp::multiplayer::AIChatbotSpaceComponent::GetVisualState, &csp::multiplayer::AIChatbotSpaceComponent::SetVisualState)
        .property("position", &csp::multiplayer::AIChatbotSpaceComponent::GetPosition, &csp::multiplayer::AIChatbotSpaceComponent::SetPosition);
}
