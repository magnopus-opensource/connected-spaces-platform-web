#include "../../../containers/Array.h"
#include "../../../containers/String.h"
#include "../../../utils/JSDisposable.h"

#include "CSP/Common/Array.h"
#include "CSP/Common/String.h"
#include "CSP/Systems/HotspotSequence/HotspotGroup.h"
#include "CSP/Systems/WebService.h"

#include "emscripten/bind.h"

EMSCRIPTEN_BINDINGS(CSPHotspotGroup)
{
    emscripten::class_<csp::systems::HotspotGroup>("HotspotGroup")
        .class_function(
            "create", +[]() { return csp::systems::HotspotGroup(); })
        .property("name", &csp::systems::HotspotGroup::Name)
        .property("items", &csp::systems::HotspotGroup::Items)
        .function("equals(other)", &csp::systems::HotspotGroup::operator==);

    emscripten::class_<csp::systems::HotspotGroupResult, emscripten::base<csp::systems::ResultBase>>("HotspotGroupResult")
        .function(
            "getHotspotGroup", +[](const csp::systems::HotspotGroupResult& self) { return self.GetHotspotGroup(); });

    emscripten::class_<csp::systems::HotspotGroupsResult, emscripten::base<csp::systems::ResultBase>>("HotspotGroupsResult")
        .function(
            "getHotspotGroups", +[](const csp::systems::HotspotGroupsResult& self) {
                return bindings::utils::JSDisposable<csp::common::Array<csp::systems::HotspotGroup>> { self.GetHotspotGroups() };
            });
}
