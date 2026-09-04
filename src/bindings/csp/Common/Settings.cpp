#include "../../containers/Map.h"
#include "../../containers/String.h"

#include "CSP/Common/Map.h"
#include "CSP/Common/Settings.h"
#include "CSP/Common/String.h"

#include "emscripten/bind.h"

EMSCRIPTEN_BINDINGS(CSPSettings)
{
    emscripten::register_type<csp::common::Map<csp::common::String, csp::common::String>>("Map<string, string>");

    emscripten::class_<csp::common::ApplicationSettings>("ApplicationSettings")
        .class_function(
            "create", +[]() { return csp::common::ApplicationSettings(); })
        .property("applicationName", &csp::common::ApplicationSettings::ApplicationName)
        .property("context", &csp::common::ApplicationSettings::Context)
        .property("allowAnonymous", &csp::common::ApplicationSettings::AllowAnonymous)
        .property("settings", &csp::common::ApplicationSettings::Settings)
        .function("equals(other)", &csp::common::ApplicationSettings::operator==);

    emscripten::class_<csp::common::SettingsCollection>("SettingsCollection")
        .class_function(
            "create", +[]() { return csp::common::SettingsCollection(); })
        .property("userId", &csp::common::SettingsCollection::UserId)
        .property("context", &csp::common::SettingsCollection::Context)
        .property("settings", &csp::common::SettingsCollection::Settings)
        .function("equals(other)", &csp::common::SettingsCollection::operator==);
}
