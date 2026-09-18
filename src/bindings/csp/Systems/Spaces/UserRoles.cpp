#include "../../../containers/Array.h"
#include "../../../containers/String.h"
#include "../../../utils/JSDisposable.h"

#include "CSP/Common/Array.h"
#include "CSP/Common/String.h"
#include "CSP/Systems/Spaces/UserRoles.h"
#include "CSP/Systems/WebService.h"

#include "emscripten/bind.h"
#include <utility>

EMSCRIPTEN_BINDINGS(CSPUserRoles)
{
    emscripten::enum_<csp::systems::SpaceUserRole>("SpaceUserRole", emscripten::enum_value_type::number)
        .value("Owner", csp::systems::SpaceUserRole::Owner)
        .value("Moderator", csp::systems::SpaceUserRole::Moderator)
        .value("User", csp::systems::SpaceUserRole::User)
        .value("Invalid", csp::systems::SpaceUserRole::Invalid);

    emscripten::class_<csp::systems::UserRoleInfo>("UserRoleInfo")
        .class_function(
            "create", +[]() { return csp::systems::UserRoleInfo(); })
        .property("userId", &csp::systems::UserRoleInfo::UserId)
        .property("userRole", &csp::systems::UserRoleInfo::UserRole)
        .function("equals(other)", &csp::systems::UserRoleInfo::operator==);

    emscripten::class_<csp::systems::InviteUserRoleInfo>("InviteUserRoleInfo")
        .class_function(
            "create", +[]() { return csp::systems::InviteUserRoleInfo(); })
        .property("userEmail", &csp::systems::InviteUserRoleInfo::UserEmail)
        .property("userRole", &csp::systems::InviteUserRoleInfo::UserRole)
        .function("equals(other)", &csp::systems::InviteUserRoleInfo::operator==);

    emscripten::class_<csp::systems::InviteUserRoleInfoCollection>("InviteUserRoleInfoCollection")
        .class_function(
            "create", +[]() { return csp::systems::InviteUserRoleInfoCollection(); })
        .property("emailLinkUrl", &csp::systems::InviteUserRoleInfoCollection::EmailLinkUrl)
        .property("signupUrl", &csp::systems::InviteUserRoleInfoCollection::SignupUrl)
        .function(
            "getInviteUserRoleInfos",
            +[](const csp::systems::InviteUserRoleInfoCollection& self) {
                return bindings::utils::JSDisposable<csp::common::Array<csp::systems::InviteUserRoleInfo>> { self.InviteUserRoleInfos };
            })
        .function(
            "setInviteUserRoleInfos(value)",
            +[](csp::systems::InviteUserRoleInfoCollection& self, csp::common::Array<csp::systems::InviteUserRoleInfo> value) { self.InviteUserRoleInfos = std::move(value); })
        .function("equals(other)", &csp::systems::InviteUserRoleInfoCollection::operator==);

    emscripten::class_<csp::systems::UserRoleCollectionResult, emscripten::base<csp::systems::ResultBase>>("UserRoleCollectionResult")
        .function(
            "getUsersRoles", +[](const csp::systems::UserRoleCollectionResult& self) {
                return bindings::utils::JSDisposable<csp::common::Array<csp::systems::UserRoleInfo>> { self.GetUsersRoles() };
            });
}
