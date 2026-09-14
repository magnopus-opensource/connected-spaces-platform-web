#include "../../containers/Array.h"
#include "../../containers/String.h"
#include "../../utils/JSDisposable.h"

#include "CSP/Common/Array.h"
#include "CSP/Common/String.h"
#include "CSP/Systems/ServiceStatus.h"
#include "CSP/Systems/WebService.h"

#include "emscripten/bind.h"
#include <utility>

EMSCRIPTEN_BINDINGS(CSPServiceStatus)
{
    emscripten::class_<csp::systems::VersionMetadata>("VersionMetadata")
        .class_function(
            "create", +[]() { return csp::systems::VersionMetadata(); })
        .property("version", &csp::systems::VersionMetadata::Version)
        .property("deprecationDatetime", &csp::systems::VersionMetadata::DeprecationDatetime)
        .function("equals(other)", &csp::systems::VersionMetadata::operator==);

    emscripten::class_<csp::systems::ServiceStatus>("ServiceStatus")
        .class_function(
            "create", +[]() { return csp::systems::ServiceStatus(); })
        .property("reverseProxy", &csp::systems::ServiceStatus::ReverseProxy)
        .property("name", &csp::systems::ServiceStatus::Name)
        .property("currentApiVersion", &csp::systems::ServiceStatus::CurrentApiVersion)
        .function(
            "getApiVersions",
            +[](const csp::systems::ServiceStatus& self) { return bindings::utils::JSDisposable<csp::common::Array<csp::systems::VersionMetadata>> { self.ApiVersions }; })
        .function(
            "setApiVersions(value)", +[](csp::systems::ServiceStatus& self, csp::common::Array<csp::systems::VersionMetadata> value) { self.ApiVersions = std::move(value); })
        .function("equals(other)", &csp::systems::ServiceStatus::operator==);

    emscripten::class_<csp::systems::ServicesDeploymentStatus>("ServicesDeploymentStatus")
        .class_function(
            "create", +[]() { return csp::systems::ServicesDeploymentStatus(); })
        .property("version", &csp::systems::ServicesDeploymentStatus::Version)
        .function(
            "getServices",
            +[](const csp::systems::ServicesDeploymentStatus& self) { return bindings::utils::JSDisposable<csp::common::Array<csp::systems::ServiceStatus>> { self.Services }; })
        .function(
            "setServices(value)", +[](csp::systems::ServicesDeploymentStatus& self, csp::common::Array<csp::systems::ServiceStatus> value) { self.Services = std::move(value); })
        .function("equals(other)", &csp::systems::ServicesDeploymentStatus::operator==);

    emscripten::class_<csp::systems::ServicesDeploymentStatusResult, emscripten::base<csp::systems::ResultBase>>("ServicesDeploymentStatusResult")
        .function(
            "getLatestServicesDeploymentStatus", +[](const csp::systems::ServicesDeploymentStatusResult& self) { return self.GetLatestServicesDeploymentStatus(); });
}
