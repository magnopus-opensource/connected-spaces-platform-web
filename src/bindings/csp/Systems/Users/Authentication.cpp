#include "../../../containers/String.h"

#include "CSP/Common/LoginState.h"
#include "CSP/Common/String.h"
#include "CSP/Systems/Users/Authentication.h"
#include "CSP/Systems/WebService.h"

#include "emscripten/bind.h"

EMSCRIPTEN_BINDINGS(CSPAuthentication)
{
    emscripten::class_<csp::systems::LoginTokenInfo>("LoginTokenInfo")
        .class_function(
            "create", +[]() { return csp::systems::LoginTokenInfo(); })
        .property("accessToken", &csp::systems::LoginTokenInfo::AccessToken)
        .property("accessExpiryTime", &csp::systems::LoginTokenInfo::AccessExpiryTime)
        .property("refreshToken", &csp::systems::LoginTokenInfo::RefreshToken)
        .property("refreshExpiryTime", &csp::systems::LoginTokenInfo::RefreshExpiryTime);

    emscripten::class_<csp::systems::TokenOptions>("TokenOptions")
        .class_function(
            "create", +[]() { return csp::systems::TokenOptions(); })
        .property("accessTokenExpiryLength", &csp::systems::TokenOptions::AccessTokenExpiryLength)
        .property("refreshTokenExpiryLength", &csp::systems::TokenOptions::RefreshTokenExpiryLength);

    emscripten::class_<csp::systems::LoginStateResult, emscripten::base<csp::systems::ResultBase>>("LoginStateResult")
        .function(
            "getLoginState", +[](const csp::systems::LoginStateResult& self) { return self.GetLoginState(); });

    emscripten::class_<csp::systems::LoginTokenInfoResult, emscripten::base<csp::systems::ResultBase>>("LoginTokenInfoResult")
        .function(
            "getLoginTokenInfo", +[](const csp::systems::LoginTokenInfoResult& self) { return self.GetLoginTokenInfo(); });
}
