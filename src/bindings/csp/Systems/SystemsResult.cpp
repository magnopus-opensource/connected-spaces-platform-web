// Stub binding.

#include "../../containers/String.h"

#include "CSP/Common/String.h"
#include "CSP/Systems/SystemsResult.h"
#include "CSP/Systems/WebService.h"

#include "emscripten/bind.h"

EMSCRIPTEN_BINDINGS(CSPSystemsResult)
{
    emscripten::class_<csp::systems::NullResult, emscripten::base<csp::systems::ResultBase>>("NullResult");

    emscripten::class_<csp::systems::StringResult, emscripten::base<csp::systems::ResultBase>>("StringResult").property("value", &csp::systems::StringResult::GetValue);
}
