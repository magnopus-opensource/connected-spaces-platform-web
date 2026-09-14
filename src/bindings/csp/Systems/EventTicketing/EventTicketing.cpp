#include "../../../containers/Array.h"
#include "../../../containers/String.h"
#include "../../../utils/JSDisposable.h"

#include "CSP/Common/Array.h"
#include "CSP/Common/String.h"
#include "CSP/Systems/EventTicketing/EventTicketing.h"
#include "CSP/Systems/WebService.h"

#include "emscripten/bind.h"

EMSCRIPTEN_BINDINGS(CSPEventTicketing)
{
    emscripten::enum_<csp::systems::EventTicketingVendor>("EventTicketingVendor", emscripten::enum_value_type::number)
        .value("Eventbrite", csp::systems::EventTicketingVendor::Eventbrite)
        .value("Unknown", csp::systems::EventTicketingVendor::Unknown);

    emscripten::enum_<csp::systems::TicketStatus>("TicketStatus", emscripten::enum_value_type::number)
        .value("Purchased", csp::systems::TicketStatus::Purchased)
        .value("Redeemed", csp::systems::TicketStatus::Redeemed)
        .value("Unknown", csp::systems::TicketStatus::Unknown);

    emscripten::class_<csp::systems::TicketedEvent>("TicketedEvent")
        .class_function(
            "create", +[]() { return csp::systems::TicketedEvent(); })
        .property("id", &csp::systems::TicketedEvent::Id)
        .property("spaceId", &csp::systems::TicketedEvent::SpaceId)
        .property("vendor", &csp::systems::TicketedEvent::Vendor)
        .property("vendorEventId", &csp::systems::TicketedEvent::VendorEventId)
        .property("vendorEventUri", &csp::systems::TicketedEvent::VendorEventUri)
        .property("isTicketingActive", &csp::systems::TicketedEvent::IsTicketingActive)
        .function("equals(other)", &csp::systems::TicketedEvent::operator==);

    emscripten::class_<csp::systems::EventTicket>("EventTicket")
        .class_function(
            "create", +[]() { return csp::systems::EventTicket(); })
        .property("id", &csp::systems::EventTicket::Id)
        .property("spaceId", &csp::systems::EventTicket::SpaceId)
        .property("vendor", &csp::systems::EventTicket::Vendor)
        .property("vendorEventId", &csp::systems::EventTicket::VendorEventId)
        .property("vendorTicketId", &csp::systems::EventTicket::VendorTicketId)
        .property("status", &csp::systems::EventTicket::Status)
        .property("userId", &csp::systems::EventTicket::UserId)
        .property("email", &csp::systems::EventTicket::Email)
        .function("equals(other)", &csp::systems::EventTicket::operator==);

    emscripten::class_<csp::systems::TicketedEventVendorAuthInfo>("TicketedEventVendorAuthInfo")
        .class_function(
            "create", +[]() { return csp::systems::TicketedEventVendorAuthInfo(); })
        .property("vendor", &csp::systems::TicketedEventVendorAuthInfo::Vendor)
        .property("clientId", &csp::systems::TicketedEventVendorAuthInfo::ClientId)
        .property("authorizeEndpoint", &csp::systems::TicketedEventVendorAuthInfo::AuthorizeEndpoint)
        .property("oAuthRedirectUrl", &csp::systems::TicketedEventVendorAuthInfo::OAuthRedirectUrl)
        .function("equals(other)", &csp::systems::TicketedEventVendorAuthInfo::operator==);

    emscripten::class_<csp::systems::TicketedEventResult, emscripten::base<csp::systems::ResultBase>>("TicketedEventResult")
        .function(
            "getTicketedEvent", +[](const csp::systems::TicketedEventResult& self) { return self.GetTicketedEvent(); });

    emscripten::class_<csp::systems::TicketedEventCollectionResult, emscripten::base<csp::systems::ResultBase>>("TicketedEventCollectionResult")
        .function(
            "getTicketedEvents", +[](const csp::systems::TicketedEventCollectionResult& self) {
                return bindings::utils::JSDisposable<csp::common::Array<csp::systems::TicketedEvent>> { self.GetTicketedEvents() };
            });

    emscripten::class_<csp::systems::EventTicketResult, emscripten::base<csp::systems::ResultBase>>("EventTicketResult")
        .function(
            "getEventTicket", +[](const csp::systems::EventTicketResult& self) { return self.GetEventTicket(); });

    emscripten::class_<csp::systems::SpaceIsTicketedResult, emscripten::base<csp::systems::ResultBase>>("SpaceIsTicketedResult")
        .property(
            "isTicketedEvent", +[](const csp::systems::SpaceIsTicketedResult& self) { return self.GetIsTicketedEvent(); });

    emscripten::class_<csp::systems::TicketedEventVendorAuthInfoResult, emscripten::base<csp::systems::ResultBase>>("TicketedEventVendorAuthInfoResult")
        .function(
            "getVendorAuthInfo", +[](const csp::systems::TicketedEventVendorAuthInfoResult& self) { return self.GetVendorAuthInfo(); });
}
