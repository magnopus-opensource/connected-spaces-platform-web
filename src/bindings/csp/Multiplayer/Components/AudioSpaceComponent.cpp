#include "../../../containers/String.h"

#include "CSP/Common/String.h"
#include "CSP/Common/Vector.h"
#include "CSP/Multiplayer/Components/AudioSpaceComponent.h"

#include "emscripten/bind.h"

EMSCRIPTEN_BINDINGS(CSPAudioSpaceComponent)
{
    emscripten::enum_<csp::multiplayer::AudioPlaybackState>("AudioPlaybackState", emscripten::enum_value_type::number)
        .value("Reset", csp::multiplayer::AudioPlaybackState::Reset)
        .value("Pause", csp::multiplayer::AudioPlaybackState::Pause)
        .value("Play", csp::multiplayer::AudioPlaybackState::Play)
        .value("Num", csp::multiplayer::AudioPlaybackState::Num);

    emscripten::enum_<csp::multiplayer::AudioPropertyKeys>("AudioPropertyKeys", emscripten::enum_value_type::number)
        .value("Position", csp::multiplayer::AudioPropertyKeys::Position)
        .value("PlaybackState", csp::multiplayer::AudioPropertyKeys::PlaybackState)
        .value("AudioType", csp::multiplayer::AudioPropertyKeys::AudioType)
        .value("AudioAssetId", csp::multiplayer::AudioPropertyKeys::AudioAssetId)
        .value("AssetCollectionId", csp::multiplayer::AudioPropertyKeys::AssetCollectionId)
        .value("AttenuationRadius", csp::multiplayer::AudioPropertyKeys::AttenuationRadius)
        .value("IsLoopPlayback", csp::multiplayer::AudioPropertyKeys::IsLoopPlayback)
        .value("TimeSincePlay", csp::multiplayer::AudioPropertyKeys::TimeSincePlay)
        .value("Volume", csp::multiplayer::AudioPropertyKeys::Volume)
        .value("IsEnabled", csp::multiplayer::AudioPropertyKeys::IsEnabled)
        .value("ThirdPartyComponentRef", csp::multiplayer::AudioPropertyKeys::ThirdPartyComponentRef)
        .value("Num", csp::multiplayer::AudioPropertyKeys::Num);

    emscripten::class_<csp::multiplayer::AudioSpaceComponent, emscripten::base<csp::multiplayer::ComponentBase>>("AudioSpaceComponent")
        .property("position", &csp::multiplayer::AudioSpaceComponent::GetPosition, &csp::multiplayer::AudioSpaceComponent::SetPosition)
        .property("playbackState", &csp::multiplayer::AudioSpaceComponent::GetPlaybackState, &csp::multiplayer::AudioSpaceComponent::SetPlaybackState)
        .property("audioAssetId", &csp::multiplayer::AudioSpaceComponent::GetAudioAssetId, &csp::multiplayer::AudioSpaceComponent::SetAudioAssetId)
        .property("assetCollectionId", &csp::multiplayer::AudioSpaceComponent::GetAssetCollectionId, &csp::multiplayer::AudioSpaceComponent::SetAssetCollectionId)
        .property("isLoopPlayback", &csp::multiplayer::AudioSpaceComponent::GetIsLoopPlayback, &csp::multiplayer::AudioSpaceComponent::SetIsLoopPlayback)
        .property("timeSincePlay", &csp::multiplayer::AudioSpaceComponent::GetTimeSincePlay, &csp::multiplayer::AudioSpaceComponent::SetTimeSincePlay)
        .property("audioType", &csp::multiplayer::AudioSpaceComponent::GetAudioType, &csp::multiplayer::AudioSpaceComponent::SetAudioType)
        .property("attenuationRadius", &csp::multiplayer::AudioSpaceComponent::GetAttenuationRadius, &csp::multiplayer::AudioSpaceComponent::SetAttenuationRadius)
        .property("volume", &csp::multiplayer::AudioSpaceComponent::GetVolume, &csp::multiplayer::AudioSpaceComponent::SetVolume)
        .property("isEnabled", &csp::multiplayer::AudioSpaceComponent::GetIsEnabled, &csp::multiplayer::AudioSpaceComponent::SetIsEnabled)
        .property("thirdPartyComponentRef", &csp::multiplayer::AudioSpaceComponent::GetThirdPartyComponentRef, &csp::multiplayer::AudioSpaceComponent::SetThirdPartyComponentRef);
}
