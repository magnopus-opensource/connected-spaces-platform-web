#include "SharedTypeBindings.h"

#include "emscripten/bind.h"
#include "emscripten/val.h"

#include "CSP/Common/Map.h"
#include "CSP/Common/String.h"

/*
 * Definitions for shared CSP type bindings.
 *
 * Bindings for generic types without a single natural home in the CSP source tree can be defined
 * here to locate them easily and avoid potential multiple definition errors.
 */

EMSCRIPTEN_BINDINGS(CSPTypes)
{
    // Container types

    emscripten::register_type<csp::common::Map<csp::common::String, csp::common::String>>("Map<string, string>");

    emscripten::register_optional<uint64_t>();

    // Promise types

    emscripten::register_type<PromiseOfBoolean>("Promise<boolean>");
    emscripten::register_type<PromiseOfSpaceEntityPointer>("Promise<SpaceEntity | null>");
}
