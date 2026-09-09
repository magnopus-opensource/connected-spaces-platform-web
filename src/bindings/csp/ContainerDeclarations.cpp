
#include "../containers/Array.h"
#include "../containers/List.h"
#include "../containers/Map.h"
#include "../containers/Optional.h"
#include "../containers/String.h"
#include "../utils/JSDisposable.h"

#include "CSP/Common/Array.h"
#include "CSP/Common/List.h"
#include "CSP/Common/Map.h"
#include "CSP/Common/ReplicatedValue.h"
#include "CSP/Common/Settings.h"
#include "CSP/Common/String.h"
#include "CSP/Multiplayer/SpaceEntity.h"
#include "CSP/Systems/Spaces/UserRoles.h"

#include "emscripten/bind.h"
#include <cstdint>

/*
 * Registrations for custom container types used in the bindings.
 * No paired header for this one, as there's no val types we need to declare, containers are fully mapped objects (See bindings/containers)
 *
 * Be aware that you need to declare a JSDisposable version of any container that is used in a return path. See the container mapping
 * headers themselves for more, or reference Interop-Types.md
 */

EMSCRIPTEN_BINDINGS(CSPContainerDeclarations)
{

    /* Common */

    // List
    emscripten::register_type<csp::common::List<csp::common::ApplicationSettings>>("ApplicationSettings[]");
    emscripten::register_type<bindings::utils::JSDisposable<csp::common::List<csp::common::ApplicationSettings>>>("(ApplicationSettings[] & Disposable)");
    emscripten::register_type<csp::common::List<csp::common::SettingsCollection>>("SettingsCollection[]");
    emscripten::register_type<bindings::utils::JSDisposable<csp::common::List<csp::common::SettingsCollection>>>("(SettingsCollection[] & Disposable)");

    // Array
    emscripten::register_type<csp::common::Array<csp::common::ReplicatedValue>>("ReplicatedValue[]");
    emscripten::register_type<bindings::utils::JSDisposable<csp::common::Array<csp::common::ReplicatedValue>>>("(ReplicatedValue[] & Disposable)");

    // Map
    emscripten::register_type<csp::common::Map<csp::common::String, csp::common::String>>("Map<string, string>");
    emscripten::register_type<csp::common::Map<csp::common::String, csp::common::ReplicatedValue>>("Map<string, ReplicatedValue>");
    emscripten::register_type<bindings::utils::JSDisposable<csp::common::Map<csp::common::String, csp::common::ReplicatedValue>>>("(Map<string, ReplicatedValue> & Disposable)");

    // Optional
    emscripten::register_optional<uint64_t>();

    /* Multiplayer*/

    // List
    emscripten::register_type<csp::common::List<csp::multiplayer::SpaceEntity*>>("(SpaceEntity | null)[]");
    // Array
    // Map
    // Optional

    /* Systems */

    // List
    // Array
    emscripten::register_type<csp::common::Array<csp::systems::SpaceUserRole>>("SpaceUserRole[]");
    // Map
    // Optional
}
