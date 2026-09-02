/*
 * Bindings for CSP/Systems/WebService.h
 *
 * Contains only the bindings for ResultBase, the base class of all {*}Result types.
 * Every concrete result type must register itself with ResultBase as follows:
 *
 *     emscripten::class_<csp::systems::NullResult, emscripten::base<csp::systems::ResultBase>>("NullResult")
 */

#include "../../containers/String.h"

#include "CSP/Systems/WebService.h"

#include "emscripten/bind.h"

EMSCRIPTEN_BINDINGS(CSPWebService)
{
    emscripten::class_<csp::systems::ResultBase>("ResultBase")
        .property("resultCode", &csp::systems::ResultBase::GetResultCode)
        .property("httpResultCode", &csp::systems::ResultBase::GetHttpResultCode)
        .property("responseBody", &csp::systems::ResultBase::GetResponseBody)
        .property("requestProgress", &csp::systems::ResultBase::GetRequestProgress)
        .property("responseProgress", &csp::systems::ResultBase::GetResponseProgress)
        .property("failureReason", &csp::systems::ResultBase::GetFailureReason)
        .function("equals(other)", &csp::systems::ResultBase::operator==);
}
