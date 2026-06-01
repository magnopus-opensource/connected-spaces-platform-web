#include "emscripten/bind.h"
#include "emscripten/val.h"
#include "Handles.h"
  
namespace {

// Forward declaration to support recursion
bool ArrayEquals(emscripten::val a, emscripten::val b);

/*
 * Generic equality method that can handle both primitive JS types, and bound embind types.
 * Handles NULL/NAN and recursing through arrays correctly.
 * A bound C++ type will defer to a bound ".equals" method, which is assumed to call
 * the C++ equality operator.
 */
bool ElementEquals(emscripten::val a, emscripten::val b){

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

    // Beyond this point we're only making meaningful claims for bound C++ objects. 
    // This check is arguably true by definition of having made it this far, but check anyway.
    if (!bindings::utils::IsBoundHandle(a) || !bindings::utils::IsBoundHandle(b)){
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
bool ArrayEquals(emscripten::val a, emscripten::val b) {

      // Early out reference equality and type checks.
      if (a.strictlyEquals(b)) return true;
      if (!a.isArray() || !b.isArray()) return false;

      // Are they the same length?
      const unsigned aLength = a["length"].as<unsigned>();
      const unsigned bLength = b["length"].as<unsigned>();
      if (aLength != bLength) return false;

      // Do elementwise comparison, using a function that accounts for both JS and CPP bound objects, along with array recursion
      for (unsigned i = 0; i < aLength; ++i) {
          if (!ElementEquals(a[i], b[i])) return false;
      }
      return true;
}
}

EMSCRIPTEN_BINDINGS(CSPEquality) {
      emscripten::function("arrayEquals", &ArrayEquals);
      emscripten::function("elementEquals", &ElementEquals);
  }
