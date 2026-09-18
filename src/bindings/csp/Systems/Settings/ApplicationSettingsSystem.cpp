#include "../../../async/Promises.h"
#include "../../../containers/Array.h"
#include "../../../containers/Optional.h"
#include "../../../containers/String.h"
#include "../../CallbackDeclarations.h"
#include "../../PromiseDeclarations.h"

#include "CSP/Common/Array.h"
#include "CSP/Common/Optional.h"
#include "CSP/Common/String.h"
#include "CSP/Systems/Settings/ApplicationSettings.h"
#include "CSP/Systems/Settings/ApplicationSettingsSystem.h"
#include "CSP/Systems/SystemBase.h"

#include "emscripten/bind.h"
#include "emscripten/val.h"

namespace emscripten::internal {
template <> void raw_destructor<csp::systems::ApplicationSettingsSystem>(csp::systems::ApplicationSettingsSystem*) { }
}

EMSCRIPTEN_BINDINGS(CSPApplicationSettingsSystem)
{
    emscripten::class_<csp::systems::ApplicationSettingsSystem, emscripten::base<csp::systems::SystemBase>>("ApplicationSettingsSystem")
        .function(
            "getSettingsByContext(applicationName, context, keys)",
            +[](csp::systems::ApplicationSettingsSystem& self, const csp::common::String& applicationName, const csp::common::String& context,
                 const csp::common::Optional<csp::common::Array<csp::common::String>>& keys) {
                return Promisify<PromiseOfApplicationSettingsResult>(
                    [&](emscripten::val cb) { self.GetSettingsByContext(applicationName, context, keys, ToNativeCallback(cb.as<ApplicationSettingsResultCallback>())); });
            })
        .function(
            "getSettingsByContextAnonymous(tenant, applicationName, context, keys)",
            +[](csp::systems::ApplicationSettingsSystem& self, const csp::common::String& tenant, const csp::common::String& applicationName, const csp::common::String& context,
                 const csp::common::Optional<csp::common::Array<csp::common::String>>& keys) {
                return Promisify<PromiseOfApplicationSettingsResult>([&](emscripten::val cb) {
                    self.GetSettingsByContextAnonymous(tenant, applicationName, context, keys, ToNativeCallback(cb.as<ApplicationSettingsResultCallback>()));
                });
            });
}
