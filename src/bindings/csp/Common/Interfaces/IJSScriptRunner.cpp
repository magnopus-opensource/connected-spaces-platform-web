#include "../../../containers/String.h"

#include "CSP/Common/Interfaces/IJSScriptRunner.h"
#include "CSP/Common/String.h"

#include "emscripten/bind.h"

EMSCRIPTEN_BINDINGS(CSPJSScriptRunner)
{
    emscripten::class_<csp::common::IJSScriptRunner>("IJSScriptRunner").function("runScript(contextId, scriptText)", &csp::common::IJSScriptRunner::RunScript);
}
