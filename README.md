# Connected Spaces Platform Web

Typescript bindings for in-browser use of the [connected spaces platform library](https://github.com/magnopus-opensource/connected-spaces-platform), generated via embind.

> [!WARNING]
>
> 👷 Under construction. Check back in a few months.

## Setup

Before continuing, ensure you have [Emscripten](https://emscripten.org/docs/tools_reference/emsdk.html) (emsdk) installed and activated on your system. The currently supported version is **5.0.2**.

You will also need the build system [CMake](https://cmake.org/download/) (**3.28 or above**), the package manager [Conan 2](https://docs.conan.io/2/installation.html) (tested on **2.27.0**, but version shouldn't matter much), as well as [Typescript.](https://www.npmjs.com/package/typescript) (**5.9.3**).

> [!TIP]
>
> Our [build automation](.github/workflows/build-bindings.yml) provides an alternate example on how to build and install these bindings.

### Building CSP

You will need an install of [CSP](https://github.com/magnopus-opensource/connected-spaces-platform) to build against. At time of writing, CSP was in the process of transitioning to a newer, cmake based build system. These bindings use the new build systems. Produce an install directory somewhere on your local system. Refer to the CSP repo for instructions on how to do this, or as always, look at our [build automation](.github/actions/build-csp-emsdk/action.yml) for an example.

> [!NOTE]
>
> During the transition to the new CSP build system, dependent projects are required to fetch dependencies, although this should not be the case for very long. Presuming you wish to make a debug build, run the following from the root directory to do so : 
>
> ```
> conan install . --profile:host=profiles/host/emscripten-debug --build=missing

### Building the Bindings

From here on out, it is a regular Cmake triad.

```
export CMAKE_PREFIX_PATH="path/to/csp/install/dir"
cmake --preset debug
cmake --build --preset debug
cmake --build --preset debug --target install
```

All going well, this should produce you an install directory containing a `.wasm`, a `.js`, a `.ts` as well as a `package.json` such that they can be installed as a node module. The install target places these at `./CSP-WASM-Bindings` by default.

### Testing

Tests are written in `vitest`, emulating a chromium browser via `playwright`.

To run tests, first navigate to the tests directory and install the dependencies

```
cd tests
npm install
npx playwright install chromium
```

Then go ahead and run the tests

```
npm test
```

If all goes well, you'll see something like the following (although with a fair few more tests, this guide is being written right at the start of the project.)

```
> test
> vitest run


 RUN  v4.1.6 C:/dev/connected-spaces-platform-web/tests

 ✓  chromium  src/memory.test.ts (6 tests) 297ms
 ✓  chromium  src/version.test.ts (2 tests) 286ms

 Test Files  2 passed (2)
      Tests  8 passed (8)
   Start at  10:40:53
   Duration  1.80s (transform 0ms, setup 0ms, import 1.03s, tests 583ms, environment 0ms)
```