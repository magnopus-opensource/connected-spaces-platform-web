// Stub binding.

#include "../../utils/Handles.h"
#include "../PointerDeclarations.h"

#include "CSP/Systems/SystemsManager.h"

#include "emscripten/bind.h"
#include "emscripten/val.h"

namespace emscripten::internal {
// Embind instantiates raw_destructor whether you like it or not in the `class_` object.
// This satisfies it, it has no other effect (i hope).
template <> void raw_destructor<csp::systems::SystemsManager>(csp::systems::SystemsManager*) { }
}

EMSCRIPTEN_BINDINGS(CSPSystemsManager)
{
    emscripten::class_<csp::systems::SystemsManager>("SystemsManager")
        .class_function(
            "get", +[]() { return bindings::utils::NonOwningVal<SystemsManagerReference>(&csp::systems::SystemsManager::Get()); })
        .function(
            "getMultiplayerConnection",
            +[](csp::systems::SystemsManager& self) { return bindings::utils::NonOwningVal<MultiplayerConnectionPointer>(self.GetMultiplayerConnection()); })
        .function(
            "getEventBus", +[](csp::systems::SystemsManager& self) { return bindings::utils::NonOwningVal<NetworkEventBusPointer>(self.GetEventBus()); })
        .function("getLogSystem", +[](csp::systems::SystemsManager& self) { return bindings::utils::NonOwningVal<LogSystemPointer>(self.GetLogSystem()); });
}
