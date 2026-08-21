import { describe, it, expect, beforeAll } from 'vitest';
import { loadCSP } from '../loadModule';
import { BindingsTestType, type MainModule } from 'connected-spaces-platform-bindings';

/*
 * Tests for the binding of awaitable callbacks into the typescript boundary.
 */

describe('Awaitable Callbacks', () => {
  let csp: MainModule;

  beforeAll(async () => {
    csp = await loadCSP();
  });

  it('Await void result', async () => {
    using helper = csp.CallbacksBindingMechanismsTestType.create();

    // @ts-expect-error There is no return value normally but we want to test that the promise
    // resolves to undefined
    using result = await helper.callbackFunctionNoArgsAsync();

    expect(result).toBeUndefined();
  });

  it('Void result with promise', async () => {
    using helper = csp.CallbacksBindingMechanismsTestType.create();

    let thenCallbackCalled = false;

    const resultPromise = helper.callbackFunctionNoArgsAsync().then(() => {
      thenCallbackCalled = true;
    });

    await resultPromise;

    expect(thenCallbackCalled).toBe(true);
  });

  it('Await primitive result', async () => {
    const helper = csp.CallbacksBindingMechanismsTestType.create();

    const result = await helper.callbackFunctionPrimitiveArgAsync();

    expect(result).toBe(10);
  });

  it('Primitive result with promise', async () => {
    const helper = csp.CallbacksBindingMechanismsTestType.create();

    let thenCallbackCalled = false;

    const resultPromise = helper.callbackFunctionPrimitiveArgAsync().then((value) => {
      thenCallbackCalled = true;

      expect(value).toBe(10);
    });

    await resultPromise;

    expect(thenCallbackCalled).toBe(true);
  });

  it('Await value type result', async () => {
    const helper = csp.CallbacksBindingMechanismsTestType.create();

    using result = await helper.callbackFunctionValueArgAsync();

    expect(result.value).toBe(1);
    expect(result.name).toBe('One');
  });

  it('Value type result with promise', async () => {
    const helper = csp.CallbacksBindingMechanismsTestType.create();

    let thenCallbackCalled = false;

    const resultPromise = helper.callbackFunctionValueArgAsync().then((result) => {
      thenCallbackCalled = true;

      expect(result.value).toBe(1);
      expect(result.name).toBe('One');
    });

    await resultPromise;

    expect(thenCallbackCalled).toBe(true);
  });

  it('Await const ref result', async () => {
    const helper = csp.CallbacksBindingMechanismsTestType.create();

    using result = await helper.callbackFunctionValueArgByConstRefAsync();

    expect(result.value).toBe(1);
    expect(result.name).toBe('One');
  });

  it('Const ref result with promise', async () => {
    const helper = csp.CallbacksBindingMechanismsTestType.create();

    let thenCallbackCalled = false;

    const resultPromise = helper.callbackFunctionValueArgByConstRefAsync().then((result) => {
      thenCallbackCalled = true;

      expect(result.value).toBe(1);
      expect(result.name).toBe('One');
    });

    await resultPromise;

    expect(thenCallbackCalled).toBe(true);
  });

  it('Await pointer result', async () => {
    const helper = csp.CallbacksBindingMechanismsTestType.create();

    // Pointer is non-owning so technically no dispose is needed, but we allow 'using' for
    // consistency
    using result = await helper.callbackFunctionPointerArgAsync();

    expect(result).not.toBeFalsy();

    expect(result?.value).toBe(1);
    expect(result?.name).toBe('One');
  });

  it('Pointer result with promise', async () => {
    const helper = csp.CallbacksBindingMechanismsTestType.create();

    let thenCallbackCalled = false;

    const resultPromise = helper.callbackFunctionPointerArgAsync().then((result) => {
      thenCallbackCalled = true;

      expect(result).not.toBeFalsy();

      expect(result?.value).toBe(1);
      expect(result?.name).toBe('One');
    });

    await resultPromise;

    expect(thenCallbackCalled).toBe(true);
  });

  // ----------
  // Optional types

  it('Await optional value type result', async () => {
    using helper = csp.CallbacksBindingMechanismsTestType.create();

    using result = await helper.callbackFunctionValueOptAsync();

    expect(result).toBeDefined();
    expect(result?.value).toBe(1);
    expect(result?.name).toBe('One');
  });

  it('Optional value type result with promise', async () => {
    using helper = csp.CallbacksBindingMechanismsTestType.create();

    let thenCallbackCalled = false;

    const resultPromise = helper.callbackFunctionValueOptAsync().then((result) => {
      thenCallbackCalled = true;

      expect(result).toBeDefined();
      expect(result?.value).toBe(1);
      expect(result?.name).toBe('One');
    });

    await resultPromise;

    expect(thenCallbackCalled).toBe(true);
  });

  it('Await optional pointer type result', async () => {
    using helper = csp.CallbacksBindingMechanismsTestType.create();

    using result = await helper.callbackFunctionPointerOptAsync();

    expect(result).not.toBeFalsy();
    expect(result?.value).toBe(1);
    expect(result?.name).toBe('One');
  });

  it('Optional pointer type result with promise', async () => {
    using helper = csp.CallbacksBindingMechanismsTestType.create();

    let thenCallbackCalled = false;

    const resultPromise = helper.callbackFunctionPointerOptAsync().then((result) => {
      thenCallbackCalled = true;

      expect(result).not.toBeFalsy();
      expect(result?.value).toBe(1);
      expect(result?.name).toBe('One');
    });

    await resultPromise;

    expect(thenCallbackCalled).toBe(true);
  });

  it('Null optional value type result with promise', async () => {
    using helper = csp.CallbacksBindingMechanismsTestType.create();

    let thenCallbackCalled = false;

    const resultPromise = helper.callbackFunctionNullValueOptAsync().then((result) => {
      thenCallbackCalled = true;

      expect(result).toBeUndefined();
    });

    await resultPromise;

    expect(thenCallbackCalled).toBe(true);
  });

  it('Null optional pointer type result with promise', async () => {
    using helper = csp.CallbacksBindingMechanismsTestType.create();

    let thenCallbackCalled = false;

    const resultPromise = helper.callbackFunctionNullPointerOptAsync().then((result) => {
      thenCallbackCalled = true;

      expect(result).toBeUndefined();
    });

    await resultPromise;

    expect(thenCallbackCalled).toBe(true);
  });

  it('Await primitive result from multi primitive input arg function', async () => {
    const helper = csp.CallbacksBindingMechanismsTestType.create();

    const result = await helper.callbackFunctionMultiInputPrimitiveArgAsync(1, 2);

    expect(result).toBe(3);
  });

  // ----------
  // Pointer lifetime

  it('Await pointer result lifetime', async () => {
    const helper = csp.CallbacksBindingMechanismsTestType.create();

    const beforeCallbackCount = csp.BindingsTestType.aliveCount();

    {
      using result = await helper.callbackFunctionPointerArgAsync();

      expect(result?.value).toBe(1);
      expect(result?.name).toBe('One');

      // Pointer is non-owning: no copy made, no aliveCount change, no disposal needed technically
      // but we allow disposing the handle for consistency
      expect(csp.BindingsTestType.aliveCount()).toBe(beforeCallbackCount);
    }

    expect(csp.BindingsTestType.aliveCount()).toBe(beforeCallbackCount);
  });

  // ----------
  // Value type lifetime

  it('Await value type lifetime', async () => {
    const helper = csp.CallbacksBindingMechanismsTestType.create();

    const beforeCallbackCount = csp.BindingsTestType.aliveCount();

    {
      using result = await helper.callbackFunctionValueArgAsync();

      expect(result.value).toBe(1);

      const afterCallbackCount = csp.BindingsTestType.aliveCount();
      expect(afterCallbackCount).toBe(beforeCallbackCount + 1);
    }

    expect(csp.BindingsTestType.aliveCount()).toBe(beforeCallbackCount);
  });

  it('Value type lifetime with promise', async () => {
    const helper = csp.CallbacksBindingMechanismsTestType.create();

    const beforeCallbackCount = csp.BindingsTestType.aliveCount();

    const resultPromise = helper.callbackFunctionValueArgAsync().then((result) => {
      // Can also use a DisposableStack instead, i.e. disposableStack.use(result)
      using disposableResult = result;

      expect(result.value).toBe(1);

      const afterCallbackCount = csp.BindingsTestType.aliveCount();
      expect(afterCallbackCount).toBe(beforeCallbackCount + 1);
    });

    // Await the promise - the then() callback will run first
    await resultPromise;

    expect(csp.BindingsTestType.aliveCount()).toBe(beforeCallbackCount);
  });

  it('Await const ref type lifetime', async () => {
    const helper = csp.CallbacksBindingMechanismsTestType.create();

    const beforeCallbackCount = csp.BindingsTestType.aliveCount();

    {
      using result = await helper.callbackFunctionValueArgByConstRefAsync();

      expect(result.value).toBe(1);

      const afterCallbackCount = csp.BindingsTestType.aliveCount();
      expect(afterCallbackCount).toBe(beforeCallbackCount + 1);
    }

    expect(csp.BindingsTestType.aliveCount()).toBe(beforeCallbackCount);
  });

  it('Const ref type lifetime with promise', async () => {
    const helper = csp.CallbacksBindingMechanismsTestType.create();

    const beforeCallbackCount = csp.BindingsTestType.aliveCount();

    const resultPromise = helper.callbackFunctionValueArgByConstRefAsync().then((result) => {
      using disposableResult = result;

      expect(result.value).toBe(1);

      const afterCallbackCount = csp.BindingsTestType.aliveCount();
      expect(afterCallbackCount).toBe(beforeCallbackCount + 1);
    });

    await resultPromise;

    expect(csp.BindingsTestType.aliveCount()).toBe(beforeCallbackCount);
  });

  it('Referencing an un-disposed value arg outside an async callback works', async () => {
    using helper = csp.CallbacksBindingMechanismsTestType.create();

    let thenCallbackCalled = false;
    let liftedResult: BindingsTestType | undefined;

    const resultPromise = helper.callbackFunctionValueArgAsync().then((result) => {
      thenCallbackCalled = true;

      liftedResult = result;

      expect(result.value).toBe(1);
      expect(result.name).toBe('One');
    });

    await resultPromise;

    expect(thenCallbackCalled).toBe(true);

    expect(liftedResult).toBeDefined();

    // We can use the lifted result here because it has not been disposed yet
    expect(liftedResult?.value).toBe(1);
    expect(liftedResult?.name).toBe('One');

    // Dispose the lifted result
    liftedResult?.delete();
  });

  // ----------
  // Optional lifetime

  it('Await optional value type result lifetime', async () => {
    const helper = csp.CallbacksBindingMechanismsTestType.create();

    const beforeCallbackCount = csp.BindingsTestType.aliveCount();

    {
      using result = await helper.callbackFunctionValueOptAsync();

      expect(result).toBeDefined();

      expect(result?.delete).toBeDefined();
      expect(result?.[Symbol.dispose]).toBeDefined();

      expect(csp.BindingsTestType.aliveCount()).toBe(beforeCallbackCount + 1);
    }

    expect(csp.BindingsTestType.aliveCount()).toBe(beforeCallbackCount);
  });

  // ----------
  // Multiple awaits or thens on the same promise

  it('Value type lifetime, multiple await', async () => {
    const helper = csp.CallbacksBindingMechanismsTestType.create();

    const beforeCallbackCount = csp.BindingsTestType.aliveCount();

    const resultPromise = helper.callbackFunctionValueArgAsync();

    {
      using result1 = await resultPromise;
      // Needs to be const here to avoid double dispose, only one 'using' should be used on the same
      // promise result.
      const result2 = await resultPromise;

      expect(result1.value).toBe(1);
      expect(result1.name).toBe('One');

      expect(csp.BindingsTestType.aliveCount()).toBe(beforeCallbackCount + 1);
    }

    expect(csp.BindingsTestType.aliveCount()).toBe(beforeCallbackCount);

    // This will throw - underlying value has already been deleted here
    await expect(async () => {
      const result3 = await resultPromise;
      result3.value;
    }).rejects.toThrow();
  });

  it('Value type lifetime, count does not increase with multiple awaits', async () => {
    const helper = csp.CallbacksBindingMechanismsTestType.create();

    const beforeCallbackCount = csp.BindingsTestType.aliveCount();

    const resultPromise = helper.callbackFunctionValueArgAsync();

    // Note no 'using' here to not dispose the result
    const result1 = await resultPromise;

    expect(csp.BindingsTestType.aliveCount()).toBe(beforeCallbackCount + 1);

    // Note no 'using' here
    const result2 = await resultPromise;

    // We're accessing the same underlying object again so the alive count will not have increased
    expect(csp.BindingsTestType.aliveCount()).toBe(beforeCallbackCount + 1);

    result1.delete();
  });

  it('Value type lifetime, disposing after first use means value is deleted for subsequent awaits', async () => {
    const helper = csp.CallbacksBindingMechanismsTestType.create();

    const resultPromise = helper.callbackFunctionValueArgAsync();

    {
      // With 'using' to dispose result
      using result1 = await resultPromise;
    }

    const result2 = await resultPromise;

    // The value has already been disposed
    expect(result2.isDeleted()).toBe(true);

    // Trying to access the value throws an error
    expect(() => result2.value).toThrow();
  });

  it('Value type lifetime with promise, multiple then', async () => {
    const helper = csp.CallbacksBindingMechanismsTestType.create();

    const beforeCallbackCount = csp.BindingsTestType.aliveCount();

    const resultPromise = helper.callbackFunctionValueArgAsync();

    resultPromise.then((result) => {
      expect(result.value).toBe(1);
      expect(result.name).toBe('One');

      expect(csp.BindingsTestType.aliveCount()).toBe(beforeCallbackCount + 1);
    });

    resultPromise.then((result) => {
      // We've not disposed the result in the first then() callback so we can still access it here

      expect(result.value).toBe(1);
      expect(result.name).toBe('One');

      // It's the same underlying object so the alive count will not have increased
      expect(csp.BindingsTestType.aliveCount()).toBe(beforeCallbackCount + 1);
    });

    await Promise.resolve();

    // No disposal happened so the count will not have reduced
    expect(csp.BindingsTestType.aliveCount()).toBe(beforeCallbackCount + 1);

    // Do this to dispose the result
    using result = await resultPromise;
  });
});
