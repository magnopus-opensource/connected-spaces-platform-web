import { describe, it, expect, beforeAll } from 'vitest';
import { loadCSP } from '../loadModule';
import { BindingsTestType, type MainModule } from 'connected-spaces-platform-bindings';
import { describeOnAndOffThread, untilCallbacksSettled } from './testUtils';

/*
 * Tests for the binding of awaitable callbacks into the typescript boundary.
 */

describeOnAndOffThread('Awaitable Callbacks', (offThread) => {
  let csp: MainModule;

  beforeAll(async () => {
    csp = await loadCSP();
  });

  it('Await void result', async () => {
    using helper = csp.CallbacksBindingMechanismsTestType.create(offThread);

    // @ts-expect-error There is no return value normally but we want to test that the promise
    // resolves to undefined
    using result = await helper.callbackFunctionNoArgsAsync();

    expect(result).toBeUndefined();
  });

  it('Void result with promise', async () => {
    using helper = csp.CallbacksBindingMechanismsTestType.create(offThread);

    let thenCallbackCalled = false;

    const resultPromise = helper.callbackFunctionNoArgsAsync().then(() => {
      thenCallbackCalled = true;
    });

    await resultPromise;

    expect(thenCallbackCalled).toBe(true);
  });

  it('Await primitive result', async () => {
    using helper = csp.CallbacksBindingMechanismsTestType.create(offThread);

    const result = await helper.callbackFunctionPrimitiveArgAsync();

    expect(result).toBe(10);
  });

  it('Primitive result with promise', async () => {
    using helper = csp.CallbacksBindingMechanismsTestType.create(offThread);

    let thenCallbackCalled = false;

    const resultPromise = helper.callbackFunctionPrimitiveArgAsync().then((value) => {
      thenCallbackCalled = true;

      expect(value).toBe(10);
    });

    await resultPromise;

    expect(thenCallbackCalled).toBe(true);
  });

  it('Await value type result', async () => {
    using helper = csp.CallbacksBindingMechanismsTestType.create(offThread);

    using result = await helper.callbackFunctionValueArgAsync();

    expect(result.value).toBe(1);
    expect(result.name).toBe('One');
  });

  it('Value type result with promise', async () => {
    using helper = csp.CallbacksBindingMechanismsTestType.create(offThread);

    let thenCallbackCalled = false;

    const resultPromise = helper.callbackFunctionValueArgAsync().then((result) => {
      thenCallbackCalled = true;

      expect(result.value).toBe(1);
      expect(result.name).toBe('One');
    });

    await resultPromise;
    await untilCallbacksSettled(csp, () => thenCallbackCalled);

    expect(thenCallbackCalled).toBe(true);
  });

  it('Await const ref result', async () => {
    using helper = csp.CallbacksBindingMechanismsTestType.create(offThread);

    using result = await helper.callbackFunctionValueArgByConstRefAsync();

    expect(result.value).toBe(1);
    expect(result.name).toBe('One');
  });

  it('Const ref result with promise', async () => {
    using helper = csp.CallbacksBindingMechanismsTestType.create(offThread);

    let thenCallbackCalled = false;

    const resultPromise = helper.callbackFunctionValueArgByConstRefAsync().then((result) => {
      thenCallbackCalled = true;

      expect(result.value).toBe(1);
      expect(result.name).toBe('One');
    });

    await resultPromise;
    await untilCallbacksSettled(csp, () => thenCallbackCalled);

    expect(thenCallbackCalled).toBe(true);
  });

  it('Await pointer result', async () => {
    using helper = csp.CallbacksBindingMechanismsTestType.create(offThread);

    // Pointer is non-owning so technically no dispose is needed, but we allow 'using' for
    // consistency
    using result = await helper.callbackFunctionPointerArgAsync();

    expect(result).not.toBeFalsy();

    expect(result?.value).toBe(1);
    expect(result?.name).toBe('One');
  });

  it('Pointer result with promise', async () => {
    using helper = csp.CallbacksBindingMechanismsTestType.create(offThread);

    let thenCallbackCalled = false;

    const resultPromise = helper.callbackFunctionPointerArgAsync().then((result) => {
      thenCallbackCalled = true;

      expect(result).not.toBeFalsy();

      expect(result?.value).toBe(1);
      expect(result?.name).toBe('One');
    });

    await resultPromise;
    await untilCallbacksSettled(csp, () => thenCallbackCalled);

    expect(thenCallbackCalled).toBe(true);
  });

  // ----------
  // Container types

  it('Await container of pointers result', async () => {
    using helper = csp.CallbacksBindingMechanismsTestType.create(offThread);

    using result = await helper.callbackFunctionContainerOfPointersAsync();

    expect(result.length).toBe(2);

    expect(result[0]?.value).toBe(1);
    expect(result[0]?.name).toBe('One');
    expect(result[1]?.value).toBe(2);
    expect(result[1]?.name).toBe('Two');
  });

  it('Container of pointers result with promise', async () => {
    using helper = csp.CallbacksBindingMechanismsTestType.create(offThread);

    let thenCallbackCalled = false;

    const resultPromise = helper.callbackFunctionContainerOfPointersAsync().then((pointerContainerArg) => {
      thenCallbackCalled = true;

      expect(pointerContainerArg.length).toBe(2);

      expect(pointerContainerArg[0]?.value).toBe(1);
      expect(pointerContainerArg[0]?.name).toBe('One');
      expect(pointerContainerArg[1]?.value).toBe(2);
      expect(pointerContainerArg[1]?.name).toBe('Two');
    });

    await resultPromise;
    await untilCallbacksSettled(csp, () => thenCallbackCalled);

    expect(thenCallbackCalled).toBe(true);
  });

  it('Await container of values result', async () => {
    using helper = csp.CallbacksBindingMechanismsTestType.create(offThread);

    using result = await helper.callbackFunctionContainerOfValuesAsync();

    expect(result.length).toBe(2);

    expect(result[0]?.value).toBe(1);
    expect(result[0]?.name).toBe('One');
    expect(result[1]?.value).toBe(2);
    expect(result[1]?.name).toBe('Two');
  });

  it('Container of values result with promise', async () => {
    using helper = csp.CallbacksBindingMechanismsTestType.create(offThread);

    let thenCallbackCalled = false;

    const resultPromise = helper.callbackFunctionContainerOfValuesAsync().then((valueContainerArg) => {
      thenCallbackCalled = true;

      expect(valueContainerArg.length).toBe(2);

      expect(valueContainerArg[0]?.value).toBe(1);
      expect(valueContainerArg[0]?.name).toBe('One');
      expect(valueContainerArg[1]?.value).toBe(2);
      expect(valueContainerArg[1]?.name).toBe('Two');
    });

    await resultPromise;
    await untilCallbacksSettled(csp, () => thenCallbackCalled);

    expect(thenCallbackCalled).toBe(true);
  });

  it('Await container of values by const ref result', async () => {
    using helper = csp.CallbacksBindingMechanismsTestType.create(offThread);

    using result = await helper.callbackFunctionContainerOfValuesByConstRefAsync();

    expect(result.length).toBe(2);

    expect(result[0]?.value).toBe(1);
    expect(result[0]?.name).toBe('One');
    expect(result[1]?.value).toBe(2);
    expect(result[1]?.name).toBe('Two');
  });

  it('Container of values by const ref result with promise', async () => {
    using helper = csp.CallbacksBindingMechanismsTestType.create(offThread);

    let thenCallbackCalled = false;

    const resultPromise = helper.callbackFunctionContainerOfValuesByConstRefAsync().then((valueContainerArg) => {
      thenCallbackCalled = true;

      expect(valueContainerArg.length).toBe(2);

      expect(valueContainerArg[0]?.value).toBe(1);
      expect(valueContainerArg[0]?.name).toBe('One');
      expect(valueContainerArg[1]?.value).toBe(2);
      expect(valueContainerArg[1]?.name).toBe('Two');
    });

    await resultPromise;
    await untilCallbacksSettled(csp, () => thenCallbackCalled);

    expect(thenCallbackCalled).toBe(true);
  });

  // ----------
  // Nested container types

  it('Await nested container of pointers result', async () => {
    using helper = csp.CallbacksBindingMechanismsTestType.create(offThread);

    using result = await helper.callbackFunctionNestedContainerOfPointersAsync();

    expect(result.size).toBe(2);

    for (const [, array] of result.entries()) {
      expect(array).not.toBeFalsy();
      expect(array?.length).toBe(2);
    }
  });

  it('Nested container of pointers result with promise', async () => {
    using helper = csp.CallbacksBindingMechanismsTestType.create(offThread);

    let thenCallbackCalled = false;

    const resultPromise = helper.callbackFunctionNestedContainerOfPointersAsync().then((valueContainerArg) => {
      thenCallbackCalled = true;

      expect(valueContainerArg.size).toBe(2);

      const array1 = valueContainerArg.get(0);
      expect(array1).not.toBeFalsy();
      expect(array1?.length).toBe(2);
      expect(array1?.[0]?.value).toBe(1);
      expect(array1?.[0]?.name).toBe('One');
      expect(array1?.[1]?.value).toBe(2);
      expect(array1?.[1]?.name).toBe('Two');

      const array2 = valueContainerArg.get(1);
      expect(array2).not.toBeFalsy();
      expect(array2?.length).toBe(2);
      expect(array2?.[0]?.value).toBe(3);
      expect(array2?.[0]?.name).toBe('Three');
      expect(array2?.[1]?.value).toBe(4);
      expect(array2?.[1]?.name).toBe('Four');
    });

    await resultPromise;
    await untilCallbacksSettled(csp, () => thenCallbackCalled);

    expect(thenCallbackCalled).toBe(true);
  });

  it('Await nested container of values result', async () => {
    using helper = csp.CallbacksBindingMechanismsTestType.create(offThread);

    using result = await helper.callbackFunctionNestedContainerOfValuesAsync();

    expect(result.size).toBe(2);

    for (const [, array] of result.entries()) {
      expect(array).not.toBeFalsy();
      expect(array?.length).toBe(2);
    }
  });

  it('Nested container of values result with promise', async () => {
    using helper = csp.CallbacksBindingMechanismsTestType.create(offThread);

    let thenCallbackCalled = false;

    const resultPromise = helper.callbackFunctionNestedContainerOfValuesAsync().then((valueContainerArg) => {
      thenCallbackCalled = true;

      expect(valueContainerArg.size).toBe(2);

      const array1 = valueContainerArg.get(0);
      expect(array1).not.toBeFalsy();
      expect(array1?.length).toBe(2);
      expect(array1?.[0]?.value).toBe(1);
      expect(array1?.[0]?.name).toBe('One');
      expect(array1?.[1]?.value).toBe(2);
      expect(array1?.[1]?.name).toBe('Two');

      const array2 = valueContainerArg.get(1);
      expect(array2).not.toBeFalsy();
      expect(array2?.length).toBe(2);
      expect(array2?.[0]?.value).toBe(3);
      expect(array2?.[0]?.name).toBe('Three');
      expect(array2?.[1]?.value).toBe(4);
      expect(array2?.[1]?.name).toBe('Four');
    });

    await resultPromise;
    await untilCallbacksSettled(csp, () => thenCallbackCalled);

    expect(thenCallbackCalled).toBe(true);
  });

  it('Await nested container of values by const ref result', async () => {
    using helper = csp.CallbacksBindingMechanismsTestType.create(offThread);

    using result = await helper.callbackFunctionNestedContainerOfValuesByConstRefAsync();

    expect(result.size).toBe(2);

    for (const [, array] of result.entries()) {
      expect(array).not.toBeFalsy();
      expect(array?.length).toBe(2);
    }
  });

  it('Nested container of values by const ref result with promise', async () => {
    using helper = csp.CallbacksBindingMechanismsTestType.create(offThread);

    let thenCallbackCalled = false;

    const resultPromise = helper.callbackFunctionNestedContainerOfValuesByConstRefAsync().then((valueContainerArg) => {
      thenCallbackCalled = true;

      expect(valueContainerArg.size).toBe(2);

      const array1 = valueContainerArg.get(0);
      expect(array1).not.toBeFalsy();
      expect(array1?.length).toBe(2);
      expect(array1?.[0]?.value).toBe(1);
      expect(array1?.[0]?.name).toBe('One');
      expect(array1?.[1]?.value).toBe(2);
      expect(array1?.[1]?.name).toBe('Two');

      const array2 = valueContainerArg.get(1);
      expect(array2).not.toBeFalsy();
      expect(array2?.length).toBe(2);
      expect(array2?.[0]?.value).toBe(3);
      expect(array2?.[0]?.name).toBe('Three');
      expect(array2?.[1]?.value).toBe(4);
      expect(array2?.[1]?.name).toBe('Four');
    });

    await resultPromise;
    await untilCallbacksSettled(csp, () => thenCallbackCalled);

    expect(thenCallbackCalled).toBe(true);
  });

  // ----------
  // Optional types

  it('Await optional value type result', async () => {
    using helper = csp.CallbacksBindingMechanismsTestType.create(offThread);

    using result = await helper.callbackFunctionValueOptAsync();

    expect(result).toBeDefined();
    expect(result?.value).toBe(1);
    expect(result?.name).toBe('One');
  });

  it('Optional value type result with promise', async () => {
    using helper = csp.CallbacksBindingMechanismsTestType.create(offThread);

    let thenCallbackCalled = false;

    const resultPromise = helper.callbackFunctionValueOptAsync().then((result) => {
      thenCallbackCalled = true;

      expect(result).toBeDefined();
      expect(result?.value).toBe(1);
      expect(result?.name).toBe('One');
    });

    await resultPromise;
    await untilCallbacksSettled(csp, () => thenCallbackCalled);

    expect(thenCallbackCalled).toBe(true);
  });

  it('Await optional pointer type result', async () => {
    using helper = csp.CallbacksBindingMechanismsTestType.create(offThread);

    using result = await helper.callbackFunctionPointerOptAsync();

    expect(result).not.toBeFalsy();
    expect(result?.value).toBe(1);
    expect(result?.name).toBe('One');
  });

  it('Optional pointer type result with promise', async () => {
    using helper = csp.CallbacksBindingMechanismsTestType.create(offThread);

    let thenCallbackCalled = false;

    const resultPromise = helper.callbackFunctionPointerOptAsync().then((result) => {
      thenCallbackCalled = true;

      expect(result).not.toBeFalsy();
      expect(result?.value).toBe(1);
      expect(result?.name).toBe('One');
    });

    await resultPromise;
    await untilCallbacksSettled(csp, () => thenCallbackCalled);

    expect(thenCallbackCalled).toBe(true);
  });

  it('Null optional value type result with promise', async () => {
    using helper = csp.CallbacksBindingMechanismsTestType.create(offThread);

    let thenCallbackCalled = false;

    const resultPromise = helper.callbackFunctionNullValueOptAsync().then((result) => {
      thenCallbackCalled = true;

      expect(result).toBeUndefined();
    });

    await resultPromise;
    await untilCallbacksSettled(csp, () => thenCallbackCalled);

    expect(thenCallbackCalled).toBe(true);
  });

  it('Container of optional type result with promise', async () => {
    using helper = csp.CallbacksBindingMechanismsTestType.create(offThread);

    let thenCallbackCalled = false;

    const resultPromise = helper.callbackFunctionArrayOfOptAsync().then((result) => {
      thenCallbackCalled = true;

      expect(result.length).toBe(2);

      expect(result[0]?.value).toBe(1);
      expect(result[0]?.name).toBe('One');
      expect(result[1]?.value).toBe(2);
      expect(result[1]?.name).toBe('Two');
    });

    await resultPromise;
    await untilCallbacksSettled(csp, () => thenCallbackCalled);

    expect(thenCallbackCalled).toBe(true);
  });

  it('Null optional pointer type result with promise', async () => {
    using helper = csp.CallbacksBindingMechanismsTestType.create(offThread);

    let thenCallbackCalled = false;

    const resultPromise = helper.callbackFunctionNullPointerOptAsync().then((result) => {
      thenCallbackCalled = true;

      expect(result).toBeUndefined();
    });

    await resultPromise;
    await untilCallbacksSettled(csp, () => thenCallbackCalled);

    expect(thenCallbackCalled).toBe(true);
  });

  it('Await primitive result from multi primitive input arg function', async () => {
    using helper = csp.CallbacksBindingMechanismsTestType.create(offThread);

    const result = await helper.callbackFunctionMultiInputPrimitiveArgAsync(1, 2);

    expect(result).toBe(3);
  });

  // ----------
  // Pointer lifetime

  it('Await pointer result lifetime', async () => {
    using helper = csp.CallbacksBindingMechanismsTestType.create(offThread);

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
    using helper = csp.CallbacksBindingMechanismsTestType.create(offThread);

    const beforeCallbackCount = csp.BindingsTestType.aliveCount();

    {
      using result = await helper.callbackFunctionValueArgAsync();

      await untilCallbacksSettled(csp);

      expect(result.value).toBe(1);

      const afterCallbackCount = csp.BindingsTestType.aliveCount();
      expect(afterCallbackCount).toBe(beforeCallbackCount + 1);
    }

    expect(csp.BindingsTestType.aliveCount()).toBe(beforeCallbackCount);
  });

  it('Value type lifetime with promise', async () => {
    using helper = csp.CallbacksBindingMechanismsTestType.create(offThread);

    let thenCallbackCalled = false;
    const beforeCallbackCount = csp.BindingsTestType.aliveCount();

    const basePromise = helper.callbackFunctionValueArgAsync();

    // Deliberately left undisposed here so we can check the alive count once settled, below -
    // disposing inline here would race the off-thread teardown.
    const resultPromise = basePromise.then((result) => {
      thenCallbackCalled = true;

      expect(result.value).toBe(1);
    });

    // Await the promise - the then() callback will run first
    await resultPromise;
    await untilCallbacksSettled(csp, () => thenCallbackCalled);

    expect(csp.BindingsTestType.aliveCount()).toBe(beforeCallbackCount + 1);

    {
      // Re-await the base promise to get a disposable handle to the same underlying object
      // (see 'Value type lifetime, count does not increase with multiple awaits').
      using disposableResult = await basePromise;
    }

    expect(csp.BindingsTestType.aliveCount()).toBe(beforeCallbackCount);
  });

  it('Await const ref type lifetime', async () => {
    using helper = csp.CallbacksBindingMechanismsTestType.create(offThread);

    const beforeCallbackCount = csp.BindingsTestType.aliveCount();

    {
      using result = await helper.callbackFunctionValueArgByConstRefAsync();

      await untilCallbacksSettled(csp);

      expect(result.value).toBe(1);

      const afterCallbackCount = csp.BindingsTestType.aliveCount();
      expect(afterCallbackCount).toBe(beforeCallbackCount + 1);
    }

    expect(csp.BindingsTestType.aliveCount()).toBe(beforeCallbackCount);
  });

  it('Const ref type lifetime with promise', async () => {
    using helper = csp.CallbacksBindingMechanismsTestType.create(offThread);

    let thenCallbackCalled = false;
    const beforeCallbackCount = csp.BindingsTestType.aliveCount();

    const basePromise = helper.callbackFunctionValueArgByConstRefAsync();

    // Deliberately left undisposed here so we can check the alive count once settled, below -
    // disposing inline here would race the off-thread teardown.
    const resultPromise = basePromise.then((result) => {
      thenCallbackCalled = true;

      expect(result.value).toBe(1);
    });

    await resultPromise;
    await untilCallbacksSettled(csp, () => thenCallbackCalled);

    expect(csp.BindingsTestType.aliveCount()).toBe(beforeCallbackCount + 1);

    {
      // Re-await the base promise to get a disposable handle to the same underlying object
      // (see 'Value type lifetime, count does not increase with multiple awaits').
      using disposableResult = await basePromise;
    }

    expect(csp.BindingsTestType.aliveCount()).toBe(beforeCallbackCount);
  });

  it('Referencing an un-disposed value arg outside an async callback works', async () => {
    using helper = csp.CallbacksBindingMechanismsTestType.create(offThread);

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
  // Container lifetime

  it('Container of non-owning pointers lifetime with promise', async () => {
    using helper = csp.CallbacksBindingMechanismsTestType.create(offThread);

    let thenCallbackCalled = false;
    const beforeCallbackCount = csp.BindingsTestType.aliveCount();

    const resultPromise = helper.callbackFunctionContainerOfPointersAsync().then((pointerContainerArg) => {
      thenCallbackCalled = true;

      expect(pointerContainerArg.length).toBe(2);

      // Pointers should not incur any new lifetimes
      expect(csp.BindingsTestType.aliveCount()).toBe(beforeCallbackCount);
    });

    await resultPromise;
    await untilCallbacksSettled(csp, () => thenCallbackCalled);

    expect(thenCallbackCalled).toBe(true);
    expect(csp.BindingsTestType.aliveCount()).toBe(beforeCallbackCount);
  });

  it('Container of values by const ref lifetime with promise', async () => {
    using helper = csp.CallbacksBindingMechanismsTestType.create(offThread);

    let thenCallbackCalled = false;
    const beforeCallbackCount = csp.BindingsTestType.aliveCount();

    const basePromise = helper.callbackFunctionContainerOfValuesByConstRefAsync();

    // Deliberately left undisposed here so we can check the alive count once settled, below -
    // disposing inline here would race the off-thread teardown.
    const resultPromise = basePromise.then((valueContainerArg) => {
      thenCallbackCalled = true;

      expect(valueContainerArg.length).toBe(2);
    });

    await resultPromise;
    await untilCallbacksSettled(csp, () => thenCallbackCalled);

    expect(thenCallbackCalled).toBe(true);
    expect(csp.BindingsTestType.aliveCount()).toBe(beforeCallbackCount + 2);

    {
      // Re-await the base promise to get a disposable handle to the same underlying container
      // (see 'Value type lifetime, count does not increase with multiple awaits').
      using disposableValueContainerArg = await basePromise;
    }

    expect(csp.BindingsTestType.aliveCount()).toBe(beforeCallbackCount);
  });

  it('Container of values lifetime with promise', async () => {
    using helper = csp.CallbacksBindingMechanismsTestType.create(offThread);

    let thenCallbackCalled = false;
    const beforeCallbackCount = csp.BindingsTestType.aliveCount();

    const basePromise = helper.callbackFunctionContainerOfValuesAsync();

    // Deliberately left undisposed here so we can check the alive count once settled, below -
    // disposing inline here would race the off-thread teardown.
    const resultPromise = basePromise.then((valueContainerArg) => {
      thenCallbackCalled = true;

      expect(valueContainerArg.length).toBe(2);
    });

    await resultPromise;
    await untilCallbacksSettled(csp, () => thenCallbackCalled);

    expect(thenCallbackCalled).toBe(true);
    expect(csp.BindingsTestType.aliveCount()).toBe(beforeCallbackCount + 2);

    {
      // Re-await the base promise to get a disposable handle to the same underlying container
      // (see 'Value type lifetime, count does not increase with multiple awaits').
      using disposableValueContainerArg = await basePromise;
    }

    expect(csp.BindingsTestType.aliveCount()).toBe(beforeCallbackCount);
  });

  // ----------
  // Nested container lifetime

  it('Await nested container of values by lifetime', async () => {
    using helper = csp.CallbacksBindingMechanismsTestType.create(offThread);

    const beforeCallbackCount = csp.BindingsTestType.aliveCount();

    {
      using result = await helper.callbackFunctionNestedContainerOfValuesAsync();

      await untilCallbacksSettled(csp);

      expect(result.size).toBe(2);

      expect(csp.BindingsTestType.aliveCount()).toBe(beforeCallbackCount + 4);
    }

    expect(csp.BindingsTestType.aliveCount()).toBe(beforeCallbackCount);
  });

  // ----------
  // Optional lifetime

  it('Await optional value type result lifetime', async () => {
    using helper = csp.CallbacksBindingMechanismsTestType.create(offThread);

    const beforeCallbackCount = csp.BindingsTestType.aliveCount();

    {
      using result = await helper.callbackFunctionValueOptAsync();

      await untilCallbacksSettled(csp);

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
    using helper = csp.CallbacksBindingMechanismsTestType.create(offThread);

    const beforeCallbackCount = csp.BindingsTestType.aliveCount();

    const resultPromise = helper.callbackFunctionValueArgAsync();

    {
      using result1 = await resultPromise;
      // Needs to be const here to avoid double dispose, only one 'using' should be used on the same
      // promise result.
      const result2 = await resultPromise;

      await untilCallbacksSettled(csp);

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
    using helper = csp.CallbacksBindingMechanismsTestType.create(offThread);

    const beforeCallbackCount = csp.BindingsTestType.aliveCount();

    const resultPromise = helper.callbackFunctionValueArgAsync();

    // Note no 'using' here to not dispose the result
    const result1 = await resultPromise;

    await untilCallbacksSettled(csp);

    expect(csp.BindingsTestType.aliveCount()).toBe(beforeCallbackCount + 1);

    // Note no 'using' here
    const result2 = await resultPromise;

    // We're accessing the same underlying object again so the alive count will not have increased
    expect(csp.BindingsTestType.aliveCount()).toBe(beforeCallbackCount + 1);

    result1.delete();
  });

  it('Value type lifetime, disposing after first use means value is deleted for subsequent awaits', async () => {
    using helper = csp.CallbacksBindingMechanismsTestType.create(offThread);

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
    using helper = csp.CallbacksBindingMechanismsTestType.create(offThread);

    const beforeCallbackCount = csp.BindingsTestType.aliveCount();

    const resultPromise = helper.callbackFunctionValueArgAsync();

    let firstThenCalled = false;
    let secondThenCalled = false;

    resultPromise.then((result) => {
      firstThenCalled = true;

      expect(result.value).toBe(1);
      expect(result.name).toBe('One');
    });

    resultPromise.then((result) => {
      // We've not disposed the result in the first then() callback so we can still access it here
      secondThenCalled = true;

      expect(result.value).toBe(1);
      expect(result.name).toBe('One');
    });

    // Wait for both thens to fire, and for the off-thread invocation to fully settle, before
    // inspecting the alive count - otherwise this races against the off-thread teardown.
    await untilCallbacksSettled(csp, () => firstThenCalled && secondThenCalled);

    // It's the same underlying object so the alive count will not have increased, and no
    // disposal has happened so the count will not have reduced either.
    expect(csp.BindingsTestType.aliveCount()).toBe(beforeCallbackCount + 1);

    // Do this to dispose the result
    using result = await resultPromise;
  });
});
