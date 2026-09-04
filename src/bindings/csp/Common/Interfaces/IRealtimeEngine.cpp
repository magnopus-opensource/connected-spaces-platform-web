#include "../../../async/Callbacks.h"
#include "../../../async/Promises.h"
#include "../../../containers/List.h"
#include "../../../containers/Optional.h"
#include "../../../containers/String.h"
#include "../../../utils/Handles.h"
#include "../../Multiplayer/SpaceEntity.h"

#include "CSP/Common/Interfaces/IRealtimeEngine.h"
#include "CSP/Common/List.h"
#include "CSP/Common/Optional.h"
#include "CSP/Common/SharedEnums.h"
#include "CSP/Common/String.h"
#include "CSP/Multiplayer/SpaceEntity.h"
#include "CSP/Multiplayer/SpaceTransform.h"

#include "emscripten/bind.h"
#include "emscripten/val.h"
#include <cstdint>

MAKE_CALLBACK(csp::multiplayer::EntityCreatedCallback, EntityCreatedCallback, "(entity: SpaceEntity | null) => void")
MAKE_CALLBACK(csp::multiplayer::CallbackHandler, BooleanCallback, "(success: boolean) => void")
MAKE_CALLBACK(csp::common::EntityFetchCompleteCallback, EntityFetchCompleteCallback, "(entityCount: number) => void")

EMSCRIPTEN_DECLARE_VAL_TYPE(PromiseOfSpaceEntityPointer);
EMSCRIPTEN_DECLARE_VAL_TYPE(PromiseOfBoolean);

EMSCRIPTEN_BINDINGS(CSPRealtimeEngine)
{
    emscripten::register_type<PromiseOfSpaceEntityPointer>("Promise<SpaceEntity | null>");
    emscripten::register_type<PromiseOfBoolean>("Promise<boolean>");

    emscripten::register_optional<uint64_t>();

    emscripten::register_type<csp::common::List<csp::multiplayer::SpaceEntity*>>("(SpaceEntity | null)[]");

    emscripten::enum_<csp::common::RealtimeEngineType>("RealtimeEngineType", emscripten::enum_value_type::number)
        .value("Online", csp::common::RealtimeEngineType::Online)
        .value("Offline", csp::common::RealtimeEngineType::Offline);

    emscripten::class_<csp::common::IRealtimeEngine>("IRealtimeEngine")
        .property("realtimeEngineType", &csp::common::IRealtimeEngine::GetRealtimeEngineType)
        .property("numEntities", &csp::common::IRealtimeEngine::GetNumEntities)
        .property("numAvatars", &csp::common::IRealtimeEngine::GetNumAvatars)
        .property("numObjects", &csp::common::IRealtimeEngine::GetNumObjects)
        .function(
            "setEntityFetchCompleteCallback(callback)",
            +[](csp::common::IRealtimeEngine& self, EntityFetchCompleteCallback callback) { self.SetEntityFetchCompleteCallback(ToNativeCallback(callback)); })
        .function(
            "findSpaceEntity(name)",
            +[](csp::common::IRealtimeEngine& self, const csp::common::String& name) { return bindings::utils::NonOwningVal<SpaceEntityPointer>(self.FindSpaceEntity(name)); })
        .function(
            "findSpaceEntityById(entityId)",
            +[](csp::common::IRealtimeEngine& self, uint64_t entityId) { return bindings::utils::NonOwningVal<SpaceEntityPointer>(self.FindSpaceEntityById(entityId)); })
        .function(
            "findSpaceAvatar(name)",
            +[](csp::common::IRealtimeEngine& self, const csp::common::String& name) { return bindings::utils::NonOwningVal<SpaceEntityPointer>(self.FindSpaceAvatar(name)); })
        .function(
            "findSpaceObject(name)",
            +[](csp::common::IRealtimeEngine& self, const csp::common::String& name) { return bindings::utils::NonOwningVal<SpaceEntityPointer>(self.FindSpaceObject(name)); })
        .function(
            "getEntityByIndex(entityIndex)",
            +[](csp::common::IRealtimeEngine& self, size_t entityIndex) { return bindings::utils::NonOwningVal<SpaceEntityPointer>(self.GetEntityByIndex(entityIndex)); })
        .function(
            "getAvatarByIndex(avatarIndex)",
            +[](csp::common::IRealtimeEngine& self, size_t avatarIndex) { return bindings::utils::NonOwningVal<SpaceEntityPointer>(self.GetAvatarByIndex(avatarIndex)); })
        .function(
            "getObjectByIndex(objectIndex)",
            +[](csp::common::IRealtimeEngine& self, size_t objectIndex) { return bindings::utils::NonOwningVal<SpaceEntityPointer>(self.GetObjectByIndex(objectIndex)); })
        .function(
            "getAllEntities",
            +[](const csp::common::IRealtimeEngine& self) {
                const csp::common::List<csp::multiplayer::SpaceEntity*>* entities = self.GetAllEntities();
                return entities != nullptr ? *entities : csp::common::List<csp::multiplayer::SpaceEntity*> { };
            })
        .function(
            "getRootHierarchyEntities",
            +[](const csp::common::IRealtimeEngine& self) {
                const csp::common::List<csp::multiplayer::SpaceEntity*>* entities = self.GetRootHierarchyEntities();
                return entities != nullptr ? *entities : csp::common::List<csp::multiplayer::SpaceEntity*> { };
            })
        .function(
            "isEntityModifiable(spaceEntity)",
            +[](const csp::common::IRealtimeEngine& self, csp::multiplayer::SpaceEntity* spaceEntity) { return self.IsEntityModifiable(spaceEntity); },
            emscripten::allow_raw_pointers())
        .function(
            "createAvatarAsync(name, userId, spaceTransform, isVisible, avatarState, avatarId, avatarPlayMode, locomotionModel)",
            +[](csp::common::IRealtimeEngine& self, const csp::common::String& name, const csp::common::String& userId, const csp::multiplayer::SpaceTransform& spaceTransform,
                 bool isVisible, csp::multiplayer::AvatarState avatarState, const csp::common::String& avatarId, csp::multiplayer::AvatarPlayMode avatarPlayMode,
                 csp::multiplayer::LocomotionModel locomotionModel) {
                return Promisify<PromiseOfSpaceEntityPointer>([&](emscripten::val cb) {
                    self.CreateAvatar(
                        name, userId, spaceTransform, isVisible, avatarState, avatarId, avatarPlayMode, locomotionModel, ToNativeCallback(cb.as<EntityCreatedCallback>()));
                });
            })
        .function(
            "createEntityAsync(name, spaceTransform, parentId)",
            +[](csp::common::IRealtimeEngine& self, const csp::common::String& name, const csp::multiplayer::SpaceTransform& spaceTransform,
                 const csp::common::Optional<uint64_t>& parentId) {
                return Promisify<PromiseOfSpaceEntityPointer>(
                    [&](emscripten::val cb) { self.CreateEntity(name, spaceTransform, parentId, ToNativeCallback(cb.as<EntityCreatedCallback>())); });
            })
        .function(
            "destroyEntityAsync(entity)",
            +[](csp::common::IRealtimeEngine& self, csp::multiplayer::SpaceEntity* entity) {
                return Promisify<PromiseOfBoolean>([&](emscripten::val cb) { self.DestroyEntity(entity, ToNativeCallback(cb.as<BooleanCallback>())); });
            },
            emscripten::allow_raw_pointers());
}
