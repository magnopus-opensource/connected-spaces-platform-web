#pragma once

#include "emscripten/val.h"
#include <string>

namespace bindings::utils {

/*
 * Check to see if we're a C++ bound embind object or not.
 * `isDeleted` is technically a contract check, embind doesn't
 * provide something altogether straightforward for this, but this 
 * is as good as standard.
 */
inline bool IsBoundHandle(emscripten::val v) {
    if (v.typeOf().as<std::string>() != "object") return false;
    if (v.isNull()) return false;
    return !v["isDeleted"].isUndefined();
}

}
