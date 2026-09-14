#include "../../../async/Promises.h"
#include "../../../containers/String.h"
#include "../../CallbackDeclarations.h"
#include "../../PromiseDeclarations.h"

#include "CSP/Common/String.h"
#include "CSP/Systems/GraphQL/GraphQL.h"
#include "CSP/Systems/GraphQL/GraphQLSystem.h"
#include "CSP/Systems/SystemBase.h"

#include "emscripten/bind.h"
#include "emscripten/val.h"

namespace emscripten::internal {
template <> void raw_destructor<csp::systems::GraphQLSystem>(csp::systems::GraphQLSystem*) { }
}

EMSCRIPTEN_BINDINGS(CSPGraphQLSystem)
{
    emscripten::class_<csp::systems::GraphQLSystem, emscripten::base<csp::systems::SystemBase>>("GraphQLSystem")
        .function(
            "runRequest(requestBody)",
            +[](csp::systems::GraphQLSystem& self, const csp::common::String& requestBody) {
                return Promisify<PromiseOfGraphQLResult>([&](emscripten::val cb) { self.RunRequest(requestBody, ToNativeCallback(cb.as<GraphQLReceivedCallback>())); });
            })
        .function(
            "runQuery(queryText)", +[](csp::systems::GraphQLSystem& self, const csp::common::String& queryText) {
                return Promisify<PromiseOfGraphQLResult>([&](emscripten::val cb) { self.RunQuery(queryText, ToNativeCallback(cb.as<GraphQLReceivedCallback>())); });
            });
}
