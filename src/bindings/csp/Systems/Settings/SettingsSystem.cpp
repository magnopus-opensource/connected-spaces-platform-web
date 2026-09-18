#include "../../../async/Promises.h"
#include "../../../containers/String.h"
#include "../../CallbackDeclarations.h"
#include "../../PromiseDeclarations.h"

#include "CSP/Common/String.h"
#include "CSP/Systems/Assets/Asset.h"
#include "CSP/Systems/Settings/SettingsCollection.h"
#include "CSP/Systems/Settings/SettingsSystem.h"
#include "CSP/Systems/SystemBase.h"

#include "emscripten/bind.h"
#include "emscripten/val.h"

namespace emscripten::internal {
template <> void raw_destructor<csp::systems::SettingsSystem>(csp::systems::SettingsSystem*) { }
}

EMSCRIPTEN_BINDINGS(CSPSettingsSystem)
{
    emscripten::class_<csp::systems::SettingsSystem, emscripten::base<csp::systems::SystemBase>>("SettingsSystem")
        .function(
            "setNdaStatus(value)",
            +[](csp::systems::SettingsSystem& self, bool value) {
                return Promisify<PromiseOfNullResult>([&](emscripten::val cb) { self.SetNDAStatus(value, ToNativeCallback(cb.as<NullResultCallback>())); });
            })
        .function(
            "getNdaStatus",
            +[](csp::systems::SettingsSystem& self) {
                return Promisify<PromiseOfBooleanResult>([&](emscripten::val cb) { self.GetNDAStatus(ToNativeCallback(cb.as<BooleanResultCallback>())); });
            })
        .function(
            "setNewsletterStatus(value)",
            +[](csp::systems::SettingsSystem& self, bool value) {
                return Promisify<PromiseOfNullResult>([&](emscripten::val cb) { self.SetNewsletterStatus(value, ToNativeCallback(cb.as<NullResultCallback>())); });
            })
        .function(
            "getNewsletterStatus",
            +[](csp::systems::SettingsSystem& self) {
                return Promisify<PromiseOfBooleanResult>([&](emscripten::val cb) { self.GetNewsletterStatus(ToNativeCallback(cb.as<BooleanResultCallback>())); });
            })
        .function(
            "addRecentlyVisitedSpace(spaceId)",
            +[](csp::systems::SettingsSystem& self, const csp::common::String& spaceId) {
                return Promisify<PromiseOfNullResult>([&](emscripten::val cb) { self.AddRecentlyVisitedSpace(spaceId, ToNativeCallback(cb.as<NullResultCallback>())); });
            })
        .function(
            "getRecentlyVisitedSpaces",
            +[](csp::systems::SettingsSystem& self) {
                return Promisify<PromiseOfStringArrayResult>([&](emscripten::val cb) { self.GetRecentlyVisitedSpaces(ToNativeCallback(cb.as<StringArrayResultCallback>())); });
            })
        .function(
            "clearRecentlyVisitedSpaces",
            +[](csp::systems::SettingsSystem& self) {
                return Promisify<PromiseOfNullResult>([&](emscripten::val cb) { self.ClearRecentlyVisitedSpaces(ToNativeCallback(cb.as<NullResultCallback>())); });
            })
        .function(
            "addBlockedSpace(spaceId)",
            +[](csp::systems::SettingsSystem& self, const csp::common::String& spaceId) {
                return Promisify<PromiseOfNullResult>([&](emscripten::val cb) { self.AddBlockedSpace(spaceId, ToNativeCallback(cb.as<NullResultCallback>())); });
            })
        .function(
            "removeBlockedSpace(spaceId)",
            +[](csp::systems::SettingsSystem& self, const csp::common::String& spaceId) {
                return Promisify<PromiseOfNullResult>([&](emscripten::val cb) { self.RemoveBlockedSpace(spaceId, ToNativeCallback(cb.as<NullResultCallback>())); });
            })
        .function(
            "getBlockedSpaces",
            +[](csp::systems::SettingsSystem& self) {
                return Promisify<PromiseOfStringArrayResult>([&](emscripten::val cb) { self.GetBlockedSpaces(ToNativeCallback(cb.as<StringArrayResultCallback>())); });
            })
        .function(
            "clearBlockedSpaces",
            +[](csp::systems::SettingsSystem& self) {
                return Promisify<PromiseOfNullResult>([&](emscripten::val cb) { self.ClearBlockedSpaces(ToNativeCallback(cb.as<NullResultCallback>())); });
            })
        .function(
            "updateAvatarPortrait(newAvatarPortrait)",
            +[](csp::systems::SettingsSystem& self, const csp::systems::FileAssetDataSource& newAvatarPortrait) {
                return Promisify<PromiseOfNullResult>([&](emscripten::val cb) { self.UpdateAvatarPortrait(newAvatarPortrait, ToNativeCallback(cb.as<NullResultCallback>())); });
            })
        .function(
            "getAvatarPortrait(userId)",
            +[](csp::systems::SettingsSystem& self, const csp::common::String& userId) {
                return Promisify<PromiseOfUriResult>([&](emscripten::val cb) { self.GetAvatarPortrait(userId, ToNativeCallback(cb.as<UriResultCallback>())); });
            })
        .function(
            "updateAvatarPortraitWithBuffer(newAvatarPortrait)",
            +[](csp::systems::SettingsSystem& self, const csp::systems::BufferAssetDataSource& newAvatarPortrait) {
                return Promisify<PromiseOfNullResult>(
                    [&](emscripten::val cb) { self.UpdateAvatarPortraitWithBuffer(newAvatarPortrait, ToNativeCallback(cb.as<NullResultCallback>())); });
            })
        .function(
            "setAvatarInfo(type, identifier, avatarVisible)",
            +[](csp::systems::SettingsSystem& self, csp::systems::AvatarType type, const csp::common::String& identifier, bool avatarVisible) {
                return Promisify<PromiseOfNullResult>(
                    [&](emscripten::val cb) { self.SetAvatarInfo(type, identifier, avatarVisible, ToNativeCallback(cb.as<NullResultCallback>())); });
            })
        .function(
            "getAvatarInfo", +[](csp::systems::SettingsSystem& self) {
                return Promisify<PromiseOfAvatarInfoResult>([&](emscripten::val cb) { self.GetAvatarInfo(ToNativeCallback(cb.as<AvatarInfoResultCallback>())); });
            });
}
