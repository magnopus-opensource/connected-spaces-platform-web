#include "PointerDeclarations.h"

#include "emscripten/bind.h"

/*
 * Registrations for the pointer types declared in PointerDeclarations.h.
 * Each registration is what gives the pointer its TypeScript signature in the generated .d.ts.
 */

EMSCRIPTEN_BINDINGS(CSPPointerDeclarations)
{
    emscripten::register_type<CancellationTokenPointer>("CancellationToken | null");
    emscripten::register_type<SpaceEntityPointer>("SpaceEntity | null");
    emscripten::register_type<ComponentBasePointer>("ComponentBase | null");
    emscripten::register_type<ReplicatedValuePointer>("ReplicatedValue | null");
    emscripten::register_type<MultiplayerConnectionPointer>("MultiplayerConnection | null");
    emscripten::register_type<NetworkEventBusPointer>("NetworkEventBus | null");
    emscripten::register_type<OnlineRealtimeEnginePointer>("OnlineRealtimeEngine | null");
    emscripten::register_type<LogSystemPointer>("LogSystem | null");

    emscripten::register_type<EntityScriptReference>("EntityScript");
    emscripten::register_type<SystemsManagerReference>("SystemsManager");
}
