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

// Polyfill DisposableStack
// See https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/DisposableStack
if (typeof DisposableStack === 'undefined') {
  globalThis.DisposableStack = class DisposableStack {
    #disposers = [];
    #disposed = false;

    get disposed() {
      return this.#disposed;
    }

    use(value) {
      if (this.#disposed) {
        throw new ReferenceError('DisposableStack already disposed');
      }
      if (value != null) {
        const dispose = value[Symbol.dispose];
        if (typeof dispose !== 'function') {
          throw new TypeError('Value is not disposable');
        }
        this.#disposers.push(() => dispose.call(value));
      }
      return value;
    }

    adopt(value, onDispose) {
      if (this.#disposed) {
        throw new ReferenceError('DisposableStack already disposed');
      }
      if (typeof onDispose !== 'function') {
        throw new TypeError('onDispose must be a function');
      }
      this.#disposers.push(() => onDispose(value));
      return value;
    }

    defer(onDispose) {
      if (this.#disposed) {
        throw new ReferenceError('DisposableStack already disposed');
      }
      if (typeof onDispose !== 'function') {
        throw new TypeError('onDispose must be a function');
      }
      this.#disposers.push(onDispose);
    }

    move() {
      if (this.#disposed) {
        throw new ReferenceError('DisposableStack already disposed');
      }
      const newStack = new DisposableStack();
      newStack.#disposers = this.#disposers;
      this.#disposers = [];
      this.#disposed = true;
      return newStack;
    }

    dispose() {
      if (this.#disposed) {
        return;
      }
      this.#disposed = true;

      let hasError = false;
      let error;

      // Disposers are executed in reverse order of how they were added
      while (this.#disposers.length > 0) {
        const disposer = this.#disposers.pop();
        try {
          disposer();
        } catch (e) {
          if (hasError) {
            // If an error already occurred, wrap it in a SuppressedError
            error = new SuppressedError(e, error, 'An error was suppressed during disposal.');
          } else {
            error = e;
            hasError = true;
          }
        }
      }

      if (hasError) {
        throw error;
      }
    }

    [Symbol.dispose]() {
      this.dispose();
    }
  };

  // Ensure Object.prototype.toString.call(stack) returns '[object DisposableStack]'
  Object.defineProperty(globalThis.DisposableStack.prototype, Symbol.toStringTag, {
    value: 'DisposableStack',
    configurable: true,
    enumerable: false,
    writable: false
  });
}

/**
 * End polyfills.
 */
