#pragma once

#include "emscripten/val.h"

/*
 * Manifest Val types for pointers to CSP types exposed as nullable single-object handles.
 * Pairs with PointerDeclarations.cpp
 */

EMSCRIPTEN_DECLARE_VAL_TYPE(CancellationTokenPointer);
EMSCRIPTEN_DECLARE_VAL_TYPE(SpaceEntityPointer);
EMSCRIPTEN_DECLARE_VAL_TYPE(ComponentBasePointer);
EMSCRIPTEN_DECLARE_VAL_TYPE(MultiplayerConnectionPointer);
EMSCRIPTEN_DECLARE_VAL_TYPE(NetworkEventBusPointer);
EMSCRIPTEN_DECLARE_VAL_TYPE(OnlineRealtimeEnginePointer);
EMSCRIPTEN_DECLARE_VAL_TYPE(LogSystemPointer);

EMSCRIPTEN_DECLARE_VAL_TYPE(UserSystemPointer);
EMSCRIPTEN_DECLARE_VAL_TYPE(SpaceSystemPointer);
EMSCRIPTEN_DECLARE_VAL_TYPE(AssetSystemPointer);
EMSCRIPTEN_DECLARE_VAL_TYPE(VoipSystemPointer);
EMSCRIPTEN_DECLARE_VAL_TYPE(ScriptSystemPointer);
EMSCRIPTEN_DECLARE_VAL_TYPE(PointOfInterestSystemPointer);
EMSCRIPTEN_DECLARE_VAL_TYPE(AnchorSystemPointer);
EMSCRIPTEN_DECLARE_VAL_TYPE(ApplicationSettingsSystemPointer);
EMSCRIPTEN_DECLARE_VAL_TYPE(SettingsSystemPointer);
EMSCRIPTEN_DECLARE_VAL_TYPE(GraphQLSystemPointer);
EMSCRIPTEN_DECLARE_VAL_TYPE(MaintenanceSystemPointer);
EMSCRIPTEN_DECLARE_VAL_TYPE(EventTicketingSystemPointer);
EMSCRIPTEN_DECLARE_VAL_TYPE(ECommerceSystemPointer);
EMSCRIPTEN_DECLARE_VAL_TYPE(QuotaSystemPointer);
EMSCRIPTEN_DECLARE_VAL_TYPE(SequenceSystemPointer);
EMSCRIPTEN_DECLARE_VAL_TYPE(HotspotSequenceSystemPointer);
EMSCRIPTEN_DECLARE_VAL_TYPE(AnalyticsSystemPointer);
EMSCRIPTEN_DECLARE_VAL_TYPE(ExternalServiceProxySystemPointer);

/*
 * Materials are the one place the API hands back an owning pointer, so unlike every other pointer
 * here this one must keep its disposal behaviours. See the asset system and material bindings for
 * usage.
 */
EMSCRIPTEN_DECLARE_VAL_TYPE(OwnedMaterialPointer);

EMSCRIPTEN_DECLARE_VAL_TYPE(EntityScriptReference);
EMSCRIPTEN_DECLARE_VAL_TYPE(SystemsManagerReference);
