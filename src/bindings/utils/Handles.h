#pragma once

#include "emscripten/val.h"
#include <cassert>
#include <string>
#include <utility>

namespace bindings::utils {

/*
 * Check to see if we're a C++ bound embind object or not.
 * `isDeleted` is technically not a contract check, embind doesn't
 * provide something altogether straightforward for this, but this
 * is as good as standard.
 */
inline bool IsBoundHandle(emscripten::val v)
{
    if (v.typeOf().as<std::string>() != "object")
        return false;
    if (v.isNull())
        return false;
    return !v["isDeleted"].isUndefined();
}

/*
 * Since emscripten doesn't really approve of non-owning handles, but our API has them nonetheless
 * we inject some safety by undefining the owning methods (delete, deleteLater)
 * are called. This overrides the actual deletion behaviour, making it formally impossible
 * to delete non-owning pointers if enriched in this manner. You'll get a typeerror if you try.
 *
 * Notable, `clone` remains allowed, even on pointer types where it's sort of redundant.
 * This decision is to create a uniform API surface, where cloning out of a callback is always the right thing to do.
*/
inline void ForbidOwningMemoryBehaviours(emscripten::val handle)
{
    assert(bindings::utils::IsBoundHandle(handle) && "ForbidOwningMemoryBehaviours was passed a value that is not a bound handle");

    handle.set("delete", emscripten::val::undefined());
    handle.set("deleteLater", emscripten::val::undefined());
    handle.set(emscripten::val::global("Symbol")["dispose"], emscripten::val::undefined());
}

/*
 * Return a `val` with non owning behaviours removed
 * HandleT sometimes need to be provided explicitly to a type registered manually with emscripten via EMSCRIPTEN_DECLARE_VAL_TYPE.
 * For example in the motivating example when we directly return non-owning pointers, we are returning a raw `val`, as we need
 * to do that in order to interact with the JS side functions, but a raw `val` does not carry a TS signature unless we explicitly
 * use the DECLARE_VAL_TYPE path to enrich it.
 * You need to do this less than you might think, as many constructs (such as callbacks), directly encode their typescript types,
 * so there is no need to ensure the val remains enriched. At time of writing, this is only necessary for direct returns.
 *
 * Remember that all EMSCRIPTEN_DECLARE_VAL_TYPE does is extend emscripten::val but give it a name, literally that's it.
 */
template <typename HandleT = emscripten::val, typename T> inline HandleT NonOwningVal(T&& val)
{
    emscripten::val handle(std::forward<T>(val));
    if (IsBoundHandle(handle)) {
        ForbidOwningMemoryBehaviours(handle);
    }
    return HandleT { std::move(handle) };
}

/* Same as above, but with a reference return value policy. For pointer objects */
template <typename HandleT = emscripten::val, typename T> inline HandleT NonOwningValRef(T&& val)
{
    emscripten::val handle(std::forward<T>(val), emscripten::return_value_policy::reference());
    if (IsBoundHandle(handle)) {
        ForbidOwningMemoryBehaviours(handle);
    }
    return HandleT { std::move(handle) };
}
}
