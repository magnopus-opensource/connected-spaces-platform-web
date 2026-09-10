#include "../../../containers/String.h"

#include "CSP/Common/SharedEnums.h"
#include "CSP/Common/String.h"
#include "CSP/Common/Vector.h"
#include "CSP/Multiplayer/Components/VideoPlayerSpaceComponent.h"

#include "emscripten/bind.h"

EMSCRIPTEN_BINDINGS(CSPVideoPlayerSpaceComponent)
{
    emscripten::enum_<csp::multiplayer::VideoPlayerPlaybackState>("VideoPlayerPlaybackState", emscripten::enum_value_type::number)
        .value("Reset", csp::multiplayer::VideoPlayerPlaybackState::Reset)
        .value("Pause", csp::multiplayer::VideoPlayerPlaybackState::Pause)
        .value("Play", csp::multiplayer::VideoPlayerPlaybackState::Play)
        .value("Num", csp::multiplayer::VideoPlayerPlaybackState::Num);

    emscripten::enum_<csp::multiplayer::VideoPlayerActions>("VideoPlayerActions", emscripten::enum_value_type::number)
        .value("VideoBegin", csp::multiplayer::VideoPlayerActions::VideoBegin)
        .value("VideoEnd", csp::multiplayer::VideoPlayerActions::VideoEnd)
        .value("Num", csp::multiplayer::VideoPlayerActions::Num);

    emscripten::enum_<csp::multiplayer::VideoPlayerSourceType>("VideoPlayerSourceType", emscripten::enum_value_type::number)
        .value("URLSource", csp::multiplayer::VideoPlayerSourceType::URLSource)
        .value("AssetSource", csp::multiplayer::VideoPlayerSourceType::AssetSource)
        .value("WowzaStreamSource", csp::multiplayer::VideoPlayerSourceType::WowzaStreamSource)
        .value("Num", csp::multiplayer::VideoPlayerSourceType::Num);

    emscripten::enum_<csp::multiplayer::VideoPlayerPropertyKeys>("VideoPlayerPropertyKeys", emscripten::enum_value_type::number)
        .value("VideoAssetId", csp::multiplayer::VideoPlayerPropertyKeys::VideoAssetId)
        .value("VideoAssetURL", csp::multiplayer::VideoPlayerPropertyKeys::VideoAssetURL)
        .value("AssetCollectionId", csp::multiplayer::VideoPlayerPropertyKeys::AssetCollectionId)
        .value("Position", csp::multiplayer::VideoPlayerPropertyKeys::Position)
        .value("Rotation", csp::multiplayer::VideoPlayerPropertyKeys::Rotation)
        .value("Scale", csp::multiplayer::VideoPlayerPropertyKeys::Scale)
        .value("IsStateShared", csp::multiplayer::VideoPlayerPropertyKeys::IsStateShared)
        .value("IsAutoPlay", csp::multiplayer::VideoPlayerPropertyKeys::IsAutoPlay)
        .value("IsLoopPlayback", csp::multiplayer::VideoPlayerPropertyKeys::IsLoopPlayback)
        .value("IsAutoResize", csp::multiplayer::VideoPlayerPropertyKeys::IsAutoResize)
        .value("AttenuationRadius", csp::multiplayer::VideoPlayerPropertyKeys::AttenuationRadius)
        .value("PlaybackState", csp::multiplayer::VideoPlayerPropertyKeys::PlaybackState)
        .value("CurrentPlayheadPosition", csp::multiplayer::VideoPlayerPropertyKeys::CurrentPlayheadPosition)
        .value("TimeSincePlay", csp::multiplayer::VideoPlayerPropertyKeys::TimeSincePlay)
        .value("VideoPlayerSourceType", csp::multiplayer::VideoPlayerPropertyKeys::VideoPlayerSourceType)
        .value("IsVisible", csp::multiplayer::VideoPlayerPropertyKeys::IsVisible)
        .value("IsARVisible", csp::multiplayer::VideoPlayerPropertyKeys::IsARVisible)
        .value("MeshComponentId", csp::multiplayer::VideoPlayerPropertyKeys::MeshComponentId)
        .value("IsEnabled", csp::multiplayer::VideoPlayerPropertyKeys::IsEnabled)
        .value("IsVirtualVisible", csp::multiplayer::VideoPlayerPropertyKeys::IsVirtualVisible)
        .value("StereoVideoType", csp::multiplayer::VideoPlayerPropertyKeys::StereoVideoType)
        .value("IsStereoFlipped", csp::multiplayer::VideoPlayerPropertyKeys::IsStereoFlipped)
        .value("Volume", csp::multiplayer::VideoPlayerPropertyKeys::Volume)
        .value("AudioType", csp::multiplayer::VideoPlayerPropertyKeys::AudioType)
        .value("Num", csp::multiplayer::VideoPlayerPropertyKeys::Num);

    emscripten::class_<csp::multiplayer::VideoPlayerSpaceComponent, emscripten::base<csp::multiplayer::ComponentBase>>("VideoPlayerSpaceComponent")
        .property("videoAssetId", &csp::multiplayer::VideoPlayerSpaceComponent::GetVideoAssetId, &csp::multiplayer::VideoPlayerSpaceComponent::SetVideoAssetId)
        .property("videoAssetURL", &csp::multiplayer::VideoPlayerSpaceComponent::GetVideoAssetURL, &csp::multiplayer::VideoPlayerSpaceComponent::SetVideoAssetURL)
        .property("assetCollectionId", &csp::multiplayer::VideoPlayerSpaceComponent::GetAssetCollectionId, &csp::multiplayer::VideoPlayerSpaceComponent::SetAssetCollectionId)
        .property("position", &csp::multiplayer::VideoPlayerSpaceComponent::GetPosition, &csp::multiplayer::VideoPlayerSpaceComponent::SetPosition)
        .property("rotation", &csp::multiplayer::VideoPlayerSpaceComponent::GetRotation, &csp::multiplayer::VideoPlayerSpaceComponent::SetRotation)
        .property("scale", &csp::multiplayer::VideoPlayerSpaceComponent::GetScale, &csp::multiplayer::VideoPlayerSpaceComponent::SetScale)
        .property("transform", &csp::multiplayer::VideoPlayerSpaceComponent::GetTransform, &csp::multiplayer::VideoPlayerSpaceComponent::SetTransform)
        .property("isStateShared", &csp::multiplayer::VideoPlayerSpaceComponent::GetIsStateShared, &csp::multiplayer::VideoPlayerSpaceComponent::SetIsStateShared)
        .property("isAutoPlay", &csp::multiplayer::VideoPlayerSpaceComponent::GetIsAutoPlay, &csp::multiplayer::VideoPlayerSpaceComponent::SetIsAutoPlay)
        .property("isLoopPlayback", &csp::multiplayer::VideoPlayerSpaceComponent::GetIsLoopPlayback, &csp::multiplayer::VideoPlayerSpaceComponent::SetIsLoopPlayback)
        .property("isAutoResize", &csp::multiplayer::VideoPlayerSpaceComponent::GetIsAutoResize, &csp::multiplayer::VideoPlayerSpaceComponent::SetIsAutoResize)
        .property("playbackState", &csp::multiplayer::VideoPlayerSpaceComponent::GetPlaybackState, &csp::multiplayer::VideoPlayerSpaceComponent::SetPlaybackState)
        .property("currentPlayheadPosition", &csp::multiplayer::VideoPlayerSpaceComponent::GetCurrentPlayheadPosition,
            &csp::multiplayer::VideoPlayerSpaceComponent::SetCurrentPlayheadPosition)
        .property("timeSincePlay", &csp::multiplayer::VideoPlayerSpaceComponent::GetTimeSincePlay, &csp::multiplayer::VideoPlayerSpaceComponent::SetTimeSincePlay)
        .property(
            "videoPlayerSourceType", &csp::multiplayer::VideoPlayerSpaceComponent::GetVideoPlayerSourceType, &csp::multiplayer::VideoPlayerSpaceComponent::SetVideoPlayerSourceType)
        .property("stereoVideoType", &csp::multiplayer::VideoPlayerSpaceComponent::GetStereoVideoType, &csp::multiplayer::VideoPlayerSpaceComponent::SetStereoVideoType)
        .property("isStereoFlipped", &csp::multiplayer::VideoPlayerSpaceComponent::GetIsStereoFlipped, &csp::multiplayer::VideoPlayerSpaceComponent::SetIsStereoFlipped)
        .property("audioType", &csp::multiplayer::VideoPlayerSpaceComponent::GetAudioType, &csp::multiplayer::VideoPlayerSpaceComponent::SetAudioType)
        .property("attenuationRadius", &csp::multiplayer::VideoPlayerSpaceComponent::GetAttenuationRadius, &csp::multiplayer::VideoPlayerSpaceComponent::SetAttenuationRadius)
        .property("volume", &csp::multiplayer::VideoPlayerSpaceComponent::GetVolume, &csp::multiplayer::VideoPlayerSpaceComponent::SetVolume)
        .property("isVisible", &csp::multiplayer::VideoPlayerSpaceComponent::GetIsVisible, &csp::multiplayer::VideoPlayerSpaceComponent::SetIsVisible)
        .property("isARVisible", &csp::multiplayer::VideoPlayerSpaceComponent::GetIsARVisible, &csp::multiplayer::VideoPlayerSpaceComponent::SetIsARVisible)
        .property("isVirtualVisible", &csp::multiplayer::VideoPlayerSpaceComponent::GetIsVirtualVisible, &csp::multiplayer::VideoPlayerSpaceComponent::SetIsVirtualVisible)
        .property("isEnabled", &csp::multiplayer::VideoPlayerSpaceComponent::GetIsEnabled, &csp::multiplayer::VideoPlayerSpaceComponent::SetIsEnabled);
}
