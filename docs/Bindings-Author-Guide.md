# CSP Bindings Author Guide

This document is intended to provide guidance for anyone working on updating the CSP bindings, for example to bind new CSP APIs.

For general information about binding C++ types, refer to the [Embind documentation](https://emscripten.org/docs/porting/connecting_cpp_and_javascript/embind.html).

For a quick summary of rules for CSP bindings, see [CSP Binding Rules](#csp-binding-rules) at the end of this document.

## Bindings File Structure

The file structure of the bindings source files mirrors that of the headers in the CSP source tree. This is to easily locate the bindings corresponding to a given CSP type: simply follow the same relative path as the CSP header file in the [/src/bindings/csp](../src/bindings/csp) folder to locate the `.cpp` file with the binding.

## Enums

Enums are bound using the `number` style (`emscripten::enum_value_type::number`) as follows:

```cpp
emscripten::enum_<EResultCode>("EResultCode", emscripten::enum_value_type::number)
    .value("Init", EResultCode::Init)
    .value("InProgress", EResultCode::InProgress)
    .value("Success", EResultCode::Success)
    .value("Failed", EResultCode::Failed);
```

## Classes

Classes are bound as follows:

```cpp
emscripten::class_<CspClass>("CspClass")
    .class_function(
        "create(value, name)", +[](int value, csp::common::String name) { return CspClass(value, std::move(name)); })
    .property("value", &CspClass::GetValue, &CspClass::SetValue)
    .property("name", &CspClass::GetName)
    .function("equals(other)", &CspClass::operator==)
    .class_function("StaticFunction", +[] { return CspClass::StaticFunction(); });
```

### Class Constructors

Class constructors are bound as static `create` functions using `class_function`. As function overloading is not possible in JavaScript, if you wish to bind several constructors, each must have a different name:

```cpp
emscripten::class_<CspClass>("CspClass")
    .class_function(
        "create(value, name)", +[](int value, csp::common::String name) { return CspClass(value, std::move(name)); })
    .class_function(
        "createWithValue(value)", +[](int value) { return CspClass(value); })
    ...
```

### Class Inheritance and Polymorphism

If a class inherits from a base class, include it in the class bindings definition using `emscripten::base`:

```cpp
emscripten::class_<CspClass, emscripten::base<CspBaseClass>>("CspClass")
   ...
```

### Properties

Public instance fields, or value getter and setter pairs may be bound as JavaScript properties.

```cpp
emscripten::class_<CspClass>("CspClass")
    ...
    .property("value", &CspClass::GetValue, &CspClass::SetValue) // .value is readable and writable
    .property("name", &CspClass::GetName) // .name is read-only
    ...
```

Only getters returning types that do not require disposal by the user should be bound as properties, namely primitive types, enums and strings, as well as containers of these three types.
For other return types, the getter (and setter) should be bound as a standard function.

### Equality Operators

Many CSP classes implement an equality `==` operator. This should be bound as an `equals(other)` function, as JavaScript does not support operator overloading:

```cpp
emscripten::class_<CspClass>("CspClass")
    ...
    .function("equals(other)", &CspClass::operator==)
    ...
```

### Value Objects

Some classes may be bound as _value objects_. In JavaScript, users of these types do not need to worry about memory lifetime management.

This works for classes/structs that have only public members:

```cpp
emscripten::value_object<Vector2>("Vector2")
    .field("x", &Vector2::X)
    .field("y", &Vector2::Y);
```

It's also possible for classes/structs with private members, where **each member has a public getter and setter**:

```cpp
emscripten::value_object<Vector2>("Vector2")
    .field("x", &Vector2::GetX, &Vector2::SetX)
    .field("y", &Vector2::GetY, &Vector2::SetY);
```

For a class to be bindable as a value object, the following criteria must be true:

- It must be default constructible and trivially copyable
- Each field must be independently readable and writable, either by being a public member or by having a dedicated getter and setter
- It must not be polymorphic
- No reference semantics - value objects are copied across the boundary

## Callback and Async Functions

### Callback-based Functions

When binding callback-based functions, first use the `MAKE_CALLBACK` macro to define the `ToNativeCallback` utility function, an associated _val type_ for the JavaScript callback and define the TypeScript callback type annotation.

```cpp
MAKE_CALLBACK(CspCallbackType, CallbackJsType, "(value: boolean) => void")
```

Then bind callback-based function as follows:

```cpp
emscripten::class_<CspClass>("CspClass")
    ...
    .function("setCallback(callback)", +[](CspClass& self, CallbackJsType callback)
    {
        self.SetCallback(ToNativeCallback(callback));
    })
    ...
```

### Async Functions

Callback-based functions may also be bound as async functions returning a promise in JavaScript.
To do this, you must first ensure a promise type corresponding to the callback type argument is registered:

```cpp
EMSCRIPTEN_DECLARE_VAL_TYPE(PromiseOfBoolean);

// Inside an EMSCRIPTEN_BINDINGS block
emscripten::register_type<PromiseOfBoolean>("Promise<boolean>");
```

If not already done, use `MAKE_CALLBACK` macro for the callback type as above:

```cpp
MAKE_CALLBACK(CspCallbackType, CallbackJsType, "(value: boolean) => void")
```

Then the `Promisify` function is used to return a promise from the function binding:

```cpp
emscripten::class_<CspClass>("CspClass")
    ...
    .function("asyncFunction", +[](CspClass& self)
    {
        return Promisify<PromiseOfBoolean>(
            [&](emscripten::val cb) { self.CallbackFunction(ToNativeCallback(cb.as<CallbackJsType>())); }
        );
    })
    ...
```

## Pointers

When pointers to a specific type appear in the bindings as either a function argument or return value, the pointer type must be registered.
By convention, the registered type should be named as `{TypeName}Pointer`:

```cpp
emscripten::register_type<CspClassPointer>("CspClass | null");
```

If the pointer type is used as a return type, a corresponding _val type_ should be declared. This may be placed in a header file if it is used in several places:

```cpp
EMSCRIPTEN_DECLARE_VAL_TYPE(CspClassPointer);
```

All pointers should be non-owning for the JavaScript code and not disposed by the user. The only exception to this rule are material pointers returned by the asset system.
When returning a non-owning pointer from a function, wrap it in a `NonOwningVal`:

```cpp
emscripten::class_<AnotherCspClass>("AnotherCspClass")
    ...
    .function(
        "functionReturningPointer",
        +[](AnotherCspClass& self)
        {
            return bindings::utils::NonOwningVal<CspClassPointer>(self.FunctionReturningPointer());
        })
    ...
```

Doing this makes it impossible for the user to dispose/delete the pointer.

## CSP Binding Rules

- CSP enums are bound using the `number` style (`emscripten::enum_value_type::number`)
- Only getter functions returning types not requiring user disposal should be bound as properties
- CSP `System` classes are bound without constructor functions, as they are not user-constructible
- Pointers returned by the CSP bindings should be non-owning, i.e. owned by CSP and not user-disposed, with the exception of material pointers returned by the asset system

### Unbound Types

The following CSP types and functions are purposefully not bound.

Namespace `Common`:

- `MimeTypeHelper`, `MimeTypes` and `FileExtensions`
