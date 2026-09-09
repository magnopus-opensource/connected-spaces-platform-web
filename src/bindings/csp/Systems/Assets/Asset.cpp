// Stub binding.

#include "CSP/Systems/Assets/Asset.h"

#include "emscripten/bind.h"

EMSCRIPTEN_BINDINGS(CSPAsset)
{
    emscripten::enum_<csp::systems::EAssetType>("EAssetType", emscripten::enum_value_type::number)
        .value("IMAGE", csp::systems::EAssetType::IMAGE)
        .value("THUMBNAIL", csp::systems::EAssetType::THUMBNAIL)
        .value("SIMULATION", csp::systems::EAssetType::SIMULATION)
        .value("MODEL", csp::systems::EAssetType::MODEL)
        .value("VIDEO", csp::systems::EAssetType::VIDEO)
        .value("SCRIPT_LIBRARY", csp::systems::EAssetType::SCRIPT_LIBRARY)
        .value("HOLOCAP_VIDEO", csp::systems::EAssetType::HOLOCAP_VIDEO)
        .value("HOLOCAP_AUDIO", csp::systems::EAssetType::HOLOCAP_AUDIO)
        .value("AUDIO", csp::systems::EAssetType::AUDIO)
        .value("GAUSSIAN_SPLAT", csp::systems::EAssetType::GAUSSIAN_SPLAT)
        .value("MATERIAL", csp::systems::EAssetType::MATERIAL)
        .value("ANNOTATION", csp::systems::EAssetType::ANNOTATION)
        .value("ANNOTATION_THUMBNAIL", csp::systems::EAssetType::ANNOTATION_THUMBNAIL)
        .value("TEXT", csp::systems::EAssetType::TEXT);
}
