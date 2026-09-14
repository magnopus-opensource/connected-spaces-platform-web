#include "CSP/Systems/SystemBase.h"

#include "emscripten/bind.h"

EMSCRIPTEN_BINDINGS(CSPSystemBase)
{
    emscripten::class_<csp::systems::SystemBase>("SystemBase").function("registerSystemCallback", &csp::systems::SystemBase::RegisterSystemCallback);
}
