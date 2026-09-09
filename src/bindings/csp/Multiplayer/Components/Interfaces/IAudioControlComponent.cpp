#include "CSP/Multiplayer/Components/Interfaces/IAudioControlComponent.h"

#include "emscripten/bind.h"

EMSCRIPTEN_BINDINGS(CSPAudioControlComponent)
{
    emscripten::enum_<csp::multiplayer::AudioType>("AudioType", emscripten::enum_value_type::number)
        .value("Global", csp::multiplayer::AudioType::Global)
        .value("Spatial", csp::multiplayer::AudioType::Spatial)
        .value("Num", csp::multiplayer::AudioType::Num);
}
