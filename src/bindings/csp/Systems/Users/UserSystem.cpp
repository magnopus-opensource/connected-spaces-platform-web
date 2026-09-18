#include "../../../async/Promises.h"
#include "../../../containers/Array.h"
#include "../../../containers/Optional.h"
#include "../../../containers/String.h"
#include "../../CallbackDeclarations.h"
#include "../../PromiseDeclarations.h"

#include "CSP/Common/Array.h"
#include "CSP/Common/LoginState.h"
#include "CSP/Common/Optional.h"
#include "CSP/Common/SharedEnums.h"
#include "CSP/Common/String.h"
#include "CSP/Systems/Quota/Quota.h"
#include "CSP/Systems/SystemBase.h"
#include "CSP/Systems/Users/Authentication.h"
#include "CSP/Systems/Users/ThirdPartyAuthentication.h"
#include "CSP/Systems/Users/UserSystem.h"

#include "emscripten/bind.h"
#include "emscripten/val.h"

namespace emscripten::internal {
template <> void raw_destructor<csp::systems::UserSystem>(csp::systems::UserSystem*) { }
}

EMSCRIPTEN_BINDINGS(CSPUserSystem)
{
    emscripten::class_<csp::systems::UserSystem, emscripten::base<csp::systems::SystemBase>>("UserSystem")
        .function(
            "getLoginState", +[](const csp::systems::UserSystem& self) { return self.GetLoginState(); })
        .function(
            "getSupportedThirdPartyAuthenticationProviders",
            +[](const csp::systems::UserSystem& self) {
                return csp::common::Array<csp::systems::EThirdPartyAuthenticationProviders> { self.GetSupportedThirdPartyAuthenticationProviders() };
            })
        .function(
            "setNewLoginTokenReceivedCallback(callback)",
            +[](csp::systems::UserSystem& self, LoginTokenInfoResultCallback callback) { self.SetNewLoginTokenReceivedCallback(ToNativeCallback(callback)); })
        .function(
            "setUserPermissionsChangedCallback(callback)",
            +[](csp::systems::UserSystem& self, AccessControlChangedEventCallback callback) { self.SetUserPermissionsChangedCallback(ToNativeCallback(callback)); })
        .function(
            "login(email, password, createMultiplayerConnection, userHasVerifiedAge, tokenOptions)",
            +[](csp::systems::UserSystem& self, const csp::common::String& email, const csp::common::String& password, bool createMultiplayerConnection,
                 const csp::common::Optional<bool>& userHasVerifiedAge, const csp::common::Optional<csp::systems::TokenOptions>& tokenOptions) {
                return Promisify<PromiseOfLoginStateResult>([&](emscripten::val cb) {
                    self.Login(email, password, createMultiplayerConnection, userHasVerifiedAge, tokenOptions, ToNativeCallback(cb.as<LoginStateResultCallback>()));
                });
            })
        .function(
            "loginWithRefreshToken(userId, refreshToken, createMultiplayerConnection, tokenOptions)",
            +[](csp::systems::UserSystem& self, const csp::common::String& userId, const csp::common::String& refreshToken, bool createMultiplayerConnection,
                 const csp::common::Optional<csp::systems::TokenOptions>& tokenOptions) {
                return Promisify<PromiseOfLoginStateResult>([&](emscripten::val cb) {
                    self.LoginWithRefreshToken(userId, refreshToken, createMultiplayerConnection, tokenOptions, ToNativeCallback(cb.as<LoginStateResultCallback>()));
                });
            })
        .function(
            "loginAsGuest(createMultiplayerConnection, userHasVerifiedAge, tokenOptions)",
            +[](csp::systems::UserSystem& self, bool createMultiplayerConnection, const csp::common::Optional<bool>& userHasVerifiedAge,
                 const csp::common::Optional<csp::systems::TokenOptions>& tokenOptions) {
                return Promisify<PromiseOfLoginStateResult>([&](emscripten::val cb) {
                    self.LoginAsGuest(createMultiplayerConnection, userHasVerifiedAge, tokenOptions, ToNativeCallback(cb.as<LoginStateResultCallback>()));
                });
            })
        .function(
            "loginAsGuestWithDeferredProfileCreation(userHasVerifiedAge)",
            +[](csp::systems::UserSystem& self, const csp::common::Optional<bool>& userHasVerifiedAge) {
                return Promisify<PromiseOfLoginStateResult>(
                    [&](emscripten::val cb) { self.LoginAsGuestWithDeferredProfileCreation(userHasVerifiedAge, ToNativeCallback(cb.as<LoginStateResultCallback>())); });
            })
        .function(
            "getThirdPartyProviderAuthorizeUrl(authProvider, redirectUrl, clientType)",
            +[](csp::systems::UserSystem& self, csp::systems::EThirdPartyAuthenticationProviders authProvider, const csp::common::String& redirectUrl,
                 const csp::common::Optional<csp::systems::EThirdPartyPlatform>& clientType) {
                return Promisify<PromiseOfStringResult>(
                    [&](emscripten::val cb) { self.GetThirdPartyProviderAuthorizeURL(authProvider, redirectUrl, clientType, ToNativeCallback(cb.as<StringResultCallback>())); });
            })
        .function(
            "loginToThirdPartyAuthenticationProvider(thirdPartyToken, thirdPartyStateId, createMultiplayerConnection, userHasVerifiedAge, tokenOptions)",
            +[](csp::systems::UserSystem& self, const csp::common::String& thirdPartyToken, const csp::common::String& thirdPartyStateId, bool createMultiplayerConnection,
                 const csp::common::Optional<bool>& userHasVerifiedAge, const csp::common::Optional<csp::systems::TokenOptions>& tokenOptions) {
                return Promisify<PromiseOfLoginStateResult>([&](emscripten::val cb) {
                    self.LoginToThirdPartyAuthenticationProvider(
                        thirdPartyToken, thirdPartyStateId, createMultiplayerConnection, userHasVerifiedAge, tokenOptions, ToNativeCallback(cb.as<LoginStateResultCallback>()));
                });
            })
        .function(
            "loginToThirdPartyAuthenticationProviderWithToken(authProvider, thirdPartyToken, clientType, createMultiplayerConnection, userHasVerifiedAge)",
            +[](csp::systems::UserSystem& self, csp::systems::EThirdPartyAuthenticationProviders authProvider, const csp::common::String& thirdPartyToken,
                 const csp::common::Optional<csp::systems::EThirdPartyPlatform>& clientType, bool createMultiplayerConnection,
                 const csp::common::Optional<bool>& userHasVerifiedAge) {
                return Promisify<PromiseOfLoginStateResult>([&](emscripten::val cb) {
                    self.LoginToThirdPartyAuthenticationProviderWithToken(
                        authProvider, thirdPartyToken, clientType, createMultiplayerConnection, userHasVerifiedAge, ToNativeCallback(cb.as<LoginStateResultCallback>()));
                });
            })
        .function(
            "federatedLogin(federatedLoginDetailsJson, createMultiplayerConnection)",
            +[](csp::systems::UserSystem& self, const csp::common::String& federatedLoginDetailsJson, bool createMultiplayerConnection) {
                return Promisify<PromiseOfLoginStateResult>(
                    [&](emscripten::val cb) { self.FederatedLogin(federatedLoginDetailsJson, createMultiplayerConnection, ToNativeCallback(cb.as<LoginStateResultCallback>())); });
            })
        .function(
            "logout",
            +[](csp::systems::UserSystem& self) { return Promisify<PromiseOfNullResult>([&](emscripten::val cb) { self.Logout(ToNativeCallback(cb.as<NullResultCallback>())); }); })
        .function(
            "createUser(displayName, email, password, receiveNewsletter, userHasVerifiedAge, redirectUrl, inviteToken)",
            +[](csp::systems::UserSystem& self, const csp::common::Optional<csp::common::String>& displayName, const csp::common::String& email,
                 const csp::common::String& password, bool receiveNewsletter, bool userHasVerifiedAge, const csp::common::Optional<csp::common::String>& redirectUrl,
                 const csp::common::Optional<csp::common::String>& inviteToken) {
                return Promisify<PromiseOfProfileResult>([&](emscripten::val cb) {
                    self.CreateUser(
                        displayName, email, password, receiveNewsletter, userHasVerifiedAge, redirectUrl, inviteToken, ToNativeCallback(cb.as<ProfileResultCallback>()));
                });
            })
        .function(
            "upgradeGuestAccount(displayName, email, password)",
            +[](csp::systems::UserSystem& self, const csp::common::String& displayName, const csp::common::String& email, const csp::common::String& password) {
                return Promisify<PromiseOfProfileResult>(
                    [&](emscripten::val cb) { self.UpgradeGuestAccount(displayName, email, password, ToNativeCallback(cb.as<ProfileResultCallback>())); });
            })
        .function(
            "confirmUserEmail",
            +[](csp::systems::UserSystem& self) {
                return Promisify<PromiseOfNullResult>([&](emscripten::val cb) { self.ConfirmUserEmail(ToNativeCallback(cb.as<NullResultCallback>())); });
            })
        .function(
            "resetUserPassword(token, userId, newPassword)",
            +[](csp::systems::UserSystem& self, const csp::common::String& token, const csp::common::String& userId, const csp::common::String& newPassword) {
                return Promisify<PromiseOfNullResult>(
                    [&](emscripten::val cb) { self.ResetUserPassword(token, userId, newPassword, ToNativeCallback(cb.as<NullResultCallback>())); });
            })
        .function(
            "updateUserDisplayName(userId, newUserDisplayName)",
            +[](csp::systems::UserSystem& self, const csp::common::String& userId, const csp::common::String& newUserDisplayName) {
                return Promisify<PromiseOfNullResult>(
                    [&](emscripten::val cb) { self.UpdateUserDisplayName(userId, newUserDisplayName, ToNativeCallback(cb.as<NullResultCallback>())); });
            })
        .function(
            "deleteUser(userId)",
            +[](csp::systems::UserSystem& self, const csp::common::String& userId) {
                return Promisify<PromiseOfNullResult>([&](emscripten::val cb) { self.DeleteUser(userId, ToNativeCallback(cb.as<NullResultCallback>())); });
            })
        .function(
            "forgotPassword(email, redirectUrl, emailLinkUrl, useTokenChangePasswordUrl)",
            +[](csp::systems::UserSystem& self, const csp::common::String& email, const csp::common::Optional<csp::common::String>& redirectUrl,
                 const csp::common::Optional<csp::common::String>& emailLinkUrl, bool useTokenChangePasswordUrl) {
                return Promisify<PromiseOfNullResult>(
                    [&](emscripten::val cb) { self.ForgotPassword(email, redirectUrl, emailLinkUrl, useTokenChangePasswordUrl, ToNativeCallback(cb.as<NullResultCallback>())); });
            })
        .function(
            "getProfileByUserId(userId)",
            +[](csp::systems::UserSystem& self, const csp::common::String& userId) {
                return Promisify<PromiseOfProfileResult>([&](emscripten::val cb) { self.GetProfileByUserId(userId, ToNativeCallback(cb.as<ProfileResultCallback>())); });
            })
        .function(
            "getBasicProfilesByUserId(userIds)",
            +[](csp::systems::UserSystem& self, const csp::common::Array<csp::common::String>& userIds) {
                return Promisify<PromiseOfBasicProfilesResult>(
                    [&](emscripten::val cb) { self.GetBasicProfilesByUserId(userIds, ToNativeCallback(cb.as<BasicProfilesResultCallback>())); });
            })
        .function(
            "ping",
            +[](csp::systems::UserSystem& self) { return Promisify<PromiseOfNullResult>([&](emscripten::val cb) { self.Ping(ToNativeCallback(cb.as<NullResultCallback>())); }); })
        .function(
            "resendVerificationEmail(email, redirectUrl)",
            +[](csp::systems::UserSystem& self, const csp::common::String& email, const csp::common::Optional<csp::common::String>& redirectUrl) {
                return Promisify<PromiseOfNullResult>([&](emscripten::val cb) { self.ResendVerificationEmail(email, redirectUrl, ToNativeCallback(cb.as<NullResultCallback>())); });
            })
        .function(
            "getCustomerPortalUrl(userId)",
            +[](csp::systems::UserSystem& self, const csp::common::String& userId) {
                return Promisify<PromiseOfStringResult>([&](emscripten::val cb) { self.GetCustomerPortalUrl(userId, ToNativeCallback(cb.as<StringResultCallback>())); });
            })
        .function(
            "getCheckoutSessionUrl(tier)", +[](csp::systems::UserSystem& self, csp::systems::TierNames tier) {
                return Promisify<PromiseOfStringResult>([&](emscripten::val cb) { self.GetCheckoutSessionUrl(tier, ToNativeCallback(cb.as<StringResultCallback>())); });
            });
}
