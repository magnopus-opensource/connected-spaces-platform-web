#include "../utils/Handles.h"
#include "emscripten/bind.h"
#include "emscripten/val.h"

namespace {

// Forward declarations to support recursion
bool ArrayEquals(emscripten::val a, emscripten::val b);
bool MapEquals(emscripten::val a, emscripten::val b);
bool OptionalEquals(emscripten::val a, emscripten::val b);

/*
 * Generic equality method that can handle both primitive JS types, and bound embind types.
 * Handles NULL/NAN and recursing through arrays correctly.
 * A bound C++ type will defer to a bound ".equals" method, which is assumed to call
 * the C++ equality operator.
 */
bool ElementEquals(emscripten::val a, emscripten::val b)
{

    // Identical primitives or identical handles to the same C++ instance
    if (a.strictlyEquals(b)) {
        return true;
    }

    // This is JS typeOf, will give `object`, `boolean`, `number`, etc.
    const std::string typeA = a.typeOf().as<std::string>();
    const std::string typeB = b.typeOf().as<std::string>();

    // Mixed type can't be equal. (is the string comparison neccesary?)
    if (typeA != typeB) {
        return false;
    }

    // Below this point, we should have complex, non primitive objects of the same type. (Primitives are dealt with in the strictlyEquals check, which does NAN != NAN as expected)
    if (typeA != "object") {
        return false;
    }

    // Null is "object" in JS, special-case it.
    if (a.isNull() || b.isNull()) {
        return a.isNull() && b.isNull();
    }

    // Recurse if we find an array. ArrayEquals ends up calling this function.
    if (a.isArray() && b.isArray()) {
        return ArrayEquals(a, b);
    }

    // Recurse if we find a Map. MapEquals ends up calling this function.
    static const emscripten::val globalMap = emscripten::val::global("Map");
    if (a.instanceof(globalMap) && b.instanceof(globalMap)) {
        return MapEquals(a, b);
    }

    // Beyond this point we're only making meaningful claims for bound C++ objects.
    // This check is arguably true by definition of having made it this far, but check anyway.
    if (!bindings::utils::IsBoundHandle(a) || !bindings::utils::IsBoundHandle(b)) {
        return false;
    }

    // Constructor.name is the string passed to emscripten::class_<T>("Name"). Check if they're the same type.
    const std::string cppTypeA = a["constructor"]["name"].as<std::string>();
    const std::string cppTypeB = b["constructor"]["name"].as<std::string>();
    if (cppTypeA != cppTypeB) {
        return false;
    }

    // Defer to the bound equals method (which will usually call the C++ equality operator).
    if (!a["equals"].isUndefined()) {
        return a.call<bool>("equals", b);
    }

    // Reached when both sides are bound handles of the same class but the class
    // didn't bind an `equals` method. No semantically expressed means of equality,
    // so we treat them as unequal.
    return false;
}

/*
 * Utility function to assert whether one array is equal to another one.
 * There is no way to do complex type constraints in embind without manually declaring every possible type
 * therefore, we use `any` here, and plan to do a manual typescript overlay file that
 * uses typescript generics to constrain these vals to the same [T].
 *
 * Will accept any array type that exposes a "length" property and has a [] operator.
 */
bool ArrayEquals(emscripten::val a, emscripten::val b)
{

    // Early out reference equality and type checks.
    if (a.strictlyEquals(b)) {
        return true;
    }
    if (!a.isArray() || !b.isArray()) {
        return false;
    }

    // Are they the same length?
    const unsigned aLength = a["length"].as<unsigned>();
    const unsigned bLength = b["length"].as<unsigned>();
    if (aLength != bLength) {
        return false;
    }

    // Do elementwise comparison, using a function that accounts for both JS and CPP bound objects, along with array recursion
    for (unsigned i = 0; i < aLength; ++i) {
        if (!ElementEquals(a[i], b[i])) {
            return false;
        }
    }
    return true;
}

/*
 * Utility function to assert whether one JS Map is equal to another, mirroring ArrayEquals.
 *
 * Equality is order-insensitive: the maps must have the same size AND, for every key in a,
 * b must contain the same key with a deeply-equal value. Values are compared deeply with
 * elementEquals. Keys are matched via the Map's native lookup (SameValueZero), which is
 * appropriate as keys should always be primitives.
 *
 * As with arrayEquals, embind can't express the key/value type constraint, so we take `any`
 * and rely on a planned typescript overlay to constrain both maps to the same <K, V>.
 */
bool MapEquals(emscripten::val a, emscripten::val b)
{

    // Early out reference equality and type checks.
    if (a.strictlyEquals(b)) {
        return true;
    }

    // Are both inputs maps?
    static const emscripten::val mapGlobal = emscripten::val::global("Map");
    if (!a.instanceof(mapGlobal) || !b.instanceof(mapGlobal)) {
        return false;
    }

    // Are they the same size?
    if (a["size"].as<unsigned>() != b["size"].as<unsigned>()) {
        return false;
    }

    // Array.from(map) yields [[key, value], ...]. Order is irrelevant: for each key in a,
    // look the key up in b and deeply compare the values.
    // Equal sizes plus every key of a present-and-matching in b implies map equality.
    const emscripten::val entriesA = emscripten::val::global("Array").call<emscripten::val>("from", a);
    const unsigned length = entriesA["length"].as<unsigned>();
    for (unsigned i = 0; i < length; ++i) {
        const emscripten::val entryA = entriesA[i];
        const emscripten::val key = entryA[0];

        if (!b.call<bool>("has", key)) {
            return false; // key missing from b
        }
        if (!ElementEquals(entryA[1], b.call<emscripten::val>("get", key))) {
            return false; // values differ
        }
    }
    return true;
}

bool OptionalEquals(emscripten::val a, emscripten::val b) {
      // Early out reference equality and type checks.
      // a will equal b if both are undefined
      if (a.strictlyEquals(b)){
        return true;
      }

      if (a.isUndefined() != b.isUndefined()) {
        return false;
      }

      return ElementEquals(a, b);
}

}

EMSCRIPTEN_BINDINGS(CSPEquality)
{
    emscripten::function("arrayEquals", &ArrayEquals);
    emscripten::function("elementEquals", &ElementEquals);
    emscripten::function("mapEquals", &MapEquals);
    emscripten::function("optionalEquals", &OptionalEquals);
}
