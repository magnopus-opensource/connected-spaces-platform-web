#pragma once

#include "../containers/Disposal.h"
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
    enum class DisposePolicy { NoDisposal, Array, Map, Optional, SingleElement };

    explicit RAIIVal(emscripten::val val, DisposePolicy policy) : Val(std::move(val)), m_disposePolicy(policy) { }
    ~RAIIVal()
    {
        switch (m_disposePolicy) {
        case DisposePolicy::Array:
            bindings::utils::DisposeArrayNoThrow(Val);
            break;
        case DisposePolicy::Map:
            bindings::utils::DisposeMapNoThrow(Val);
            break;
        case DisposePolicy::Optional:
            bindings::utils::DisposeOptionalNoThrow(Val);
            break;
        case DisposePolicy::SingleElement:
            bindings::utils::DisposeElementNoThrow(Val);
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
