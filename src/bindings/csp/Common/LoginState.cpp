#include "../../containers/List.h"
#include "../../containers/String.h"
#include "../../utils/JSDisposable.h"

#include "CSP/Common/List.h"
#include "CSP/Common/LoginState.h"
#include "CSP/Common/Settings.h"
#include "CSP/Common/SharedEnums.h"
#include "CSP/Common/String.h"

#include "emscripten/bind.h"

EMSCRIPTEN_BINDINGS(CSPLoginState)
{
    emscripten::register_type<csp::common::List<csp::common::ApplicationSettings>>("ApplicationSettings[]");
    emscripten::register_type<bindings::utils::JSDisposable<csp::common::List<csp::common::ApplicationSettings>>>("(ApplicationSettings[] & Disposable)");

    emscripten::register_type<csp::common::List<csp::common::SettingsCollection>>("SettingsCollection[]");
    emscripten::register_type<bindings::utils::JSDisposable<csp::common::List<csp::common::SettingsCollection>>>("(SettingsCollection[] & Disposable)");

    emscripten::class_<csp::common::LoginState>("LoginState")
        .class_function(
            "create", +[]() { return csp::common::LoginState(); })
        .property("refreshNeeded", &csp::common::LoginState::RefreshNeeded)
        .property("userId", &csp::common::LoginState::GetUserId)
        .property("deviceId", &csp::common::LoginState::GetDeviceId)
        .property("accessToken", &csp::common::LoginState::GetAccessToken)
        .property("refreshToken", &csp::common::LoginState::GetRefreshToken)
        .property("loginStateValue", &csp::common::LoginState::GetLoginStateValue)
        .function(
            "getDefaultApplicationSettings",
            +[](const csp::common::LoginState& self) {
                return bindings::utils::JSDisposable<csp::common::List<csp::common::ApplicationSettings>> { self.GetDefaultApplicationSettings() };
            })
        .function(
            "getDefaultSettings",
            +[](const csp::common::LoginState& self) { return bindings::utils::JSDisposable<csp::common::List<csp::common::SettingsCollection>> { self.GetDefaultSettings() }; });
}
