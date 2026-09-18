#include "../../../async/Promises.h"
#include "../../../containers/Array.h"
#include "../../../containers/Optional.h"
#include "../../../containers/String.h"
#include "../../CallbackDeclarations.h"
#include "../../PromiseDeclarations.h"

#include "CSP/Common/Array.h"
#include "CSP/Common/Optional.h"
#include "CSP/Common/String.h"
#include "CSP/Systems/EventTicketing/EventTicketing.h"
#include "CSP/Systems/EventTicketing/EventTicketingSystem.h"
#include "CSP/Systems/SystemBase.h"

#include "emscripten/bind.h"
#include "emscripten/val.h"

namespace emscripten::internal {
template <> void raw_destructor<csp::systems::EventTicketingSystem>(csp::systems::EventTicketingSystem*) { }
}

EMSCRIPTEN_BINDINGS(CSPEventTicketingSystem)
{
    emscripten::class_<csp::systems::EventTicketingSystem, emscripten::base<csp::systems::SystemBase>>("EventTicketingSystem")
        .function(
            "createTicketedEvent(spaceId, vendor, vendorEventId, vendorEventUri, isTicketingActive)",
            +[](csp::systems::EventTicketingSystem& self, const csp::common::String& spaceId, csp::systems::EventTicketingVendor vendor, const csp::common::String& vendorEventId,
                 const csp::common::String& vendorEventUri, bool isTicketingActive) {
                return Promisify<PromiseOfTicketedEventResult>([&](emscripten::val cb) {
                    self.CreateTicketedEvent(spaceId, vendor, vendorEventId, vendorEventUri, isTicketingActive, ToNativeCallback(cb.as<TicketedEventResultCallback>()));
                });
            })
        .function(
            "updateTicketedEvent(spaceId, eventId, vendor, vendorEventId, vendorEventUri, isTicketingActive)",
            +[](csp::systems::EventTicketingSystem& self, const csp::common::String& spaceId, const csp::common::String& eventId,
                 const csp::common::Optional<csp::systems::EventTicketingVendor>& vendor, const csp::common::Optional<csp::common::String>& vendorEventId,
                 const csp::common::Optional<csp::common::String>& vendorEventUri, const csp::common::Optional<bool>& isTicketingActive) {
                return Promisify<PromiseOfTicketedEventResult>([&](emscripten::val cb) {
                    self.UpdateTicketedEvent(spaceId, eventId, vendor, vendorEventId, vendorEventUri, isTicketingActive, ToNativeCallback(cb.as<TicketedEventResultCallback>()));
                });
            })
        .function(
            "getTicketedEvents(spaceIds, skip, limit)",
            +[](csp::systems::EventTicketingSystem& self, const csp::common::Array<csp::common::String>& spaceIds, const csp::common::Optional<int>& skip,
                 const csp::common::Optional<int>& limit) {
                return Promisify<PromiseOfTicketedEventCollectionResult>(
                    [&](emscripten::val cb) { self.GetTicketedEvents(spaceIds, skip, limit, ToNativeCallback(cb.as<TicketedEventCollectionResultCallback>())); });
            })
        .function(
            "submitEventTicket(spaceId, vendor, vendorEventId, vendorTicketId, onBehalfOfUserId)",
            +[](csp::systems::EventTicketingSystem& self, const csp::common::String& spaceId, csp::systems::EventTicketingVendor vendor, const csp::common::String& vendorEventId,
                 const csp::common::String& vendorTicketId, const csp::common::Optional<csp::common::String>& onBehalfOfUserId) {
                return Promisify<PromiseOfEventTicketResult>([&](emscripten::val cb) {
                    self.SubmitEventTicket(spaceId, vendor, vendorEventId, vendorTicketId, onBehalfOfUserId, ToNativeCallback(cb.as<EventTicketResultCallback>()));
                });
            })
        .function(
            "getVendorAuthorizeInfo(vendor, userId)",
            +[](csp::systems::EventTicketingSystem& self, csp::systems::EventTicketingVendor vendor, const csp::common::String& userId) {
                return Promisify<PromiseOfTicketedEventVendorAuthInfoResult>(
                    [&](emscripten::val cb) { self.GetVendorAuthorizeInfo(vendor, userId, ToNativeCallback(cb.as<TicketedEventVendorAuthorizeInfoCallback>())); });
            })
        .function(
            "getIsSpaceTicketed(spaceId)", +[](csp::systems::EventTicketingSystem& self, const csp::common::String& spaceId) {
                return Promisify<PromiseOfSpaceIsTicketedResult>(
                    [&](emscripten::val cb) { self.GetIsSpaceTicketed(spaceId, ToNativeCallback(cb.as<SpaceIsTicketedResultCallback>())); });
            });
}
