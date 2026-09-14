#include "../../utils/Handles.h"
#include "../PointerDeclarations.h"

#include "CSP/Systems/Analytics/AnalyticsSystem.h"
#include "CSP/Systems/Assets/AssetSystem.h"
#include "CSP/Systems/ECommerce/ECommerceSystem.h"
#include "CSP/Systems/EventTicketing/EventTicketingSystem.h"
#include "CSP/Systems/ExternalServices/ExternalServiceProxySystem.h"
#include "CSP/Systems/GraphQL/GraphQLSystem.h"
#include "CSP/Systems/HotspotSequence/HotspotSequenceSystem.h"
#include "CSP/Systems/Maintenance/MaintenanceSystem.h"
#include "CSP/Systems/Quota/QuotaSystem.h"
#include "CSP/Systems/Script/ScriptSystem.h"
#include "CSP/Systems/Sequence/SequenceSystem.h"
#include "CSP/Systems/Settings/ApplicationSettingsSystem.h"
#include "CSP/Systems/Settings/SettingsSystem.h"
#include "CSP/Systems/Spaces/SpaceSystem.h"
#include "CSP/Systems/Spatial/AnchorSystem.h"
#include "CSP/Systems/Spatial/PointOfInterestSystem.h"
#include "CSP/Systems/SystemsManager.h"
#include "CSP/Systems/Users/UserSystem.h"
#include "CSP/Systems/Voip/VoipSystem.h"

#include "emscripten/bind.h"
#include "emscripten/val.h"

namespace emscripten::internal {
// Embind instantiates raw_destructor whether you like it or not in the `class_` object.
// This satisfies it, it has no other effect (I hope).
template <> void raw_destructor<csp::systems::SystemsManager>(csp::systems::SystemsManager*) { }
}

EMSCRIPTEN_BINDINGS(CSPSystemsManager)
{
    emscripten::class_<csp::systems::SystemsManager>("SystemsManager")
        .class_function(
            "get", +[]() { return bindings::utils::NonOwningVal<SystemsManagerReference>(&csp::systems::SystemsManager::Get()); })
        .function(
            "getUserSystem", +[](csp::systems::SystemsManager& self) { return bindings::utils::NonOwningVal<UserSystemPointer>(self.GetUserSystem()); })
        .function(
            "getSpaceSystem", +[](csp::systems::SystemsManager& self) { return bindings::utils::NonOwningVal<SpaceSystemPointer>(self.GetSpaceSystem()); })
        .function(
            "getAssetSystem", +[](csp::systems::SystemsManager& self) { return bindings::utils::NonOwningVal<AssetSystemPointer>(self.GetAssetSystem()); })
        .function(
            "getVoipSystem", +[](csp::systems::SystemsManager& self) { return bindings::utils::NonOwningVal<VoipSystemPointer>(self.GetVoipSystem()); })
        .function(
            "getScriptSystem", +[](csp::systems::SystemsManager& self) { return bindings::utils::NonOwningVal<ScriptSystemPointer>(self.GetScriptSystem()); })
        .function(
            "getPointOfInterestSystem",
            +[](csp::systems::SystemsManager& self) { return bindings::utils::NonOwningVal<PointOfInterestSystemPointer>(self.GetPointOfInterestSystem()); })
        .function(
            "getAnchorSystem", +[](csp::systems::SystemsManager& self) { return bindings::utils::NonOwningVal<AnchorSystemPointer>(self.GetAnchorSystem()); })
        .function(
            "getLogSystem", +[](csp::systems::SystemsManager& self) { return bindings::utils::NonOwningVal<LogSystemPointer>(self.GetLogSystem()); })
        .function(
            "getApplicationSettingsSystem",
            +[](csp::systems::SystemsManager& self) { return bindings::utils::NonOwningVal<ApplicationSettingsSystemPointer>(self.GetApplicationSettingsSystem()); })
        .function(
            "getSettingsSystem", +[](csp::systems::SystemsManager& self) { return bindings::utils::NonOwningVal<SettingsSystemPointer>(self.GetSettingsSystem()); })
        .function(
            "getGraphQLSystem", +[](csp::systems::SystemsManager& self) { return bindings::utils::NonOwningVal<GraphQLSystemPointer>(self.GetGraphQLSystem()); })
        .function(
            "getMaintenanceSystem", +[](csp::systems::SystemsManager& self) { return bindings::utils::NonOwningVal<MaintenanceSystemPointer>(self.GetMaintenanceSystem()); })
        .function(
            "getEventTicketingSystem",
            +[](csp::systems::SystemsManager& self) { return bindings::utils::NonOwningVal<EventTicketingSystemPointer>(self.GetEventTicketingSystem()); })
        .function(
            "getECommerceSystem", +[](csp::systems::SystemsManager& self) { return bindings::utils::NonOwningVal<ECommerceSystemPointer>(self.GetECommerceSystem()); })
        .function(
            "getQuotaSystem", +[](csp::systems::SystemsManager& self) { return bindings::utils::NonOwningVal<QuotaSystemPointer>(self.GetQuotaSystem()); })
        .function(
            "getSequenceSystem", +[](csp::systems::SystemsManager& self) { return bindings::utils::NonOwningVal<SequenceSystemPointer>(self.GetSequenceSystem()); })
        .function(
            "getHotspotSequenceSystem",
            +[](csp::systems::SystemsManager& self) { return bindings::utils::NonOwningVal<HotspotSequenceSystemPointer>(self.GetHotspotSequenceSystem()); })
        .function(
            "getAnalyticsSystem", +[](csp::systems::SystemsManager& self) { return bindings::utils::NonOwningVal<AnalyticsSystemPointer>(self.GetAnalyticsSystem()); })
        .function(
            "getExternalServicesProxySystem",
            +[](csp::systems::SystemsManager& self) { return bindings::utils::NonOwningVal<ExternalServiceProxySystemPointer>(self.GetExternalServicesProxySystem()); })
        .function(
            "getMultiplayerConnection",
            +[](csp::systems::SystemsManager& self) { return bindings::utils::NonOwningVal<MultiplayerConnectionPointer>(self.GetMultiplayerConnection()); })
        .function(
            "getEventBus", +[](csp::systems::SystemsManager& self) { return bindings::utils::NonOwningVal<NetworkEventBusPointer>(self.GetEventBus()); });
}
