# Callbacks & Async

## Callbacks

Callback mechanisms are defined in [Callbacks.h](../src/bindings/async/Callbacks.h).
As you may expect, in order to enable C++ to call back into javascript, we must capture javascript callables in C++ ones via an adaptation layer.

At the most basic level, this looks like this.

```cpp
void JsFunctionWithCallback(emscripten::val jsCallback) {
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

As we have solved this problem with `using` doing the right thing regardless of pointer or value types, we might have chosen to simply require rebinding all callback variables, like so:

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

A tradeoff for this, (and to be fair, would be for the `using` approach as well) is if you wish to extract an object from a callback for later use, you must copy it out. We plan to provide extended copy operators to make this less burdensome, especially in the case of container types.

Technically, pointer types can be referenced out of methods as they are non-owning, although it is still unfortunately difficult to know which argument is a pointer and which is a value. We may just solve this in a caveman way by making sure we include the value category in variable names, as JS does not have access to that information at the source level.

In the future, it would be best to simplify our theory via changing the underlying API. The fact that `SpaceEntity` (and items on it, like `Component`) are uniquely expressed in the api as non-owning pointers is an unfortunate niggle preventing us from expressing a cleaner invariant. It's possible that once we manage to remove the debt of the old-wrapper generator, CSP can start expressing ownership in its own types via smart pointers, which would let us lean into what embind actually wants us to be doing.

### Lifting out of callbacks

All arguments in callbacks are `NonOwning`, meaning that they have no `.delete`, `.deleteLater`, or `Symbol.Dispose`

To get an argument out of a callback, clone it.

```js
let myArg;
system.callbackFunc((arg) => {
  myArg = arg.clone();
});
```

This is not a copy, it is a reference count incrementation. Normally, callback arguments are owned by our backend RAII system, which decrements the reference count (ie, calls `delete`) when
they fall out of callback scope. This is why you do not need to worry about ownership in callbacks.

However, lifted arguments, having incremented the reference count, are now owners of the memory. As such, you need to ensure they are deleted.
You may either rebind them into `using` variables, or just manually call `delete` or `deleteLater` on them.

It does not matter if the argument is a pointer or value type, you can call `clone` on it all the same, it'll do the right thing, (pointer deletes never delete underlying memory regardless). It should be rather tricky to get this wrong, as embind will error quite cleanly if you end up trying to do a double delete, or access memory after deletion, or use a deletion operation in a context where it is forbidden. The only thing you need to keep in mind is not to leak lifted arguments.

## Off-Thread Callbacks

From a user perspective, off-thread callbacks behave in exactly the same way as on-thread callbacks, in fact, you won't even be able to tell they're off thread, as they end up being executed back on the same thread that you registered them on.

This behavioural symmetry has only one exception ... exceptions!

When a callback is given back to the user off-thread, and it happens to throw, exceptions will not be propagated, instead, the exception will be caught and logged to `error` before it escapes the internal callback machinery.

This should not normally be a problem, as it would be rather unwise to an underlying library to rely on exceptions in an off-thread context anyway, they only bring heartache.

### How it all works

To quote the [embind](https://emscripten.org/docs/api_reference/val.h.html) docs directly.

> [!WARNING]
>
> JavaScript values can’t be shared across threads, so neither can val instances that bind them.

This is our core problem. Building such a highly threaded library to support a platform with this sort of constraint was certainly an interesting choice. At time of writing, CSP is actively working to dissolve this problem entirely by queueing network-sourced callback commands such that they can be dispatched on the main thread.

Luckily for us, Emscripten provides a way for us to do this same sort of proxy queue behaviour ourselves, [the proxying API.](https://emscripten.org/docs/api_reference/proxying.h.html)

What the bindings do is, when registering a callback, save its `pthread_id`. Then, when dispatching a callback, we do a check to see if we're on the _Affine thread_ (ie, are we dispatching the callback on the same thread we registered it on). If not, we put the callback into the proxy queue, which asks the browser event loop to execute it on the desired thread, thus bypassing the problem.

Implementationally, this gets rather complex, for two specific reasons:

- The `Val` api we're using does not encourage multithreading _at all_, we have to do some fancy tricks to make sure we remain memory safe even as execution dances between threads.
- Even destructors need to be run on the affine thread.

I won't get any more into specifics, you can view the implementation yourself [here.](../src/bindings/async/ThreadAffineCallback.h)

## Async

Awaitable async is build atop callbacks, with some important differences in terms of usage.

For callback-based C++ functions such as:

```cpp
void CppFunctionWithCallback(std::function<void(int)> callback);
```

We may bind it as an async function in JavaScript, which will be presented as follows with TypeScript:

```js
function jsFunctionWithCallbackAsync(): Promise<number>
```

This can be done in addition to or instead of binding it as a function taking a callback in JavaScript as described above.

Conceptually, the implementation looks something like this:

```cpp
void jsFunctionWithCallbackAsync() {
  emscripten::val promise, resolve = createPromise();

  auto callback = [](int argument) {
    resolve(argument);
  };

  CppFunctionWithCallback(callback);

  return promise;
}
```

Instead of providing a callback from the JavaScript side, the async version creates a JavaScript promise and callback resolving the promise within the bindings layer. This callback is supplied to the C++ function and the promise is returned from the function call.

This async version of the function is used as any other function returning a promise in JavaScript, either with `await` or a `then()` function.

```js
const result = await jsFunctionWithCallbackAsync();
// Do something with result (number)

// or

jsFunctionWithCallbackAsync().then((result) => {
  // Do something with result (number)
});
```

### Promise Fulfillment Values

The value returned by a successful promise is called the _fulfillment value_ (`result` in the examples above). The fulfillment value from async calls in the library may be `Disposable`, meaning that the caller must dispose it to avoid leaking.

This can done easily and simply with the `await` syntax and the `using` keyword, which will dispose the argument at the end of the current scope.

```js
{
  using result = await jsFunctionReturningDisposableAsync();
  ...
  // result disposed on leaving scope
}
```

Disposing the fulfillment value becomes slightly more awkward when using `then()`, requiring either rebinding the variable, calling `delete` or `Symbol.dispose` manually, or using a `DisposableStack` if the variable is lifted out of the `then` function. Callers must be especially attentive in this case to avoid leaking.

```js
jsFunctionReturningDisposableAsync().then((result) => {
  // Rebind result to dispose on leaving scope
  using disposableResult = result;
  ...
  // disposableResult disposed here
});
```

With the fulfillment value corresponding to the callback argument for the bound C++ function, you may notice that the need for explicit disposal by the caller is a departure from the automatic argument disposal provided for non-async callback functions. This difference is especially apparent when using `then` functions as in the example just above.
The manual disposal is necessary to be compatible with how promises work in JavaScript. In particular, promises may be stored and awaited at any point in the future, with the lifetime of the fulfillment value at a minimum tied to that of the promise, meaning there is no natural place for the automatic disposal to occur.

However, the fact that no automatic disposal happens implies there is no need to perform any additional clones on values returned from an async function in JavaScript.

```js
let liftedResult;
...
jsFunctionReturningDisposableAsync().then((result) => {
  // Lift result out of function - no clone needed
  liftedResult = result;
  ...
  // disposableResult disposed here
});
...
// Remember to dispose liftedResult!
```
