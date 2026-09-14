#include "../../containers/Array.h"
#include "../../containers/Map.h"
#include "../../containers/String.h"

#include "CSP/Common/Array.h"
#include "CSP/Common/Map.h"
#include "CSP/Common/String.h"
#include "CSP/Systems/SystemsResult.h"
#include "CSP/Systems/WebService.h"

#include "emscripten/bind.h"

EMSCRIPTEN_BINDINGS(CSPSystemsResult)
{
    emscripten::class_<csp::systems::NullResult, emscripten::base<csp::systems::ResultBase>>("NullResult");

    emscripten::class_<csp::systems::BooleanResult, emscripten::base<csp::systems::ResultBase>>("BooleanResult").property("value", &csp::systems::BooleanResult::GetValue);

    emscripten::class_<csp::systems::StringResult, emscripten::base<csp::systems::ResultBase>>("StringResult").property("value", &csp::systems::StringResult::GetValue);

    emscripten::class_<csp::systems::StringArrayResult, emscripten::base<csp::systems::ResultBase>>("StringArrayResult")
        .property("value", &csp::systems::StringArrayResult::GetValue);

    emscripten::class_<csp::systems::UInt64Result, emscripten::base<csp::systems::ResultBase>>("UInt64Result").property("value", &csp::systems::UInt64Result::GetValue);

    emscripten::class_<csp::systems::HTTPHeadersResult, emscripten::base<csp::systems::ResultBase>>("HTTPHeadersResult")
        .property("value", &csp::systems::HTTPHeadersResult::GetValue);
}
