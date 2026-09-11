#include "../../containers/List.h"
#include "../../containers/String.h"

#include "CSP/Common/List.h"
#include "CSP/Common/String.h"
#include "CSP/Multiplayer/CSPSceneDescription.h"

#include "emscripten/bind.h"
#include <emscripten/wire.h>

EMSCRIPTEN_BINDINGS(CSPSceneDescription)
{
    emscripten::class_<csp::multiplayer::CSPSceneDescription>("CSPSceneDescription")
        .class_function(
            // The new here seems inappropriate, we only do this because the object is non-copyable ... but why? Shouldn't it just be copyable.
            "create", +[]() { return new csp::multiplayer::CSPSceneDescription(); }, emscripten::allow_raw_pointers())
        .class_function(
            "createFromJson(sceneDescriptionJson)",
            +[](const csp::common::List<csp::common::String>& sceneDescriptionJson) { return csp::multiplayer::CSPSceneDescription(sceneDescriptionJson); });
}
