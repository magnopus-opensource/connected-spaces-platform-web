#include "../../../containers/Array.h"
#include "../../../containers/String.h"
#include "../../../utils/JSDisposable.h"

#include "CSP/Common/Array.h"
#include "CSP/Common/String.h"
#include "CSP/Systems/Multiplayer/Scope.h"
#include "CSP/Systems/WebService.h"

#include "emscripten/bind.h"

EMSCRIPTEN_BINDINGS(CSPScope)
{
    emscripten::enum_<csp::systems::PubSubModelType>("PubSubModelType", emscripten::enum_value_type::number)
        .value("Object", csp::systems::PubSubModelType::Object)
        .value("Global", csp::systems::PubSubModelType::Global);

    emscripten::class_<csp::systems::Scope>("Scope")
        .class_function(
            "create", +[]() { return csp::systems::Scope(); })
        .property("id", &csp::systems::Scope::Id)
        .property("referenceId", &csp::systems::Scope::ReferenceId)
        .property("referenceType", &csp::systems::Scope::ReferenceType)
        .property("name", &csp::systems::Scope::Name)
        .property("pubSubType", &csp::systems::Scope::PubSubType)
        .property("solveRadius", &csp::systems::Scope::SolveRadius)
        .property("managedLeaderElection", &csp::systems::Scope::ManagedLeaderElection)
        .function("equals(other)", &csp::systems::Scope::operator==);
}
