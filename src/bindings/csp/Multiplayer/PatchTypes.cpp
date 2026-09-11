#include "CSP/CSPCommon.h"

#include "CSP/Multiplayer/PatchTypes.h"

#include "emscripten/bind.h"

EMSCRIPTEN_BINDINGS(CSPPatchTypes)
{
    emscripten::enum_<csp::multiplayer::SpaceEntityUpdateFlags>("SpaceEntityUpdateFlags", emscripten::enum_value_type::number)
        .value("UPDATE_FLAGS_NAME", csp::multiplayer::SpaceEntityUpdateFlags::UPDATE_FLAGS_NAME)
        .value("UPDATE_FLAGS_POSITION", csp::multiplayer::SpaceEntityUpdateFlags::UPDATE_FLAGS_POSITION)
        .value("UPDATE_FLAGS_ROTATION", csp::multiplayer::SpaceEntityUpdateFlags::UPDATE_FLAGS_ROTATION)
        .value("UPDATE_FLAGS_SCALE", csp::multiplayer::SpaceEntityUpdateFlags::UPDATE_FLAGS_SCALE)
        .value("UPDATE_FLAGS_COMPONENTS", csp::multiplayer::SpaceEntityUpdateFlags::UPDATE_FLAGS_COMPONENTS)
        .value("UPDATE_FLAGS_SELECTION_ID", csp::multiplayer::SpaceEntityUpdateFlags::UPDATE_FLAGS_SELECTION_ID)
        .value("UPDATE_FLAGS_THIRD_PARTY_REF", csp::multiplayer::SpaceEntityUpdateFlags::UPDATE_FLAGS_THIRD_PARTY_REF)
        .value("UPDATE_FLAGS_PARENT", csp::multiplayer::SpaceEntityUpdateFlags::UPDATE_FLAGS_PARENT)
        .value("UPDATE_FLAGS_LOCK_TYPE", csp::multiplayer::SpaceEntityUpdateFlags::UPDATE_FLAGS_LOCK_TYPE);

    emscripten::enum_<csp::multiplayer::ComponentUpdateType>("ComponentUpdateType", emscripten::enum_value_type::number)
        .value("Update", csp::multiplayer::ComponentUpdateType::Update)
        .value("Add", csp::multiplayer::ComponentUpdateType::Add)
        .value("Delete", csp::multiplayer::ComponentUpdateType::Delete);

    emscripten::value_object<csp::multiplayer::ComponentUpdateInfo>("ComponentUpdateInfo")
        .field("componentId", &csp::multiplayer::ComponentUpdateInfo::ComponentId)
        .field("updateType", &csp::multiplayer::ComponentUpdateInfo::UpdateType);
}
