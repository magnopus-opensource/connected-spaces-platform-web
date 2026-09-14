#include "../../../containers/String.h"

#include "CSP/Common/Settings.h"
#include "CSP/Common/String.h"
#include "CSP/Systems/Settings/SettingsCollection.h"
#include "CSP/Systems/WebService.h"

#include "emscripten/bind.h"

EMSCRIPTEN_BINDINGS(CSPSettingsCollection)
{
    emscripten::enum_<csp::systems::AvatarType>("AvatarType", emscripten::enum_value_type::number)
        .value("None", csp::systems::AvatarType::None)
        .value("Premade", csp::systems::AvatarType::Premade)
        .value("ReadyPlayerMe", csp::systems::AvatarType::ReadyPlayerMe)
        .value("Custom", csp::systems::AvatarType::Custom);

    emscripten::class_<csp::systems::SettingsCollectionResult, emscripten::base<csp::systems::ResultBase>>("SettingsCollectionResult")
        .function(
            "getSettingsCollection", +[](const csp::systems::SettingsCollectionResult& self) { return self.GetSettingsCollection(); });

    emscripten::class_<csp::systems::AvatarInfoResult, emscripten::base<csp::systems::ResultBase>>("AvatarInfoResult")
        .property("avatarType", &csp::systems::AvatarInfoResult::GetAvatarType)
        .property("avatarIdentifier", &csp::systems::AvatarInfoResult::GetAvatarIdentifier)
        .property("avatarVisible", &csp::systems::AvatarInfoResult::GetAvatarVisible);
}
