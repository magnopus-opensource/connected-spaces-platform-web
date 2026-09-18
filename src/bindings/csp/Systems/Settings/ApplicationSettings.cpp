#include "CSP/Systems/Settings/ApplicationSettings.h"
#include "CSP/Common/Settings.h"
#include "CSP/Systems/WebService.h"

#include "emscripten/bind.h"

EMSCRIPTEN_BINDINGS(CSPApplicationSettings)
{
    emscripten::class_<csp::systems::ApplicationSettingsResult, emscripten::base<csp::systems::ResultBase>>("ApplicationSettingsResult")
        .function(
            "getApplicationSettings", +[](const csp::systems::ApplicationSettingsResult& self) { return self.GetApplicationSettings(); });
}
