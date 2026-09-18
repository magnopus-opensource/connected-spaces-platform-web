#include "../../../containers/Array.h"
#include "../../../containers/String.h"
#include "../../../utils/JSDisposable.h"

#include "CSP/Common/Array.h"
#include "CSP/Common/String.h"
#include "CSP/Systems/Users/Profile.h"
#include "CSP/Systems/WebService.h"

#include "emscripten/bind.h"

EMSCRIPTEN_BINDINGS(CSPProfile)
{
    emscripten::class_<csp::systems::BasicProfile>("BasicProfile")
        .class_function(
            "create", +[]() { return csp::systems::BasicProfile(); })
        .property("userId", &csp::systems::BasicProfile::UserId)
        .property("displayName", &csp::systems::BasicProfile::DisplayName)
        .property("avatarId", &csp::systems::BasicProfile::AvatarId)
        .function("equals(other)", &csp::systems::BasicProfile::operator==);

    emscripten::class_<csp::systems::Profile, emscripten::base<csp::systems::BasicProfile>>("Profile")
        .class_function(
            "create", +[]() { return csp::systems::Profile(); })
        .property("email", &csp::systems::Profile::Email)
        .property("isEmailConfirmed", &csp::systems::Profile::IsEmailConfirmed)
        .property("roles", &csp::systems::Profile::Roles)
        .property("lastDeviceId", &csp::systems::Profile::LastDeviceId)
        .property("createdBy", &csp::systems::Profile::CreatedBy)
        .property("createdAt", &csp::systems::Profile::CreatedAt)
        .property("updatedBy", &csp::systems::Profile::UpdatedBy)
        .property("updatedAt", &csp::systems::Profile::UpdatedAt)
        .function("equals(other)", &csp::systems::Profile::operator==);

    emscripten::class_<csp::systems::ProfileResult, emscripten::base<csp::systems::ResultBase>>("ProfileResult")
        .function(
            "getProfile", +[](const csp::systems::ProfileResult& self) { return self.GetProfile(); });

    emscripten::class_<csp::systems::BasicProfilesResult, emscripten::base<csp::systems::ResultBase>>("BasicProfilesResult")
        .function(
            "getProfiles",
            +[](const csp::systems::BasicProfilesResult& self) { return bindings::utils::JSDisposable<csp::common::Array<csp::systems::BasicProfile>> { self.GetProfiles() }; });
}
