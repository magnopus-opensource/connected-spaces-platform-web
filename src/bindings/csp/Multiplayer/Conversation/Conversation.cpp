// Stub binding.

#include "CSP/Multiplayer/Conversation/Conversation.h"

#include "emscripten/bind.h"

EMSCRIPTEN_BINDINGS(CSPConversation)
{
    emscripten::enum_<csp::multiplayer::ConversationEventType>("ConversationEventType", emscripten::enum_value_type::number)
        .value("NewConversation", csp::multiplayer::ConversationEventType::NewConversation)
        .value("NewMessage", csp::multiplayer::ConversationEventType::NewMessage)
        .value("DeleteMessage", csp::multiplayer::ConversationEventType::DeleteMessage)
        .value("DeleteConversation", csp::multiplayer::ConversationEventType::DeleteConversation)
        .value("ConversationInformation", csp::multiplayer::ConversationEventType::ConversationInformation)
        .value("MessageInformation", csp::multiplayer::ConversationEventType::MessageInformation)
        .value("SetAnnotation", csp::multiplayer::ConversationEventType::SetAnnotation)
        .value("DeleteAnnotation", csp::multiplayer::ConversationEventType::DeleteAnnotation)
        .value("SetConversationAnnotation", csp::multiplayer::ConversationEventType::SetConversationAnnotation)
        .value("DeleteConversationAnnotation", csp::multiplayer::ConversationEventType::DeleteConversationAnnotation);

    emscripten::class_<csp::multiplayer::MessageInfo>("MessageInfo");
}
