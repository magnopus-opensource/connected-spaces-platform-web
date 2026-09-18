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
    emscripten::register_type<MultiplayerConnectionPointer>("MultiplayerConnection | null");
    emscripten::register_type<NetworkEventBusPointer>("NetworkEventBus | null");
    emscripten::register_type<OnlineRealtimeEnginePointer>("OnlineRealtimeEngine | null");
    emscripten::register_type<LogSystemPointer>("LogSystem | null");

    emscripten::register_type<UserSystemPointer>("UserSystem | null");
    emscripten::register_type<SpaceSystemPointer>("SpaceSystem | null");
    emscripten::register_type<AssetSystemPointer>("AssetSystem | null");
    emscripten::register_type<VoipSystemPointer>("VoipSystem | null");
    emscripten::register_type<ScriptSystemPointer>("ScriptSystem | null");
    emscripten::register_type<PointOfInterestSystemPointer>("PointOfInterestSystem | null");
    emscripten::register_type<AnchorSystemPointer>("AnchorSystem | null");
    emscripten::register_type<ApplicationSettingsSystemPointer>("ApplicationSettingsSystem | null");
    emscripten::register_type<SettingsSystemPointer>("SettingsSystem | null");
    emscripten::register_type<GraphQLSystemPointer>("GraphQLSystem | null");
    emscripten::register_type<MaintenanceSystemPointer>("MaintenanceSystem | null");
    emscripten::register_type<EventTicketingSystemPointer>("EventTicketingSystem | null");
    emscripten::register_type<ECommerceSystemPointer>("ECommerceSystem | null");
    emscripten::register_type<QuotaSystemPointer>("QuotaSystem | null");
    emscripten::register_type<SequenceSystemPointer>("SequenceSystem | null");
    emscripten::register_type<HotspotSequenceSystemPointer>("HotspotSequenceSystem | null");
    emscripten::register_type<AnalyticsSystemPointer>("AnalyticsSystem | null");
    emscripten::register_type<ExternalServiceProxySystemPointer>("ExternalServiceProxySystem | null");

    emscripten::register_type<EntityScriptReference>("EntityScript");
    emscripten::register_type<SystemsManagerReference>("SystemsManager");
}
