#include "../../../containers/String.h"

#include "CSP/Common/String.h"
#include "CSP/Common/Vector.h"
#include "CSP/Multiplayer/Components/CinematicCameraSpaceComponent.h"

#include "emscripten/bind.h"

EMSCRIPTEN_BINDINGS(CSPCinematicCameraSpaceComponent)
{
    emscripten::enum_<csp::multiplayer::CinematicCameraPropertyKeys>("CinematicCameraPropertyKeys", emscripten::enum_value_type::number)
        .value("Position", csp::multiplayer::CinematicCameraPropertyKeys::Position)
        .value("Rotation", csp::multiplayer::CinematicCameraPropertyKeys::Rotation)
        .value("IsEnabled", csp::multiplayer::CinematicCameraPropertyKeys::IsEnabled)
        .value("FocalLength", csp::multiplayer::CinematicCameraPropertyKeys::FocalLength)
        .value("AspectRatio", csp::multiplayer::CinematicCameraPropertyKeys::AspectRatio)
        .value("SensorSize", csp::multiplayer::CinematicCameraPropertyKeys::SensorSize)
        .value("NearClip", csp::multiplayer::CinematicCameraPropertyKeys::NearClip)
        .value("FarClip", csp::multiplayer::CinematicCameraPropertyKeys::FarClip)
        .value("Iso", csp::multiplayer::CinematicCameraPropertyKeys::Iso)
        .value("ShutterSpeed", csp::multiplayer::CinematicCameraPropertyKeys::ShutterSpeed)
        .value("Aperture", csp::multiplayer::CinematicCameraPropertyKeys::Aperture)
        .value("IsViewerCamera", csp::multiplayer::CinematicCameraPropertyKeys::IsViewerCamera)
        .value("ThirdPartyComponentRef", csp::multiplayer::CinematicCameraPropertyKeys::ThirdPartyComponentRef)
        .value("FocusDistance", csp::multiplayer::CinematicCameraPropertyKeys::FocusDistance)
        .value("DepthOfFieldEnabled", csp::multiplayer::CinematicCameraPropertyKeys::DepthOfFieldEnabled)
        .value("Num", csp::multiplayer::CinematicCameraPropertyKeys::Num);

    emscripten::class_<csp::multiplayer::CinematicCameraSpaceComponent, emscripten::base<csp::multiplayer::ComponentBase>>("CinematicCameraSpaceComponent")
        .property("fov", &csp::multiplayer::CinematicCameraSpaceComponent::GetFov)
        .property("position", &csp::multiplayer::CinematicCameraSpaceComponent::GetPosition, &csp::multiplayer::CinematicCameraSpaceComponent::SetPosition)
        .property("rotation", &csp::multiplayer::CinematicCameraSpaceComponent::GetRotation, &csp::multiplayer::CinematicCameraSpaceComponent::SetRotation)
        .property("focalLength", &csp::multiplayer::CinematicCameraSpaceComponent::GetFocalLength, &csp::multiplayer::CinematicCameraSpaceComponent::SetFocalLength)
        .property("aspectRatio", &csp::multiplayer::CinematicCameraSpaceComponent::GetAspectRatio, &csp::multiplayer::CinematicCameraSpaceComponent::SetAspectRatio)
        .property("sensorSize", &csp::multiplayer::CinematicCameraSpaceComponent::GetSensorSize, &csp::multiplayer::CinematicCameraSpaceComponent::SetSensorSize)
        .property("nearClip", &csp::multiplayer::CinematicCameraSpaceComponent::GetNearClip, &csp::multiplayer::CinematicCameraSpaceComponent::SetNearClip)
        .property("farClip", &csp::multiplayer::CinematicCameraSpaceComponent::GetFarClip, &csp::multiplayer::CinematicCameraSpaceComponent::SetFarClip)
        .property("iso", &csp::multiplayer::CinematicCameraSpaceComponent::GetIso, &csp::multiplayer::CinematicCameraSpaceComponent::SetIso)
        .property("shutterSpeed", &csp::multiplayer::CinematicCameraSpaceComponent::GetShutterSpeed, &csp::multiplayer::CinematicCameraSpaceComponent::SetShutterSpeed)
        .property("aperture", &csp::multiplayer::CinematicCameraSpaceComponent::GetAperture, &csp::multiplayer::CinematicCameraSpaceComponent::SetAperture)
        .property("focusDistance", &csp::multiplayer::CinematicCameraSpaceComponent::GetFocusDistance, &csp::multiplayer::CinematicCameraSpaceComponent::SetFocusDistance)
        .property("depthOfFieldEnabled", &csp::multiplayer::CinematicCameraSpaceComponent::GetDepthOfFieldEnabled,
            &csp::multiplayer::CinematicCameraSpaceComponent::SetDepthOfFieldEnabled)
        .property("isViewerCamera", &csp::multiplayer::CinematicCameraSpaceComponent::GetIsViewerCamera, &csp::multiplayer::CinematicCameraSpaceComponent::SetIsViewerCamera)
        .property("isEnabled", &csp::multiplayer::CinematicCameraSpaceComponent::GetIsEnabled, &csp::multiplayer::CinematicCameraSpaceComponent::SetIsEnabled)
        .property("thirdPartyComponentRef", &csp::multiplayer::CinematicCameraSpaceComponent::GetThirdPartyComponentRef,
            &csp::multiplayer::CinematicCameraSpaceComponent::SetThirdPartyComponentRef);
}
