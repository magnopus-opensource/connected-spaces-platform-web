#include "../../../containers/String.h"

#include "CSP/Common/String.h"
#include "CSP/Systems/GraphQL/GraphQL.h"
#include "CSP/Systems/WebService.h"

#include "emscripten/bind.h"

EMSCRIPTEN_BINDINGS(CSPGraphQL)
{
    emscripten::class_<csp::systems::GraphQLResult, emscripten::base<csp::systems::ResultBase>>("GraphQLResult").property("response", &csp::systems::GraphQLResult::GetResponse);
}
