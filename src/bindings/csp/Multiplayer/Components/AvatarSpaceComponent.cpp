#include "../../../containers/String.h"

#include "CSP/Common/SharedEnums.h"
#include "CSP/Common/String.h"
#include "CSP/Common/Vector.h"
#include "CSP/Multiplayer/Components/AvatarSpaceComponent.h"

#include "emscripten/bind.h"

EMSCRIPTEN_BINDINGS(CSPAvatarSpaceComponent)
{
    emscripten::enum_<csp::multiplayer::AvatarComponentPropertyKeys>("AvatarComponentPropertyKeys", emscripten::enum_value_type::number)
        .value("AvatarId", csp::multiplayer::AvatarComponentPropertyKeys::AvatarId)
        .value("UserId", csp::multiplayer::AvatarComponentPropertyKeys::UserId)
        .value("State", csp::multiplayer::AvatarComponentPropertyKeys::State)
        .value("AvatarMeshIndex_DEPRECATED", csp::multiplayer::AvatarComponentPropertyKeys::AvatarMeshIndex_DEPRECATED)
        .value("AgoraUserId", csp::multiplayer::AvatarComponentPropertyKeys::AgoraUserId)
        .value("CustomAvatarUrl_DEPRECATED", csp::multiplayer::AvatarComponentPropertyKeys::CustomAvatarUrl_DEPRECATED)
        .value("IsHandIKEnabled", csp::multiplayer::AvatarComponentPropertyKeys::IsHandIKEnabled)
        .value("TargetHandIKTargetLocation", csp::multiplayer::AvatarComponentPropertyKeys::TargetHandIKTargetLocation)
        .value("HandRotation", csp::multiplayer::AvatarComponentPropertyKeys::HandRotation)
        .value("HeadRotation", csp::multiplayer::AvatarComponentPropertyKeys::HeadRotation)
        .value("WalkRunBlendPercentage", csp::multiplayer::AvatarComponentPropertyKeys::WalkRunBlendPercentage)
        .value("TorsoTwistAlpha", csp::multiplayer::AvatarComponentPropertyKeys::TorsoTwistAlpha)
        .value("AvatarPlayMode", csp::multiplayer::AvatarComponentPropertyKeys::AvatarPlayMode)
        .value("MovementDirection", csp::multiplayer::AvatarComponentPropertyKeys::MovementDirection)
        .value("LocomotionModel", csp::multiplayer::AvatarComponentPropertyKeys::LocomotionModel)
        .value("IsVisible", csp::multiplayer::AvatarComponentPropertyKeys::IsVisible)
        .value("IsARVisible", csp::multiplayer::AvatarComponentPropertyKeys::IsARVisible)
        .value("IsVirtualVisible", csp::multiplayer::AvatarComponentPropertyKeys::IsVirtualVisible)
        .value("AvatarUrl", csp::multiplayer::AvatarComponentPropertyKeys::AvatarUrl)
        .value("Num", csp::multiplayer::AvatarComponentPropertyKeys::Num);

    emscripten::class_<csp::multiplayer::AvatarSpaceComponent, emscripten::base<csp::multiplayer::ComponentBase>>("AvatarSpaceComponent")
        .property("avatarId", &csp::multiplayer::AvatarSpaceComponent::GetAvatarId, &csp::multiplayer::AvatarSpaceComponent::SetAvatarId)
        .property("userId", &csp::multiplayer::AvatarSpaceComponent::GetUserId, &csp::multiplayer::AvatarSpaceComponent::SetUserId)
        .property("state", &csp::multiplayer::AvatarSpaceComponent::GetState, &csp::multiplayer::AvatarSpaceComponent::SetState)
        .property("avatarPlayMode", &csp::multiplayer::AvatarSpaceComponent::GetAvatarPlayMode, &csp::multiplayer::AvatarSpaceComponent::SetAvatarPlayMode)
        .property("agoraUserId", &csp::multiplayer::AvatarSpaceComponent::GetAgoraUserId, &csp::multiplayer::AvatarSpaceComponent::SetAgoraUserId)
        .property("isHandIKEnabled", &csp::multiplayer::AvatarSpaceComponent::GetIsHandIKEnabled, &csp::multiplayer::AvatarSpaceComponent::SetIsHandIKEnabled)
        .property("targetHandIKTargetLocation", &csp::multiplayer::AvatarSpaceComponent::GetTargetHandIKTargetLocation,
            &csp::multiplayer::AvatarSpaceComponent::SetTargetHandIKTargetLocation)
        .property("handRotation", &csp::multiplayer::AvatarSpaceComponent::GetHandRotation, &csp::multiplayer::AvatarSpaceComponent::SetHandRotation)
        .property("headRotation", &csp::multiplayer::AvatarSpaceComponent::GetHeadRotation, &csp::multiplayer::AvatarSpaceComponent::SetHeadRotation)
        .property("walkRunBlendPercentage", &csp::multiplayer::AvatarSpaceComponent::GetWalkRunBlendPercentage, &csp::multiplayer::AvatarSpaceComponent::SetWalkRunBlendPercentage)
        .property("torsoTwistAlpha", &csp::multiplayer::AvatarSpaceComponent::GetTorsoTwistAlpha, &csp::multiplayer::AvatarSpaceComponent::SetTorsoTwistAlpha)
        .property("movementDirection", &csp::multiplayer::AvatarSpaceComponent::GetMovementDirection, &csp::multiplayer::AvatarSpaceComponent::SetMovementDirection)
        .property("locomotionModel", &csp::multiplayer::AvatarSpaceComponent::GetLocomotionModel, &csp::multiplayer::AvatarSpaceComponent::SetLocomotionModel)
        .property("isVisible", &csp::multiplayer::AvatarSpaceComponent::GetIsVisible, &csp::multiplayer::AvatarSpaceComponent::SetIsVisible)
        .property("isARVisible", &csp::multiplayer::AvatarSpaceComponent::GetIsARVisible, &csp::multiplayer::AvatarSpaceComponent::SetIsARVisible)
        .property("isVirtualVisible", &csp::multiplayer::AvatarSpaceComponent::GetIsVirtualVisible, &csp::multiplayer::AvatarSpaceComponent::SetIsVirtualVisible)
        .property("avatarUrl", &csp::multiplayer::AvatarSpaceComponent::GetAvatarUrl, &csp::multiplayer::AvatarSpaceComponent::SetAvatarUrl);
}
