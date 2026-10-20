# Clients

This document is intended to provide guidance for clients that will consume the CSP web bindings library.
The contents may be moved into the main README or elsewhere at some point in the future.

## Explicit Resource Management

The CSP web bindings API makes extensive use of the [Explicit Resource Management](https://github.com/tc39/proposal-explicit-resource-management) proposal for JavaScript, which at the time of writing has been recommended for implementation but has not yet been incorporated into ECMAScript standard. This includes the `using` keyword, the `dispose` symbol as well as the `Disposable` interface for TypeScript.

As a result, the features of the proposal are not yet available in all browsers, in particular Safari - see [Can I use](https://caniuse.com/wf-explicit-resource-management) for an updated guide to browser support.

### Build Pipeline Configuration

For client applications needing to work in Safari, the build pipeline must be configured to transpile the `using` keyword in client code. The CSP bindings library itself provides the necessary polyfills for the `dispose` symbols.

#### Vite

Consumers using Vite must explicitly set their build and development targets to `es2022` or lower (or a specific older browser version such as `safari16`) in their `vite.config.ts` or `vite.config.js` file:

```js
import { defineConfig } from 'vite';

export default defineConfig({
  build: {
    // Transpile 'using' for production
    target: ['es2022', 'safari16']
  },
  // Replace 'oxc' with 'esbuild' for versions of Vite < 8.0
  oxc: {
    // Transpile 'using' during local development
    target: ['es2022', 'safari16']
  }
});
```

#### Webpack

With Webpack, transpilation of JavaScript or TypeScript will depend on the loader used by the application such as `babel-loader`, `swc-loader` or `ts-loader`. The requirement is that the specific loader must target `ES2022` or lower, or a specific browser version which does not support Explicit Resource Management, e.g. Safari <= 26.

For example with `babel-loader` the `babel.config.json` should be set up as follows:

```jsonc
{
  "presets": [
    [
      "@babel/preset-env",
      {
        "targets": "defaults, safari >= 16"
      }
    ]
  ],
  // For Babel < 7.28, also use the explicit resource management transform plugin
  "plugins": ["@babel/plugin-transform-explicit-resource-management"]
}
```

### TypeScript Configuration

For client applications using TypeScript, version 5.2 or higher must be used to support the `using` keyword in TypeScript code. In addition, `"esnext.disposable"` should be added to the `"lib"` array in the `tsconfig.json` file in order for the necessary type definitions to be available:

```jsonc
{
  "compilerOptions": {
    "lib": ["DOM", "ES2022", "esnext.disposable"]
    //                       ^^^^^^^^^^^^^^^^^^^
  }
}
```

## Debugging

The package contains a debug build of the bindings alongside the release build, available via the `./debug` entry point or `loadCsp({ debug: true })` from `./loader`. This build can be debugged at the C++ level in Chrome with the [C/C++ DevTools Support (DWARF) extension](https://chromewebstore.google.com/detail/cc++-devtools-support-dwa/pdcpmagijalfljmkmjngeonclgbbannb). See [Debugging](Debugging.md) for more on the debugging workflow itself.

### Serving the Debug Symbols

The debug symbols are in a separate file to not inside `debug/connected-spaces-platform-bindings.wasm`, `debug/connected-spaces-platform-bindings.debug.wasm`, which the DevTools extension downloads only when DevTools is open. The `.wasm` records the filename of its accompanying `.debug.wasm`, and the extension looks for it in the same directory as the URL the `.wasm` was loaded from. So the application must serve the `.debug.wasm` next to the `.wasm`, keeping the name `connected-spaces-platform-bindings.debug.wasm`.

If the `.debug.wasm` can't be found, the bindings still run normally, but DevTools will not show any C++ sources.

> [!NOTE]
>
> The `.debug.wasm` is a large file (> 100 MB). As it is only needed for debugging, serve it with debug builds only.

Bundlers find the `.wasm` through a `new URL('connected-spaces-platform-bindings.wasm', import.meta.url)` reference in the bindings JavaScript, and usually emit it into an output directory under a hashed name, e.g. `assets/connected-spaces-platform-bindings-a1b2c3d4.wasm`. Nothing in the JavaScript references the `.debug.wasm`, so the bundler will not emit it. You need to copy it into the same output directory as the emitted `.wasm`, keeping its original (unhashed) name.

#### Vite

If the Vite dev server serves the bindings package directly out of `node_modules`, the `.debug.wasm` is already next to the `.wasm`, and nothing more is needed.

For a build, emit the `.debug.wasm` into the assets directory (`build.assetsDir`, `assets` by default) with a small plugin:

```js
import { defineConfig } from 'vite';
import { readFileSync } from 'node:fs';
import { createRequire } from 'node:module';

const require = createRequire(import.meta.url);

// Emits the CSP debug symbols next to the bundled debug .wasm.
function cspDebugSymbols() {
  return {
    name: 'csp-debug-symbols',
    apply: 'build',
    generateBundle() {
      this.emitFile({
        type: 'asset',
        fileName: 'assets/connected-spaces-platform-bindings.debug.wasm',
        source: readFileSync(
          require.resolve('@magnopus-opensource/connected-spaces-platform-bindings-web/debug/connected-spaces-platform-bindings.debug.wasm')
        )
      });
    }
  };
}

export default defineConfig({
  // Only include this in the builds you want to debug.
  plugins: [cspDebugSymbols()]
});
```

#### Webpack

Webpack emits the `.wasm` as an asset module, by default to the root of the output directory (see `output.assetModuleFilename`). Copy the `.debug.wasm` to the same place, for example with [copy-webpack-plugin](https://webpack.js.org/plugins/copy-webpack-plugin/):

```js
const CopyPlugin = require('copy-webpack-plugin');

module.exports = {
  plugins: [
    new CopyPlugin({
      patterns: [
        {
          from: require.resolve('@magnopus-opensource/connected-spaces-platform-bindings-web/debug/connected-spaces-platform-bindings.debug.wasm')
          // `to` defaults to the root of the output directory. Set it to wherever the .wasm is
          // emitted if you have changed output.assetModuleFilename.
        }
      ]
    })
  ]
};
```

### Checking It Works

With DevTools open, the extension logs a message in the console when it loads (or fails to load) the debug symbols. If they fail to load, find the URL the `.wasm` was loaded from in the `Network` tab, replace the filename with `connected-spaces-platform-bindings.debug.wasm`, and open that URL in the browser. If it doesn't download, the file has not been copied to the right place.
