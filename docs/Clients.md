# Clients

This document is intended to provide guidance for clients that will consume the CSP web bindings library.
The contents may be moved into the main README or elsewhere at some point in the future.

## Explicit Resource Management

The CSP web bindings API makes extensive use of the [Explict Resource Management](https://github.com/tc39/proposal-explicit-resource-management) proposal for JavaScript, which at the time of writing has been recommended for implemention but has not yet been incorporated into ECMAScript standard. This includes the `using` keyword, the `dispose` symbol as well as the `Disposable` interface for TypeScript.

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
