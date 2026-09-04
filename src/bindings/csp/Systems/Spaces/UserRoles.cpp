// Stub binding.

#include "CSP/Systems/Spaces/UserRoles.h"

#include "emscripten/bind.h"

EMSCRIPTEN_BINDINGS(CSPUserRoles)
{
    emscripten::enum_<csp::systems::SpaceUserRole>("SpaceUserRole", emscripten::enum_value_type::number)
        .value("Owner", csp::systems::SpaceUserRole::Owner)
        .value("Moderator", csp::systems::SpaceUserRole::Moderator)
        .value("User", csp::systems::SpaceUserRole::User)
        .value("Invalid", csp::systems::SpaceUserRole::Invalid);
}
