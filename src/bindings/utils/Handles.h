#pragma once

#include "emscripten/val.h"
#include <cassert>
#include <string>

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
 * Since emscripten dosen't really approve of non-owning handles, but our API has them nonetheless
 * we inject some safety by throwing if any of the owning methods (delete, deleteLater, clone)
 * are called. This throw overrides the actual deletion behaviour, making it formally impossible
 * to delete non-owning pointers if enriched in this manner.
 *
 * Note: I'm not sure if this prevents `using` disposal from occuring, but so long as non owning
 * pointers are never marked `Disposable`, that should not be a safety concern.
*/
inline void ForbidOwningMemoryBehaviours(emscripten::val handle)
{
    assert(bindings::utils::IsBoundHandle(handle) && "ForbidOwningMemoryBehaviours was passed a value that is not a bound handle");

    static const emscripten::val deleteThrower = emscripten::val::global("Function").new_(std::string("throw new Error('delete() should not be called on non-owning handle.');"));
    static const emscripten::val deleteLaterThrower
        = emscripten::val::global("Function").new_(std::string("throw new Error('deleteLater() should not be called on non-owning handle.');"));
    static const emscripten::val cloneThrower = emscripten::val::global("Function").new_(std::string("throw new Error('clone() should not be called on non-owning handle.');"));
    static const emscripten::val disposeThrower = emscripten::val::global("Function").new_(std::string("throw new Error('dispose() should not be called on non-owning handle.');"));

    handle.set("delete", deleteThrower);
    handle.set("deleteLater", deleteLaterThrower);
    handle.set("clone", cloneThrower);
    handle.set(emscripten::val::global("Symbol")["dispose"], disposeThrower);
}

template <typename T> inline emscripten::val NonOwningVal(T&& val)
{
    emscripten::val handle(std::forward<T>(val), emscripten::return_value_policy::reference());
    if (IsBoundHandle(handle)) {
        ForbidOwningMemoryBehaviours(handle);
    }
    return handle;
}

}
