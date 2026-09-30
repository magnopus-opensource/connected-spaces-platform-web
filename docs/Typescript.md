# TypeScript

TypeScript definitions for the bindings are generated using Embind's `--emit-tsd` option.

To ensure that the best possible types are generated, it's important to include parameter names when binding methods, functions or constructors.

```cpp
emscripten::class_<Person>("Person")
    .class_function("create(name, age)", +[](std::string name, int age)
    //                      ^^^^  ^^^
    {
      return Person(std::move(name), age);
    })
    .function("setAge(age)", &Person::SetAge)
    //                ^^^
```

## Type Overlays

In certain situations the `--emit-tsd` option is not able to generate satisfactory types for the bindings.

This is notably the case for any C++ functions that operate on JavaScript values and that take `emscripten::val` parameters. Here Embind will generate function declarations using the `any` type for these parameters.

To get around this, we've implemented a mechanism to replace specific Embind-generated types with handwritten type overlays. These are located in the [`src/type-overlays`](../src/type-overlays) folder.

To add new type overlays for C++ bindings:

1. Create a new `.d.ts` file in the `type-overlays` folder with a new interface for the custom types. This file should have the same name as the corresponding C++ source file.
1. In the `index.d.ts` import the newly created `.d.ts` file and add the interface to the `TypeOverrides` union type, following the instructions in that file.

## Type Declarations for Exported JavaScript

We can also use the same mechanism to expose TypeScript definitions for JavaScript code that is exposed by the library as part of its public interface, via `addToLibrary`/`__postset` in [`src/js`](../src/js). Not being C++, any JavaScript in the public interface will be invisible to Embind and will not have any corresponding type declarations in the generated `.d.ts` file.

Declarations for a given JavaScript file from the [`src/js`](../src/js) folder go in a file of the same name [`src/js-declarations`](../src/js-declarations) folder, so for example declarations for `js-library.js` go in `js-library.d.ts`.

For example, if we have a function included in the module API via `addToLibrary`:

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

Assuming it lives in `src/js/maths.js`, we declare it in a matching `maths.d.ts` in the [`src/js-declarations`](../src/js-declarations) folder:

```ts
declare function add(a: number, b: number): number;

export type { add };

export interface MathsDeclarations {
  readonly add: typeof add;
}
```

Then in the [`index.d.ts`](../src/js-declarations/index.d.ts) file we import and append `MathsDeclarations` to the `JsDeclarations` intersection type.

The `declare function add` and its `export type { add }` are carried across into the published declarations automatically. They only need declaring in `maths.d.ts` itself.

## Merging the Type Overlays and Declarations into the Generated d.ts File

Both the type overlays and additional type declarations are merged into the Embind-generated `d.ts` file with the [`merge-dts`](tools\merge-dts\merge-dts.ts) script. This is run as part of the build process so that a single, unified declarations file is shipped for the library.

When the script performs the merge, it ensures the following rules for the type overlays and declarations are true. The script will fail otherwise to avoid replacing or declaring types when not intended.

- For type overlays, all the replaced types _must_ already exist in the Embind-generated `d.ts` file. Type overlays replace existing type declarations.
- For the additional type declarations, the types _must not_ already exist in the Embind-generated `d.ts` file. They must only add new types to the declarations.
