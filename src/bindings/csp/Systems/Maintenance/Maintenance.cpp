#include "../../../containers/Array.h"
#include "../../../containers/String.h"
#include "../../../utils/JSDisposable.h"

#include "CSP/Common/Array.h"
#include "CSP/Common/String.h"
#include "CSP/Systems/Maintenance/Maintenance.h"
#include "CSP/Systems/WebService.h"

#include "emscripten/bind.h"

EMSCRIPTEN_BINDINGS(CSPMaintenance)
{
    emscripten::class_<csp::systems::MaintenanceInfo>("MaintenanceInfo")
        .class_function(
            "create", +[]() { return csp::systems::MaintenanceInfo(); })
        .property("description", &csp::systems::MaintenanceInfo::Description)
        .property("startDateTimestamp", &csp::systems::MaintenanceInfo::StartDateTimestamp)
        .property("endDateTimestamp", &csp::systems::MaintenanceInfo::EndDateTimestamp)
        .property("isInsideWindow", &csp::systems::MaintenanceInfo::IsInsideWindow)
        .function("equals(other)", &csp::systems::MaintenanceInfo::operator==);

    emscripten::class_<csp::systems::MaintenanceInfoResult, emscripten::base<csp::systems::ResultBase>>("MaintenanceInfoResult")
        .property("hasAnyMaintenanceWindows", &csp::systems::MaintenanceInfoResult::HasAnyMaintenanceWindows)
        .function(
            "getMaintenanceInfoResponses",
            +[](const csp::systems::MaintenanceInfoResult& self) {
                return bindings::utils::JSDisposable<csp::common::Array<csp::systems::MaintenanceInfo>> { self.GetMaintenanceInfoResponses() };
            })
        .function(
            "getLatestMaintenanceInfo", +[](const csp::systems::MaintenanceInfoResult& self) { return self.GetLatestMaintenanceInfo(); })
        .function(
            "getDefaultMaintenanceInfo", +[](const csp::systems::MaintenanceInfoResult& self) { return self.GetDefaultMaintenanceInfo(); });
}
