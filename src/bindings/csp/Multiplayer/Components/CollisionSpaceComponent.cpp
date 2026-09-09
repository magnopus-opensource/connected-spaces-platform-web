#include "../../../containers/String.h"

#include "CSP/Common/String.h"
#include "CSP/Common/Vector.h"
#include "CSP/Multiplayer/Components/CollisionSpaceComponent.h"

#include "emscripten/bind.h"

EMSCRIPTEN_BINDINGS(CSPCollisionSpaceComponent)
{
    emscripten::enum_<csp::multiplayer::CollisionPropertyKeys>("CollisionPropertyKeys", emscripten::enum_value_type::number)
        .value("Position", csp::multiplayer::CollisionPropertyKeys::Position)
        .value("Rotation", csp::multiplayer::CollisionPropertyKeys::Rotation)
        .value("Scale", csp::multiplayer::CollisionPropertyKeys::Scale)
        .value("CollisionShape", csp::multiplayer::CollisionPropertyKeys::CollisionShape)
        .value("CollisionMode", csp::multiplayer::CollisionPropertyKeys::CollisionMode)
        .value("CollisionAssetId", csp::multiplayer::CollisionPropertyKeys::CollisionAssetId)
        .value("AssetCollectionId", csp::multiplayer::CollisionPropertyKeys::AssetCollectionId)
        .value("ThirdPartyComponentRef", csp::multiplayer::CollisionPropertyKeys::ThirdPartyComponentRef)
        .value("IsEnabled", csp::multiplayer::CollisionPropertyKeys::IsEnabled)
        .value("Num", csp::multiplayer::CollisionPropertyKeys::Num);

    emscripten::enum_<csp::multiplayer::CollisionShape>("CollisionShape", emscripten::enum_value_type::number)
        .value("Box", csp::multiplayer::CollisionShape::Box)
        .value("Mesh", csp::multiplayer::CollisionShape::Mesh)
        .value("Capsule", csp::multiplayer::CollisionShape::Capsule)
        .value("Sphere", csp::multiplayer::CollisionShape::Sphere);

    emscripten::enum_<csp::multiplayer::CollisionMode>("CollisionMode", emscripten::enum_value_type::number)
        .value("Collision", csp::multiplayer::CollisionMode::Collision)
        .value("Trigger", csp::multiplayer::CollisionMode::Trigger);

    emscripten::class_<csp::multiplayer::CollisionSpaceComponent, emscripten::base<csp::multiplayer::ComponentBase>>("CollisionSpaceComponent")
        .class_function(
            "defaultSphereRadius", +[]() { return csp::multiplayer::CollisionSpaceComponent::GetDefaultSphereRadius(); })
        .class_function(
            "defaultCapsuleHalfWidth", +[]() { return csp::multiplayer::CollisionSpaceComponent::GetDefaultCapsuleHalfWidth(); })
        .class_function(
            "defaultCapsuleHalfHeight", +[]() { return csp::multiplayer::CollisionSpaceComponent::GetDefaultCapsuleHalfHeight(); })
        .property("position", &csp::multiplayer::CollisionSpaceComponent::GetPosition, &csp::multiplayer::CollisionSpaceComponent::SetPosition)
        .property("rotation", &csp::multiplayer::CollisionSpaceComponent::GetRotation, &csp::multiplayer::CollisionSpaceComponent::SetRotation)
        .property("scale", &csp::multiplayer::CollisionSpaceComponent::GetScale, &csp::multiplayer::CollisionSpaceComponent::SetScale)
        .property("transform", &csp::multiplayer::CollisionSpaceComponent::GetTransform, &csp::multiplayer::CollisionSpaceComponent::SetTransform)
        .property("collisionShape", &csp::multiplayer::CollisionSpaceComponent::GetCollisionShape, &csp::multiplayer::CollisionSpaceComponent::SetCollisionShape)
        .property("collisionMode", &csp::multiplayer::CollisionSpaceComponent::GetCollisionMode, &csp::multiplayer::CollisionSpaceComponent::SetCollisionMode)
        .property("collisionAssetId", &csp::multiplayer::CollisionSpaceComponent::GetCollisionAssetId, &csp::multiplayer::CollisionSpaceComponent::SetCollisionAssetId)
        .property("assetCollectionId", &csp::multiplayer::CollisionSpaceComponent::GetAssetCollectionId, &csp::multiplayer::CollisionSpaceComponent::SetAssetCollectionId)
        .property(
            "thirdPartyComponentRef", &csp::multiplayer::CollisionSpaceComponent::GetThirdPartyComponentRef, &csp::multiplayer::CollisionSpaceComponent::SetThirdPartyComponentRef)
        .property("isEnabled", &csp::multiplayer::CollisionSpaceComponent::GetIsEnabled, &csp::multiplayer::CollisionSpaceComponent::SetIsEnabled)
        .function("getUnscaledBoundingBoxMin", &csp::multiplayer::CollisionSpaceComponent::GetUnscaledBoundingBoxMin)
        .function("getUnscaledBoundingBoxMax", &csp::multiplayer::CollisionSpaceComponent::GetUnscaledBoundingBoxMax)
        .function("getScaledBoundingBoxMin", &csp::multiplayer::CollisionSpaceComponent::GetScaledBoundingBoxMin)
        .function("getScaledBoundingBoxMax", &csp::multiplayer::CollisionSpaceComponent::GetScaledBoundingBoxMax);
}
