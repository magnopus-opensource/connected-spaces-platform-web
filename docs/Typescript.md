# TypeScript

TypeScript definitions for the bindings are generated using Embind's `--emit-tsd` option.

To ensure that the best possible types are generated, it's important to include parameter names when binding methods, functions or constructors.

```cpp
emscripten::class_<Person>("Person")
    .class_function("create(name, age)", +[](std::string name, int age)
    //                      ^^^^  ^^^
    {
      return Person(value, std::move(name, age));
    })
    .function("setAge(age)", &Person::SetAge)
    //                ^^^
```

## Type Overlays

In certain situations the `--emit-tsd` option is not able to generate satisfactory types for the bindings.

This is notably the case for any C++ functions that operate on JavaScript values and that take `emscripten::val` parameters. Here Embind will generate function declarations using the `any` type for these parameters.

To get around this, we've implemented a mechanism to replace specific Embind-generated types with handwritten type overlays. These are located in the [`src/type-overlays`](../src/type-overlays) folder. The

The [`index.d.ts`](../src/type-overlays/index.d.ts) file is what ultimately exposes all the TypeScript type definitions for users of the library. This file combines the Embind-generated types with the custom type overlays in the `type-overlays` folder.

To add new type overlays for C++ bindings:

1. Create a new `.d.ts` file in the `type-overlays` folder with a new interface for the custom types. This file should have the same name as the corresponding C++ source file.
1. In the `index.d.ts` import the newly created `.d.ts` file and add the interface to the `TypeOverrides` union type, following the instructions in that file.

### Type Overlays for Exported JavaScript

We can also use the same mechanism to expose TypeScript definitions for JavaScript code that is exposed by the library as part of its public interface.

For example if we have function that is included in the module API via `addToLibrary`:

```js
function add(a, b) {
  return a + b;
}

// Expose add in the public interface
addToLibrary({
  $add__postset: "Module['add'] = add;",
  $add: add
});
```

We can add a type overlay for the function in an `add.d.ts` file, located in the [`src/type-overlays`](../src/type-overlays) folder:

```ts
declare function add(a: number, b: number): number;

export type { add };

export interface AddOverrides {
  readonly add: typeof add;
}
```

Then in the [`index.d.ts`](../src/type-overlays/index.d.ts) file we can import and append the `AddOverrides` interface to the `TypeOverrides` union type. Note that we must also re-export the `add` type in this same file for it to be available to users via `import type` directives.
