#include "../../utils/Handles.h"
#include "../PointerDeclarations.h"

#include "CSP/Common/CancellationToken.h"

#include "emscripten/bind.h"
#include "emscripten/val.h"

EMSCRIPTEN_BINDINGS(CSPCancellationToken)
{
    emscripten::class_<csp::common::CancellationToken>("CancellationToken")
        .class_function(
            "create", +[]() { return new csp::common::CancellationToken(); }, emscripten::allow_raw_pointers())
        .class_function(
            "dummy", +[]() { return bindings::utils::NonOwningVal<CancellationTokenPointer>(&csp::common::CancellationToken::Dummy()); })
        .property("cancelled", &csp::common::CancellationToken::Cancelled)
        .function("cancel", &csp::common::CancellationToken::Cancel);
}
