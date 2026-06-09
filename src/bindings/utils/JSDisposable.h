#pragma once

#include <optional>

namespace bindings::utils
{
/*
 * Return-only wrapper for types crossing C++ -> JS.
 *
 * TS's `using` requires the static type to declare `[Symbol.dispose]`, but embind
 * registers one TS name per C++ type and uses it in both return and parameter
 * positions. To take arrays as an example, We can't make `csp::common::Array<T>`
 * itself appear as `T[] & Disposable`without breaking typescript validation on 
 * setters for regular value arrays of primitives.
 * (a plain `[1,2,3]` literal isn't assignable to that type). 
 * So, we route returns through a distinct wrapper type registered
 * as `(T[] & Disposable)`, while `csp::common::Array<T>` stays `T[]` for parameters.
 *
 * This does mean you need to convert to this type at the binding site for returns,
 * which is a trade-off. Forgetting to do this will cause the typescript type checker
 * to disallow you from using `using` when storing an array return.
 * 
 * Supports both owned and non owned memory.
 * In the case of a value return out of CSP `T Func();`, it populates the optional.
 * In the case of a reference return `T& Func();`, it points the view to the CSP owned memory directly.
 * You should use `view` to get at the memory here in Wiretype bindings, it'll always point to the right thing.
 * Note that this does not mean that const refs don't copy over the boundary, just that we avoid a copy
 * when using this wrapper type. You could get rid of the `view` and just use `owned` and things would work identically,
 * just with a redundant copy in the bindings for no reason.
 * 
 * It is only necessary to use this adapter for types that have custom disposal behaviour,
 * which will primarily be containers.
 */
 template <typename T>
  class JSDisposable {
      // ownedType must be declared before view: member init order follows
      // declaration order, and the rvalue ctor binds view to *ownedType.
      private:
        // In theory, if we hit a reference return that is non-copyable, we could use this as the branching axis
        // for owned/non-owned memory in the wiretype bindings, rather than pointer/value. It would be more
        // honest in a way, albeit more complex conceptually.
        std::optional<T> ownedType; 
      public:
        //Points to either externally managed memory, or `ownedType`. Use this in the Wiretype bindings.
        const T& view;

    JSDisposable(T&& obj) : ownedType(std::move(obj)), view(*ownedType) {}
    JSDisposable(const T& obj) : ownedType(std::nullopt), view(obj) {}

    JSDisposable(const JSDisposable&) = delete;
    JSDisposable(JSDisposable&&) = delete;
    JSDisposable& operator=(const JSDisposable&) = delete;
    JSDisposable& operator=(JSDisposable&&) = delete;
    // reject const prvalues out of an overabundance of caution. Stop const values going to the const-ref constructor and dangling.
    JSDisposable(const T&& obj) = delete;  
};
} // namespace bindings::utils