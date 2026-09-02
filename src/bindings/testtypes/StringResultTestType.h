/*
 * Fake bindings just so we can build out a test suite.
 * These may get deleted at the end of the project. It's difficult to test
 * as you go without doing something like this, because much of CSP is
 * interdependent, so you can't really use many real types early on in the
 * bindings migration.
 */
#pragma once

#include "CSP/Common/Optional.h"
#include "CSP/Common/SharedEnums.h"
#include "CSP/Common/String.h"
#include "CSP/Systems/WebService.h"

#include <type_traits>

class StringResultTestType : public csp::systems::ResultBase {
public:
    StringResultTestType(csp::systems::EResultCode resultCode, csp::web::EResponseCodes httpResultCode,
        csp::systems::ERequestFailureReason failureReason = csp::systems::ERequestFailureReason::None)
        : csp::systems::ResultBase(resultCode, static_cast<std::underlying_type<csp::web::EResponseCodes>::type>(httpResultCode), failureReason)
    {
    }

    const csp::common::String& GetValue() const { return m_value; }
    void SetValue(csp::common::String value) { m_value = std::move(value); }

    void SetProgress(float requestProgress, float responseProgress)
    {
        RequestProgress = requestProgress;
        ResponseProgress = responseProgress;
    }

private:
    csp::common::String m_value;
};
