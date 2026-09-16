#include "../../../containers/Array.h"
#include "../../../containers/String.h"

#include "CSP/Common/Array.h"
#include "CSP/Common/String.h"
#include "CSP/Systems/Users/ThirdPartyAuthentication.h"
#include "CSP/Systems/WebService.h"

#include "emscripten/bind.h"

EMSCRIPTEN_BINDINGS(CSPThirdPartyAuthentication)
{
    emscripten::enum_<csp::systems::EThirdPartyAuthenticationProviders>("EThirdPartyAuthenticationProviders", emscripten::enum_value_type::number)
        .value("Google", csp::systems::EThirdPartyAuthenticationProviders::Google)
        .value("Discord", csp::systems::EThirdPartyAuthenticationProviders::Discord)
        .value("Apple", csp::systems::EThirdPartyAuthenticationProviders::Apple)
        .value("Netflix", csp::systems::EThirdPartyAuthenticationProviders::Netflix)
        .value("Num", csp::systems::EThirdPartyAuthenticationProviders::Num)
        .value("Invalid", csp::systems::EThirdPartyAuthenticationProviders::Invalid);

    emscripten::class_<csp::systems::ThirdPartyProviderDetails>("ThirdPartyProviderDetails")
        .class_function(
            "create", +[]() { return csp::systems::ThirdPartyProviderDetails(); })
        .property("providerName", &csp::systems::ThirdPartyProviderDetails::ProviderName)
        .property("providerClientId", &csp::systems::ThirdPartyProviderDetails::ProviderClientId)
        .property("providerAuthScopes", &csp::systems::ThirdPartyProviderDetails::ProviderAuthScopes)
        .property("providerAuthUrl", &csp::systems::ThirdPartyProviderDetails::ProviderAuthURL)
        .property("thirdPartyAuthStateId", &csp::systems::ThirdPartyProviderDetails::ThirdPartyAuthStateId)
        .property("providerRedirectUrl", &csp::systems::ThirdPartyProviderDetails::ProviderRedirectURL);
}
