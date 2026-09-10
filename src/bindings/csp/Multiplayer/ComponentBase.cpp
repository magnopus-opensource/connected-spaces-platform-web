#include "../../containers/Map.h"
#include "../../containers/String.h"
#include "../../utils/Handles.h"
#include "../../utils/JSDisposable.h"
#include "../CallbackDeclarations.h"
#include "../PointerDeclarations.h"

#include "CSP/Common/Map.h"
#include "CSP/Common/ReplicatedValue.h"
#include "CSP/Common/String.h"
#include "CSP/Multiplayer/ComponentBase.h"
#include "CSP/Multiplayer/SpaceEntity.h"

#include "emscripten/bind.h"
#include "emscripten/val.h"
#include <cstdint>
#include <utility>

EMSCRIPTEN_BINDINGS(CSPComponentBase)
{
    emscripten::enum_<csp::multiplayer::ComponentType>("ComponentType", emscripten::enum_value_type::number)
        .value("Invalid", csp::multiplayer::ComponentType::Invalid)
        .value("Core", csp::multiplayer::ComponentType::Core)
        .value("UIController_DEPRECATED", csp::multiplayer::ComponentType::UIController_DEPRECATED)
        .value("StaticModel", csp::multiplayer::ComponentType::StaticModel)
        .value("AnimatedModel", csp::multiplayer::ComponentType::AnimatedModel)
        .value("MediaSurface_DEPRECATED", csp::multiplayer::ComponentType::MediaSurface_DEPRECATED)
        .value("VideoPlayer", csp::multiplayer::ComponentType::VideoPlayer)
        .value("ImageSequencer_DEPRECATED", csp::multiplayer::ComponentType::ImageSequencer_DEPRECATED)
        .value("ExternalLink", csp::multiplayer::ComponentType::ExternalLink)
        .value("AvatarData", csp::multiplayer::ComponentType::AvatarData)
        .value("Light", csp::multiplayer::ComponentType::Light)
        .value("Button", csp::multiplayer::ComponentType::Button)
        .value("Image", csp::multiplayer::ComponentType::Image)
        .value("ScriptData", csp::multiplayer::ComponentType::ScriptData)
        .value("Custom", csp::multiplayer::ComponentType::Custom)
        .value("Conversation", csp::multiplayer::ComponentType::Conversation)
        .value("Portal", csp::multiplayer::ComponentType::Portal)
        .value("Audio", csp::multiplayer::ComponentType::Audio)
        .value("Spline", csp::multiplayer::ComponentType::Spline)
        .value("Collision", csp::multiplayer::ComponentType::Collision)
        .value("Reflection", csp::multiplayer::ComponentType::Reflection)
        .value("Fog", csp::multiplayer::ComponentType::Fog)
        .value("ECommerce", csp::multiplayer::ComponentType::ECommerce)
        .value("FiducialMarker", csp::multiplayer::ComponentType::FiducialMarker)
        .value("GaussianSplat", csp::multiplayer::ComponentType::GaussianSplat)
        .value("Text", csp::multiplayer::ComponentType::Text)
        .value("Hotspot", csp::multiplayer::ComponentType::Hotspot)
        .value("CinematicCamera", csp::multiplayer::ComponentType::CinematicCamera)
        .value("ScreenSharing", csp::multiplayer::ComponentType::ScreenSharing)
        .value("AIChatbot", csp::multiplayer::ComponentType::AIChatbot)
        .value("Delete", csp::multiplayer::ComponentType::Delete);

    emscripten::class_<csp::multiplayer::ComponentBase>("ComponentBase")
        .property("id", &csp::multiplayer::ComponentBase::GetId)
        .property("componentType", &csp::multiplayer::ComponentBase::GetComponentType)
        .property("typeId", &csp::multiplayer::ComponentBase::GetTypeId)
        .property("componentName", &csp::multiplayer::ComponentBase::GetComponentName, &csp::multiplayer::ComponentBase::SetComponentName)
        // These property functions return "view" pointers, but we don't really care for the complexity, just copy they're not that big.
        .function(
            "getProperties",
            +[](const csp::multiplayer::ComponentBase& self) -> bindings::utils::JSDisposable<csp::common::Map<uint32_t, csp::common::ReplicatedValue>> {
                return bindings::utils::JSDisposable<csp::common::Map<uint32_t, csp::common::ReplicatedValue>> { *self.GetProperties() };
            })
        .function(
            "getProperty(key)",
            +[](const csp::multiplayer::ComponentBase& self, uint16_t key) -> csp::common::ReplicatedValue {
                const auto* prop = self.GetProperty(key);
                // Honestly paranoid, the CSP implementation as written today does guard against this anyway. The return value has no need to be a pointer ... it's odd.
                return prop == nullptr ? csp::common::ReplicatedValue { } : *prop;
            })
        .function("setProperty(key, value)", &csp::multiplayer::ComponentBase::SetProperty)
        .function(
            "getParent", +[](csp::multiplayer::ComponentBase& self) { return bindings::utils::NonOwningVal<SpaceEntityPointer>(self.GetParent()); })
        .function(
            "registerActionHandler(action, actionHandler)",
            +[](csp::multiplayer::ComponentBase& self, const csp::common::String& action, EntityActionHandlerCallback actionHandler) {
                self.RegisterActionHandler(action, ToNativeCallback(actionHandler));
            })
        .function("unregisterActionHandler(action)", &csp::multiplayer::ComponentBase::UnregisterActionHandler)
        .function("invokeAction(action, actionParams)", &csp::multiplayer::ComponentBase::InvokeAction);
}
