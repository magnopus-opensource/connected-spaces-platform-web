#include "../../../containers/Array.h"
#include "../../../containers/String.h"

#include "CSP/Common/Array.h"
#include "CSP/Common/String.h"
#include "CSP/Systems/Users/Profile.h"
#include "CSP/Systems/WebService.h"

#include "emscripten/bind.h"

EMSCRIPTEN_BINDINGS(CSPProfile)
{
    emscripten::value_object<csp::systems::BasicProfile>("BasicProfile")
        .field("userId", &csp::systems::BasicProfile::UserId)
        .field("displayName", &csp::systems::BasicProfile::DisplayName)
        .field("avatarId", &csp::systems::BasicProfile::AvatarId);

    // Technically Profile inherits from BasicProfile, but it's vapid inheritance that is never used, just redeclare.
    emscripten::value_object<csp::systems::Profile>("Profile")
        .field("userId", &csp::systems::Profile::UserId)
        .field("displayName", &csp::systems::Profile::DisplayName)
        .field("avatarId", &csp::systems::Profile::AvatarId)
        .field("email", &csp::systems::Profile::Email)
        .field("isEmailConfirmed", &csp::systems::Profile::IsEmailConfirmed)
        .field("roles", &csp::systems::Profile::Roles)
        .field("lastDeviceId", &csp::systems::Profile::LastDeviceId)
        .field("createdBy", &csp::systems::Profile::CreatedBy)
        .field("createdAt", &csp::systems::Profile::CreatedAt)
        .field("updatedBy", &csp::systems::Profile::UpdatedBy)
        .field("updatedAt", &csp::systems::Profile::UpdatedAt);

    emscripten::class_<csp::systems::ProfileResult, emscripten::base<csp::systems::ResultBase>>("ProfileResult")
        .function("getProfile", +[](const csp::systems::ProfileResult& self) { return self.GetProfile(); });

    emscripten::class_<csp::systems::BasicProfilesResult, emscripten::base<csp::systems::ResultBase>>("BasicProfilesResult")
        .function("getProfiles", +[](const csp::systems::BasicProfilesResult& self) { return self.GetProfiles(); });
}
