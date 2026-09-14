#include "../containers/Array.h"
#include "../containers/Optional.h"
#include "../containers/String.h"
#include "../utils/JSDisposable.h"

#include "CSP/CSPFoundation.h"
#include "CSP/Common/Array.h"
#include "CSP/Common/Optional.h"
#include "CSP/Common/String.h"

#include "emscripten/bind.h"
#include <cstdint>
#include <utility>

EMSCRIPTEN_BINDINGS(CSPFoundation)
{
    emscripten::enum_<csp::EFeatureFlag>("EFeatureFlag", emscripten::enum_value_type::number).value("Invalid", csp::EFeatureFlag::Invalid);

    emscripten::class_<csp::FeatureFlag>("FeatureFlag")
        .class_function(
            "create", +[]() { return csp::FeatureFlag(); })
        .class_function(
            "createWithValues(type, isEnabled)", +[](csp::EFeatureFlag type, bool isEnabled) { return csp::FeatureFlag(type, isEnabled); })
        .property("type", &csp::FeatureFlag::Type)
        .property("enabled", &csp::FeatureFlag::Enabled)
        .function(
            "getDescription", +[](csp::FeatureFlag& self) { return self.GetDescription(); })
        .function("equals(other)", &csp::FeatureFlag::operator==);

    emscripten::class_<csp::ServiceDefinition>("ServiceDefinition")
        .class_function(
            "create", +[]() { return csp::ServiceDefinition(); })
        .class_function(
            "createWithValues(uri, version)", +[](const csp::common::String& uri, uint32_t version) { return csp::ServiceDefinition(uri, version); })
        .property("uri", &csp::ServiceDefinition::GetURI, &csp::ServiceDefinition::SetURI)
        .property("version", &csp::ServiceDefinition::GetVersion);

    emscripten::class_<csp::EndpointURIs>("EndpointURIs")
        .class_function(
            "create", +[]() { return csp::EndpointURIs(); })
        .function(
            "getUserService", +[](const csp::EndpointURIs& self) { return self.UserService; })
        .function(
            "setUserService(value)", +[](csp::EndpointURIs& self, csp::ServiceDefinition value) { self.UserService = std::move(value); })
        .function(
            "getPrototypeService", +[](const csp::EndpointURIs& self) { return self.PrototypeService; })
        .function(
            "setPrototypeService(value)", +[](csp::EndpointURIs& self, csp::ServiceDefinition value) { self.PrototypeService = std::move(value); })
        .function(
            "getSpatialDataService", +[](const csp::EndpointURIs& self) { return self.SpatialDataService; })
        .function(
            "setSpatialDataService(value)", +[](csp::EndpointURIs& self, csp::ServiceDefinition value) { self.SpatialDataService = std::move(value); })
        .function(
            "getMultiplayerService", +[](const csp::EndpointURIs& self) { return self.MultiplayerService; })
        .function(
            "setMultiplayerService(value)", +[](csp::EndpointURIs& self, csp::ServiceDefinition value) { self.MultiplayerService = std::move(value); })
        .function(
            "getAggregationService", +[](const csp::EndpointURIs& self) { return self.AggregationService; })
        .function(
            "setAggregationService(value)", +[](csp::EndpointURIs& self, csp::ServiceDefinition value) { self.AggregationService = std::move(value); })
        .function(
            "getTrackingService", +[](const csp::EndpointURIs& self) { return self.TrackingService; })
        .function(
            "setTrackingService(value)", +[](csp::EndpointURIs& self, csp::ServiceDefinition value) { self.TrackingService = std::move(value); })
        .function(
            "getMaintenanceWindow", +[](const csp::EndpointURIs& self) { return self.MaintenanceWindow; })
        .function(
            "setMaintenanceWindow(value)", +[](csp::EndpointURIs& self, csp::ServiceDefinition value) { self.MaintenanceWindow = std::move(value); })
        .function(
            "getMultiplayerConnection", +[](const csp::EndpointURIs& self) { return self.MultiplayerConnection; })
        .function(
            "setMultiplayerConnection(value)", +[](csp::EndpointURIs& self, csp::ServiceDefinition value) { self.MultiplayerConnection = std::move(value); });

    emscripten::class_<csp::ClientUserAgent>("ClientUserAgent")
        .class_function(
            "create", +[]() { return csp::ClientUserAgent(); })
        .property("cspVersion", &csp::ClientUserAgent::CSPVersion)
        .property("clientOs", &csp::ClientUserAgent::ClientOS)
        .property("clientSku", &csp::ClientUserAgent::ClientSKU)
        .property("clientVersion", &csp::ClientUserAgent::ClientVersion)
        .property("clientEnvironment", &csp::ClientUserAgent::ClientEnvironment)
        .property("chsEnvironment", &csp::ClientUserAgent::CHSEnvironment);

    emscripten::class_<csp::CSPFoundation>("CSPFoundation")
        .class_function("initialise(endpointRootUri, tenant, clientUserAgentHeader, featureFlagOverrides)", &csp::CSPFoundation::Initialise)
        .class_function("shutdown", &csp::CSPFoundation::Shutdown)
        .class_function("tick", &csp::CSPFoundation::Tick)
        .class_function("getVersion", &csp::CSPFoundation::GetVersion)
        .class_function("getBuildType", &csp::CSPFoundation::GetBuildType)
        .class_function("getBuildId", &csp::CSPFoundation::GetBuildID)
        .class_function("getDeviceId", &csp::CSPFoundation::GetDeviceId)
        .class_function("getIsInitialised", &csp::CSPFoundation::GetIsInitialised)
        .class_function(
            "getEndpoints", +[]() { return csp::CSPFoundation::GetEndpoints(); })
        .class_function("createEndpointsFromRoot(endpointRootUri)", &csp::CSPFoundation::CreateEndpointsFromRoot)
        .class_function(
            "getClientUserAgentInfo", +[]() { return csp::CSPFoundation::GetClientUserAgentInfo(); })
        .class_function("getClientUserAgentString", &csp::CSPFoundation::GetClientUserAgentString)
        .class_function("getTenant", &csp::CSPFoundation::GetTenant)
        .class_function("isFeatureEnabled(flag)", &csp::CSPFoundation::IsFeatureEnabled)
        .class_function(
            "getFeatureFlags", +[]() { return bindings::utils::JSDisposable<csp::common::Array<csp::FeatureFlag>> { csp::CSPFoundation::GetFeatureFlags() }; })
        .class_function("getFeatureFlagDescription(flag)", &csp::CSPFoundation::GetFeatureFlagDescription);

    emscripten::function("getComponentSchemasJson", &csp::GetComponentSchemasJson);
}
