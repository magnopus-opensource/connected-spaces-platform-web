#include "../../async/Promises.h"
#include "../../containers/List.h"
#include "../../containers/Map.h"
#include "../../containers/String.h"
#include "../../utils/Handles.h"
#include "../CallbackDeclarations.h"
#include "../PointerDeclarations.h"
#include "../PromiseDeclarations.h"

#include "CSP/Common/List.h"
#include "CSP/Common/Map.h"
#include "CSP/Common/SharedEnums.h"
#include "CSP/Common/String.h"
#include "CSP/Multiplayer/ComponentBase.h"
#include "CSP/Multiplayer/Script/EntityScript.h"
#include "CSP/Multiplayer/SpaceEntity.h"
#include "CSP/Multiplayer/SpaceTransform.h"

#include "emscripten/bind.h"
#include "emscripten/val.h"
#include <cstdint>

EMSCRIPTEN_BINDINGS(CSPSpaceEntity)
{
    emscripten::enum_<csp::multiplayer::SpaceEntityType>("SpaceEntityType", emscripten::enum_value_type::number)
        .value("Avatar", csp::multiplayer::SpaceEntityType::Avatar)
        .value("Object", csp::multiplayer::SpaceEntityType::Object);

    emscripten::enum_<csp::multiplayer::LockType>("LockType", emscripten::enum_value_type::number)
        .value("None", csp::multiplayer::LockType::None)
        .value("UserAgnostic", csp::multiplayer::LockType::UserAgnostic);

    emscripten::class_<csp::multiplayer::SpaceEntity>("SpaceEntity")
        .property("id", &csp::multiplayer::SpaceEntity::GetId)
        .property("ownerId", &csp::multiplayer::SpaceEntity::GetOwnerId)
        .property("entityType", &csp::multiplayer::SpaceEntity::GetEntityType)
        .property("isTransient", &csp::multiplayer::SpaceEntity::GetIsTransient)
        .property("isSelected", &csp::multiplayer::SpaceEntity::IsSelected)
        .property("selectingClientId", &csp::multiplayer::SpaceEntity::GetSelectingClientID)
        .property("isLocked", &csp::multiplayer::SpaceEntity::IsLocked)
        .property("lockType", &csp::multiplayer::SpaceEntity::GetLockType)
        .property("isModifiable", &csp::multiplayer::SpaceEntity::IsModifiable)
        .property("transform", &csp::multiplayer::SpaceEntity::GetTransform)
        .property("globalTransform", &csp::multiplayer::SpaceEntity::GetGlobalTransform)
        .property("globalPosition", &csp::multiplayer::SpaceEntity::GetGlobalPosition)
        .property("globalRotation", &csp::multiplayer::SpaceEntity::GetGlobalRotation)
        .property("globalScale", &csp::multiplayer::SpaceEntity::GetGlobalScale)
        // Below are get/setters rather than properties because they have a boolean return (entity might be locked)
        .function("getName", &csp::multiplayer::SpaceEntity::GetName)
        .function("setName(value)", &csp::multiplayer::SpaceEntity::SetName)
        .function("getPosition", &csp::multiplayer::SpaceEntity::GetPosition)
        .function("setPosition(value)", &csp::multiplayer::SpaceEntity::SetPosition)
        .function("getRotation", &csp::multiplayer::SpaceEntity::GetRotation)
        .function("setRotation(value)", &csp::multiplayer::SpaceEntity::SetRotation)
        .function("getScale", &csp::multiplayer::SpaceEntity::GetScale)
        .function("setScale(value)", &csp::multiplayer::SpaceEntity::SetScale)
        .function("getThirdPartyRef", &csp::multiplayer::SpaceEntity::GetThirdPartyRef)
        .function("setThirdPartyRef(value)", &csp::multiplayer::SpaceEntity::SetThirdPartyRef)
        .function("setParentId(parentId)", &csp::multiplayer::SpaceEntity::SetParentId)
        .function("removeParentEntity", &csp::multiplayer::SpaceEntity::RemoveParentEntity)
        .function(
            "getParentEntity", +[](const csp::multiplayer::SpaceEntity& self) { return bindings::utils::NonOwningVal<SpaceEntityPointer>(self.GetParentEntity()); })
        .function(
            "getChildEntities",
            +[](const csp::multiplayer::SpaceEntity& self) {
                const csp::common::List<csp::multiplayer::SpaceEntity*>* childEntities = self.GetChildEntities();
                return childEntities != nullptr ? *childEntities : csp::common::List<csp::multiplayer::SpaceEntity*> { };
            })
        .function(
            "createChildEntity(name, spaceTransform)",
            +[](csp::multiplayer::SpaceEntity& self, const csp::common::String& name, const csp::multiplayer::SpaceTransform& spaceTransform) {
                return Promisify<PromiseOfSpaceEntityPointer>(
                    [&](emscripten::val cb) { self.CreateChildEntity(name, spaceTransform, ToNativeCallback(cb.as<EntityCreatedCallback>())); });
            })
        .function(
            "destroy",
            +[](csp::multiplayer::SpaceEntity& self) { return Promisify<PromiseOfBoolean>([&](emscripten::val cb) { self.Destroy(ToNativeCallback(cb.as<BooleanCallback>())); }); })
        .function(
            "setUpdateCallback(callback)", +[](csp::multiplayer::SpaceEntity& self, SpaceEntityUpdateCallback callback) { self.SetUpdateCallback(ToNativeCallback(callback)); })
        .function(
            "setDestroyCallback(callback)", +[](csp::multiplayer::SpaceEntity& self, BooleanCallback callback) { self.SetDestroyCallback(ToNativeCallback(callback)); })
        .function(
            "setPatchSentCallback(callback)", +[](csp::multiplayer::SpaceEntity& self, BooleanCallback callback) { self.SetPatchSentCallback(ToNativeCallback(callback)); })
        .function(
            "findFirstComponentOfType(componentType)",
            +[](const csp::multiplayer::SpaceEntity& self, csp::multiplayer::ComponentType componentType) {
                return bindings::utils::NonOwningVal<ComponentBasePointer>(self.FindFirstComponentOfType(componentType));
            })
        .function(
            "getComponents",
            +[](const csp::multiplayer::SpaceEntity& self) {
                const csp::common::Map<uint16_t, csp::multiplayer::ComponentBase*>* components = self.GetComponents();
                return components != nullptr ? *components : csp::common::Map<uint16_t, csp::multiplayer::ComponentBase*> { };
            })
        .function(
            "getComponent(key)", +[](csp::multiplayer::SpaceEntity& self, uint16_t key) { return bindings::utils::NonOwningVal<ComponentBasePointer>(self.GetComponent(key)); })
        .function(
            "addComponent(componentType)",
            +[](csp::multiplayer::SpaceEntity& self, csp::multiplayer::ComponentType componentType) {
                return bindings::utils::NonOwningVal<ComponentBasePointer>(self.AddComponent(componentType));
            })
        .function(
            "addComponentByTypeId(typeId)",
            +[](csp::multiplayer::SpaceEntity& self, uint64_t typeId) { return bindings::utils::NonOwningVal<ComponentBasePointer>(self.AddComponentByTypeId(typeId)); })
        .function("removeComponent(key)", &csp::multiplayer::SpaceEntity::RemoveComponent)
        .function(
            "getScript", +[](csp::multiplayer::SpaceEntity& self) { return bindings::utils::NonOwningVal<EntityScriptReference>(self.GetScript()); })
        .function("select", &csp::multiplayer::SpaceEntity::Select)
        .function("deselect", &csp::multiplayer::SpaceEntity::Deselect)
        .function("lock", &csp::multiplayer::SpaceEntity::Lock)
        .function("unlock", &csp::multiplayer::SpaceEntity::Unlock)
        .function("queueUpdate", &csp::multiplayer::SpaceEntity::QueueUpdate);
}
