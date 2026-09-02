/*
 * Fake bindings just so we can build out a test suite.
 * These may get deleted at the end of the project. It's difficult to test
 * as you go without doing something like this, because much of CSP is
 * interdependent, so you can't really use many real types early on in the
 * bindings migration.
 *
 * This one is for a test result type inheriting from ResultBase, containing a string value.
 * The test type has public setters and constructor unlike the concrete result types in CSP which
 * rely on friend class relationships.
 */

#include "../containers/Optional.h"
#include "../containers/String.h"
#include "StringResultTestType.h"

#include "CSP/Common/Optional.h"
#include "CSP/Common/SharedEnums.h"
#include "CSP/Common/String.h"

#include "emscripten/bind.h"

EMSCRIPTEN_BINDINGS(CSPResultTestTypeBindings)
{
    // Register the optional type for ERequestFailureReason, used by the constructor
    emscripten::register_optional<csp::systems::ERequestFailureReason>();

    emscripten::class_<StringResultTestType, emscripten::base<csp::systems::ResultBase>>("StringResultTestType")
        .class_function(
            "create(resultCode, httpResultCode, failureReason)",
            +[](csp::systems::EResultCode resultCode, csp::web::EResponseCodes httpResultCode, csp::common::Optional<csp::systems::ERequestFailureReason> failureReason) {
                return StringResultTestType(resultCode, httpResultCode, failureReason.HasValue() ? *failureReason : csp::systems::ERequestFailureReason::None);
            })
        .property("value", &StringResultTestType::GetValue);
}
