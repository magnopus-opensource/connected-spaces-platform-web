# Interop Types

An "interop-type" is a C++ type that goes across the binding surface, presenting an interface that can be used in javascript.

For the most part, when we say this, we mean containers. Whilst every bound type is technically an "interop type", they tend not to require special handling or consideration. The following types however, do:

- `Array`
- `List`
- `Map`
- `Optional`
- `String`

This project attempts to present underlying C++ in as natural a way as possible for the javascript developer. This means that when we have a choice between representing a type as a JS primitive or a bound proxy, we will tend to bias towards the JS primitive unless there is a compelling reason not to. This does imply performance and interface trade-offs on occasion.

## Arrays & Lists

[Relevant implementation.](../src/bindings/containers/Array.h)

Whilst arrays and lists are different types in the underlying C++, the JS bindings treat them practically identically, as they are both presented as javascript arrays.

Methods that take or return arrays and lists, such as the following

```cpp
csp::common::Array<int> GetArray() { return {1, 2, 3}; }
void SetArray(csp::common::Array<int> array);
```

will generate JS/TS code that produces primitive arrays, such as:

```ts
using array = csp.GetArray(); // [1, 2, 3]
csp.SetArray([1, 2, 3]);
```

This goes the same for const ref returns and arguments, JS/TS semantics are identical.

Keep in mind that this means that, despite `List` being a dynamically resizable container, JS will not see size updates until it re-fetches the list. This has the advantage of completely removing any potential iterator invalidation bugs, but does have some interface and performance implications. This may change later if we find this simple approach to be unsuitable.

> [!NOTE]
> As Lists and Arrays are from the JS/TS perspective, the same type, utility functions such as `disposeArray` or `arrayEquals` are the right thing to call on both arrays and lists.

## Maps

Maps are similar to arrays and lists, in that they are represented as a javascript `Map` object, where the values may be value or pointer types.

There are two specificities to be aware of.

- Currently, `csp::common::Map` is based on `std::map`, which is ordered by strict-weak ordering of the key value. This is not the case for JS maps, which are insertion-ordered. In cases of equality, we do not enforce an ordering guarantee, so maps constructed in JS will compare equal (when using the csp utilities) to C++ maps even if out of order. However, order will be preserved when getting a map back from C++ as a return. This will almost never matter.

- Whilst the values of a map may be bound to any type, as either a value or pointer type, the keys are more limiting. In order to play nice with JS/TS identity comparison, important for maps to function semantically, keys must be bindable as JS primitives. Effectively an integral or string type. This also allows us not to worry about allocation and disposal in keys, which is nice.

## Optional

With the dynamic nature of JavaScript, there is no need for an 'optional' type as such. The pattern is instead to use a `T | undefined` type in TypeScript for optional values or return types.

The built-in bindings for `std::optional` already work in this way and with `csp::common::Optional` we have aimed to do the same.

This means that for a C++ function returning a `csp::common::optional` value such as the following example:

```cpp
csp::common::Optional<int> GetValue() { return csp::common::Optional<int>(1); }
```

the bindings will present it in TypeScript as:

```ts
getValue(): number | undefined
```

For function parameters (and object properties) optional values can either be typed as `T | undefined` or `T?`, the difference being that for the former a value must be explicitly provided (even undefined) whereas in the latter the value can be omitted.
The bindings use the `T?` version for function parameters.

For example with a function accepting an optional parameter:

```cpp
void SetValue(csp::common::Optional<int> value) { ... }
```

the TypeScript equivalent will be:

```ts
setValue(value?: number): void
```

Which can be used as follows:

```ts
setValue(1);
setValue();
setValue(undefined);
```

The implementation of the bindings for `csp::common:Optional` uses the built-in Embind bindings for `std::optional`, which some additional handling for optional of types with an attached `[Symbol.dispose]` to ensure support for `using` on optional values where expected.
Note that `using` on a value of type `(T & Disposable) | undefined` works because `using` on `undefined` or `null` values is a no-op.

One key difference to be mindful of when registering `csp::common::Optional` types is that `emscripten::register_optional` should be used instead of `emscripten::register_type`. For example:

```cpp
emscripten::register_optional<csp::common::String>();
```

An exception to that is for registering `csp::common::Optional` of types with an attached `[Symbol.dispose]` for which `emscripten::register_type` should still be used so that they are presented with `Disposable` in TypeScript:

```cpp
emscripten::register_type<bindings::utils::JSDisposable<csp::common::Optional<csp::common::List<BindingsTestType>>>>("(BindingsTestType[] & Disposable) | undefined");
```

## Strings

C++ strings and in particular the `csp::common::String` type are presented in JavaScript as the standard JS `string`.

As such, for functions with string parameters or return types as below:

```cpp
csp::common::String GetString() { return csp::common::String("Hello!"); }
void SetString (csp::common::String string);
```

the bindings will enable standard JavaScript strings to be used:

```ts
const str = csp.getString(); // 'Hello!'
csp.setString("Hello back!");
```

The binding for string piggy-backs on the built-in Embind implementation for `std::string`, and because of that involves an extra copy into an `std::string` that we may look to remove in future if it becomes an issue.

Note that due to string always being stack-based, there is no need to dispose them so TS/JS do not have any additional `dispose` function as with other interop types.

---

_For the following conceptual documentation, we tend to use `Array` as an example. But the concepts apply to all interop container types._

## Value and Pointer Types

> [!Hint]
>
> A _Value array_ is something like `csp::common::Array<MyType>`.
>
> A _Pointer array_ would be `csp::common::Array<MyType*>`.

A tradeoff in presenting JS array primitives is a need to copy. When accepting or returning a value array, _every element in the list_ will be copied. This is true regardless of whether the signature of argument or return is `T` or `const T&`.

> [!NOTE]
>
> The philosophy on whether to copy `const &` returns and arguments is still developing, and may change
> as we further integrate the api.

Pointer arrays do not invoke copies (well ... they copy _pointers_ but that's not what people normally mean). A pointer array in the interface implies that the interface is _non owning_. Users should not claim ownership of the objects in a pointer array.

The binding surface supports pointer arrays where the contained objects are owned in JS/TS, as below:

```ts
using elem1 = csp.MyType.create(1, "one");
using elem2 = csp.MyType.create(2, "two");
csp.setPointerArray([elem1, elem2]);
```

Keep in mind that when doing this, the JS/TS developer is responsible for maintaining ownership of `elem1` and `elem2`. Once they are disposed, the underlying C++ will have dangling pointers, potentially leading to a crash.

This is not a common interface expression. The normal case is for pointer arrays to be used to provide non-owning handles to C++ owned objects, such as below.

```ts
let array = csp.GetPointerArray();
let elem = array[0];
// elem is usable, but owned by the underlying C++, you don't need to worry about deleting it.
```

### Non Owning Pointers

In most (practically all) cases, we enrich pointers to be non-owning, to reflect their ownership category coming out of CSP, as CSP generally returns non-owning pointers.
What this means practically is that we forbid use of owning functions on the javascript handle, specifically `delete`, `deleteLater`, `clone`, and `Symbol.dispose`.

Pointers coming out of the api should be enriched via calling the `NonOwningVal` utility function at the bindings site.

To perform the enrichment, we need to work with raw `val` objects. Thus, it is neccesary to provide these pointer types with typescript names explicitly, via the use
of `EMSCRIPTEN_DECLARE_VAL_TYPE(MyTypePointer)` combined with `emscripten::register_type<MyTypePointer>("MyType");`.

See [Handles.h](../src/bindings/utils/Handles.h).

## Binding Types

In order to get the [disposability](#disposability) behavior we're looking for, binding containers has some amount of complexity.

You must declare the types such that the typescript emitted is annotated correctly. A type declaration must exist for each container type that appears in the interface.

```cpp
    emscripten::register_type<csp::common::Array<int>>("number[]"); //Primitive value array
    emscripten::register_type<csp::common::Array<BindingsTestType>>("MyType[]"); //Bound value array
    emscripten::register_type<csp::common::Array<BindingsTestType*>>("(MyType | null)[]"); //Bound pointer array

    emscripten::register_type<bindings::utils::JSDisposable<csp::common::Array<int>>>("(number[] & Disposable)"); //Disposable primitive value array
    emscripten::register_type<bindings::utils::JSDisposable<csp::common::Array<BindingsTestType>>>("(MyType[] & Disposable)"); //Disposable bound value array
```

There are a few things here that may seem surprising.

Firstly, `JSDisposable`, what's that? This is how we get `using` support. By binding a different type, we can allow ourselves a different typescript annotation where that type occurs. These can be thought of as the bindings for return types. By doing this, we can still accept regular shaped arrays as function arguments ([1,2,3] would not fit into a `Disposable` slot), whilst not sacrificing disposability for returns where it matters.

The tradeoff here is that you must do the type conversion at the binding site. As a general rule, anything that is returning a value array should perform a conversion. If you fail to do this, you will be unable to declare arrays out of CSP as `using`.

```cpp
.function("setArrayBasicTypeByValue(value)", &MyObjectType::SetArrayBasicTypeByValue) // Setter function, bind normally
.function("getArrayBasicTypeByValue", +[](const MyObjectType& self) {
        // Getter function, needs to be disposable.
        return bindings::utils::JSDisposable<csp::common::Array<int>>{self.GetArrayBasicTypeByValue()};
    })
```

Secondly is why primitive types are disposable? Honestly this is a bit of a style decision. It is a simple rubric to attempt to use `using` whenever possible, and there is no harm to this. Primitives are conceptually value types from a semantic perspective, even though they do not need explicit cleanup. This could reasonably be done the alternate way, demanding users use `const` or `let` rather than `using` for these returns.

## Equality

As JS does not support any sort of operator extension, this project provides free functions to bridge assumed C++ capability (in this case value equality) into JS/TS.

```ts
const a = [csp.MyType.create(1, "one"), csp.MyType.create(2, "two")];
const b = [csp.MyType.create(1, "one"), csp.MyType.create(2, "two")];
expect(csp.arrayEquals(a, b)).toBe(true); //C++ style value-equality
expect(a === b).toBe(false); //JS style identity equality
```

These helpers attempt to call the bound `equals` method on types that have it, which will almost always call the underlying C++ equality operator.

You may find these equality helpers at the top level of the module, they are defined in [Equality.cpp.](../src/bindings/containers/Equality.cpp)

## Disposability

Much effort has been put to try and generate typescript such that `using` is meaningful. We recommend defaulting to `using` for all possible cases, as it will ensure that necessary leak-avoidance is done.

Owned C++ memory, such as in value arrays, must be deleted to avoid leaks, and is thus presented in typescript as `Disposable`. A value array will be presented as `(MyType[] & Disposable)`. This allows it to be declared correctly with `using`.

Non owned memory is not disposable, and if you are type-checking correctly, will reject being declared as `using`, as is appropriate to convey that there is no disposal going on. In an ideal world, the inverse of this check would also exist as a lint rule, to insist that you use `using` unless you explicitly declare that you don't want to, and take on manual disposal responsibility yourself.

Speaking of manual disposal, the `using` mechanism calls into the free function `disposeArray`/`disposeMap` etc defined in [Disposal.cpp](../src/bindings/containers/Disposal.cpp). You are free to use these instead if you wish more manual control.
