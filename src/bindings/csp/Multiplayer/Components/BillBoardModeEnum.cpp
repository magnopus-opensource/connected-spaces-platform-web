#include "CSP/Multiplayer/Components/BillBoardModeEnum.h"

#include "emscripten/bind.h"

EMSCRIPTEN_BINDINGS(CSPBillBoardModeEnum)
{
    emscripten::enum_<csp::multiplayer::BillboardMode>("BillboardMode", emscripten::enum_value_type::number)
        .value("Off", csp::multiplayer::BillboardMode::Off)
        .value("Billboard", csp::multiplayer::BillboardMode::Billboard)
        .value("YawLockedBillboard", csp::multiplayer::BillboardMode::YawLockedBillboard);
}
