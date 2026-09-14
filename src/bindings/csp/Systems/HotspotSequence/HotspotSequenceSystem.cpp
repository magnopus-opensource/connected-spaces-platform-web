#include "../../../async/Promises.h"
#include "../../../containers/Array.h"
#include "../../../containers/String.h"
#include "../../CallbackDeclarations.h"
#include "../../PromiseDeclarations.h"

#include "CSP/Common/Array.h"
#include "CSP/Common/String.h"
#include "CSP/Systems/HotspotSequence/HotspotGroup.h"
#include "CSP/Systems/HotspotSequence/HotspotSequenceSystem.h"
#include "CSP/Systems/SystemBase.h"

#include "emscripten/bind.h"
#include "emscripten/val.h"

EMSCRIPTEN_BINDINGS(CSPHotspotSequenceSystem)
{
    emscripten::class_<csp::systems::HotspotSequenceSystem, emscripten::base<csp::systems::SystemBase>>("HotspotSequenceSystem")
        .function(
            "createHotspotGroup(groupName, hotspotIds)",
            +[](csp::systems::HotspotSequenceSystem& self, const csp::common::String& groupName, const csp::common::Array<csp::common::String>& hotspotIds) {
                return Promisify<PromiseOfHotspotGroupResult>(
                    [&](emscripten::val cb) { self.CreateHotspotGroup(groupName, hotspotIds, ToNativeCallback(cb.as<HotspotGroupResultCallback>())); });
            })
        .function(
            "renameHotspotGroup(groupName, newGroupName)",
            +[](csp::systems::HotspotSequenceSystem& self, const csp::common::String& groupName, const csp::common::String& newGroupName) {
                return Promisify<PromiseOfHotspotGroupResult>(
                    [&](emscripten::val cb) { self.RenameHotspotGroup(groupName, newGroupName, ToNativeCallback(cb.as<HotspotGroupResultCallback>())); });
            })
        .function(
            "updateHotspotGroup(groupName, hotspotIds)",
            +[](csp::systems::HotspotSequenceSystem& self, const csp::common::String& groupName, const csp::common::Array<csp::common::String>& hotspotIds) {
                return Promisify<PromiseOfHotspotGroupResult>(
                    [&](emscripten::val cb) { self.UpdateHotspotGroup(groupName, hotspotIds, ToNativeCallback(cb.as<HotspotGroupResultCallback>())); });
            })
        .function(
            "getHotspotGroup(groupName)",
            +[](csp::systems::HotspotSequenceSystem& self, const csp::common::String& groupName) {
                return Promisify<PromiseOfHotspotGroupResult>([&](emscripten::val cb) { self.GetHotspotGroup(groupName, ToNativeCallback(cb.as<HotspotGroupResultCallback>())); });
            })
        .function(
            "getHotspotGroups",
            +[](csp::systems::HotspotSequenceSystem& self) {
                return Promisify<PromiseOfHotspotGroupsResult>([&](emscripten::val cb) { self.GetHotspotGroups(ToNativeCallback(cb.as<HotspotGroupsResultCallback>())); });
            })
        .function(
            "deleteHotspotGroup(groupName)",
            +[](csp::systems::HotspotSequenceSystem& self, const csp::common::String& groupName) {
                return Promisify<PromiseOfNullResult>([&](emscripten::val cb) { self.DeleteHotspotGroup(groupName, ToNativeCallback(cb.as<NullResultCallback>())); });
            })
        .function(
            "removeItemFromGroups(itemId)",
            +[](csp::systems::HotspotSequenceSystem& self, const csp::common::String& itemId) {
                return Promisify<PromiseOfNullResult>([&](emscripten::val cb) { self.RemoveItemFromGroups(itemId, ToNativeCallback(cb.as<NullResultCallback>())); });
            })
        .function(
            "setHotspotSequenceChangedCallback(callback)",
            +[](csp::systems::HotspotSequenceSystem& self, SequenceChangedEventCallback callback) { self.SetHotspotSequenceChangedCallback(ToNativeCallback(callback)); });
}
