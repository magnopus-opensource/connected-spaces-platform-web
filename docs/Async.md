# Callbacks & Async

## Callbacks

Callback mechanisms are defined in [Callbacks.h](../src/bindings/async/Callbacks.h).
As you may expect, in order to enable C++ to call back into javascript, we must capture javascript callables in C++ ones via an adaptation layer.

At the most basic level, this looks like this.

```cpp
void JsFunctionWithCallback(emscripten::val jsCallback)
    auto adaptedCallback = [jsCallback]() {
        jsCallback();
    }
    CSP.CPPFunctionWithCallback(adaptedCallback);
}
```

The javascript object (callback), is captured in the lambdas capture list, the lambda, when called, does nothing but call the javascript callback. In this way, we enclose a javascript callable inside a C++ closure, and pass the C++ callable into CSP, bridging the gap.

## Callback Arguments

Callback arguments are the tricky thing. If you were to be using embind in a basic way, it would require you to explicitly delete objects in javascript compulsively, in order to avoid leaks, like so:

```js
system.callbackFunc((arg1, arg2) => {
  /* Do something */
  arg1.delete();
  arg2.delete();
});
```

We find this unacceptable for the following reasons:

- People forget to do this in callbacks. If there were syntax to automatically capture the variables in `using` rules that would be one thing (which is what we hope becomes the pattern for regular returns for this problem).
- You cannot apply a blanket rule, pointer types across the API should not be deleted, and doing so will cause crashes later on in the program.

For this to make sense, you need to be aware of the following invariant:

- **All pointers that come out of CSP are non-owning.**

As we have solved this problem with `using` doing the right thing regardless of pointer or value types, we might have chosen to simply require rebinding all callback variables, like so :

```js
system.callbackFunc((arg1, arg2) => {
  using myArg1 = arg1;
  using myArg2 = arg2;
  /* Do something */
});
```

Honestly, perfectly legitimate. However, we still felt this would be forgotten too often, although we admit that this was a difficult choice, as the symmetry of approach with direct returns here is appealing.

We instead apply automatic RAII style cleanup to arguments in callback functions at the bindings level. The user should never need to `.delete()` or bind callback objects to `using` inside the callback. This is conveyed to the user as, unlike objects out of return functions, callback arguments should never be marked `Disposable`.

```js
system.callbackFunc((arg1, arg2) => {
  /* Do something without thinking about memory */
});
```

We feel this is the common case, processing something inside the callback, so are optimizing for that.

A tradeoff for this, (and to be fair, would be for the `using` approach as well) is if you wish to extract an object from a callback for later use, you must copy it out. We plan to provide extended copy operators to make this less burdensome, especially in the case of containers types.

Technically, pointer types can be referenced out of methods as they are non owning, although it is still unfortunately difficult to know which argument is a pointer and which is a value. We may just solve this in a caveman way by making sure we include the value category in variable names, as JS does not have access to that information at the source level.

In the future, it would be best to simplify our theory via changing the underlying API. The fact that `SpaceEntity` (and items on it, like `Component`) are uniquely expressed in the api as non-owning pointers is an unfortunate niggle preventing us from expressing a cleaner invariant. It's possible that once we manage to remove the debt of the old-wrapper generator, CSP can start expressing ownership in its own types via smart pointers, which would let us lean into what embind actually wants us to be doing.

> ![NOTE]
>
> In your travels, you may note the method, `NonOwningVal` which overrides the `.delete`, `.deletelater`, `.clone` and `Symbol["Dispose"]` methods on pointer objects, causing them to throw.
> In an ideal world, we would apply this to all callback arguments, pointers or no, as they should never be interacted with in this way.
> However, current disposal techniques require `.delete`, so we cannot convert them into a throw so casually.
> However, I believe this is tractable, as whilst we may be overriding the handles own `.delete`, it is always still available on the prototype, we could
> use that instead for the internal RAII disposal, like : `emscripten::val::global("Object").call<emscripten::val>("getPrototypeOf", v)["delete"];`
> Maybe something to think about if you want even more safety against footguns.

## Off Thread Callbacks

TODO

## Async

Awaitable async is build atop callbacks, and works like this ... TODO
