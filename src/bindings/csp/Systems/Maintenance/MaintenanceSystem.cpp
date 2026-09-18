#include "../../../async/Promises.h"
#include "../../../containers/String.h"
#include "../../CallbackDeclarations.h"
#include "../../PromiseDeclarations.h"

#include "CSP/Common/String.h"
#include "CSP/Systems/Maintenance/Maintenance.h"
#include "CSP/Systems/Maintenance/MaintenanceSystem.h"
#include "CSP/Systems/SystemBase.h"

#include "emscripten/bind.h"
#include "emscripten/val.h"

namespace emscripten::internal {
template <> void raw_destructor<csp::systems::MaintenanceSystem>(csp::systems::MaintenanceSystem*) { }
}

EMSCRIPTEN_BINDINGS(CSPMaintenanceSystem)
{
    emscripten::class_<csp::systems::MaintenanceSystem, emscripten::base<csp::systems::SystemBase>>("MaintenanceSystem")
        .function(
            "getMaintenanceInfo(maintenanceUrl)", +[](csp::systems::MaintenanceSystem& self, const csp::common::String& maintenanceUrl) {
                return Promisify<PromiseOfMaintenanceInfoResult>(
                    [&](emscripten::val cb) { self.GetMaintenanceInfo(maintenanceUrl, ToNativeCallback(cb.as<MaintenanceInfoCallback>())); });
            });
}
