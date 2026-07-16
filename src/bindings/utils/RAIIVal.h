#pragma once

#include "Memory.h"
#include <emscripten/val.h>

namespace bindings::utils {
/*
 * Used for callback arguments. Host an emscripten val with a disposal policy, that
 * disposes itself when it falls out of scope according to that policy.
 * Ie, if you're hosting an array, pass an array disposal policy, and we'll do
 * array disposal via disposeArray()!
 */
class RAIIVal {
public:
    enum class DisposePolicy {
        NoDisposal, //Does no disposal
        Array,
        Map,
        Optional,
        SingleElement
    };

    explicit RAIIVal(emscripten::val val, DisposePolicy policy) : Val(std::move(val)), m_disposePolicy(policy) { }
    ~RAIIVal()
    {
        switch (m_disposePolicy) {
        case DisposePolicy::Array:
            bindings::utils::DisposeArray(Val);
            break;
        case DisposePolicy::Map:
            bindings::utils::DisposeMap(Val);
            break;
        case DisposePolicy::Optional:
            bindings::utils::DisposeOptional(Val);
            break;
        case DisposePolicy::SingleElement:
            bindings::utils::DisposeElement(Val);
            break;
        case DisposePolicy::NoDisposal:
            break;
        }
    }
    RAIIVal(RAIIVal&&) = delete;
    RAIIVal(const RAIIVal&) = delete;
    RAIIVal& operator=(RAIIVal&&) = delete;
    RAIIVal& operator=(const RAIIVal&) = delete;

    emscripten::val Val;

private:
    DisposePolicy m_disposePolicy;
};
} // namespace bindings::utils
