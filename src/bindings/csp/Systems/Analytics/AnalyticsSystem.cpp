#include "../../../async/Promises.h"
#include "../../../containers/Map.h"
#include "../../../containers/Optional.h"
#include "../../../containers/String.h"
#include "../../CallbackDeclarations.h"
#include "../../PromiseDeclarations.h"

#include "CSP/Common/Map.h"
#include "CSP/Common/Optional.h"
#include "CSP/Common/String.h"
#include "CSP/Systems/Analytics/AnalyticsSystem.h"
#include "CSP/Systems/SystemBase.h"

#include "emscripten/bind.h"
#include "emscripten/val.h"

namespace emscripten::internal {
template <> void raw_destructor<csp::systems::AnalyticsSystem>(csp::systems::AnalyticsSystem*) { }
}

EMSCRIPTEN_BINDINGS(CSPAnalyticsSystem)
{
    emscripten::class_<csp::systems::AnalyticsSystem, emscripten::base<csp::systems::SystemBase>>("AnalyticsSystem")
        .function("queueAnalyticsEvent(productContextSection, category, interactionType, subCategory, metadata)", &csp::systems::AnalyticsSystem::QueueAnalyticsEvent)
        .function(
            "sendAnalyticsEvent(productContextSection, category, interactionType, subCategory, metadata)",
            +[](csp::systems::AnalyticsSystem& self, const csp::common::String& productContextSection, const csp::common::String& category,
                 const csp::common::String& interactionType, const csp::common::Optional<csp::common::String>& subCategory,
                 const csp::common::Optional<csp::common::Map<csp::common::String, csp::common::String>>& metadata) {
                return Promisify<PromiseOfNullResult>([&](emscripten::val cb) {
                    self.SendAnalyticsEvent(productContextSection, category, interactionType, subCategory, metadata, ToNativeCallback(cb.as<NullResultCallback>()));
                });
            })
        .function(
            "flushAnalyticsEventsQueue", +[](csp::systems::AnalyticsSystem& self) {
                return Promisify<PromiseOfNullResult>([&](emscripten::val cb) { self.FlushAnalyticsEventsQueue(ToNativeCallback(cb.as<NullResultCallback>())); });
            });
}
