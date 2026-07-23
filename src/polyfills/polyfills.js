/**
 * Polyfills for the library, prepended to the generated JavaScript code.
 */

/**
 * Polyfills for Explicit Resource Management.
 * Client applications need to ensure they transpile the `using` syntax via their build tooling.
 */

// Polyfill dispose Symbols
globalThis.Symbol.dispose ??= Symbol('Symbol.dispose');
globalThis.Symbol.asyncDispose ??= Symbol('Symbol.asyncDispose');

// Polyfill SuppressedError for exception handling during disposal
// See https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/SuppressedError
if (typeof globalThis.SuppressedError === 'undefined') {
  globalThis.SuppressedError = class SuppressedError extends Error {
    constructor(error, suppressed, message) {
      super(message || 'An error was suppressed during disposal.');
      this.name = 'SuppressedError';
      this.error = error;
      this.suppressed = suppressed;
    }
  };
}

/**
 * End polyfills.
 */
