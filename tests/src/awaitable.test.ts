import { describe, it, expect, beforeAll } from 'vitest';
import { loadCSP } from '../loadModule';
import { type MainModule } from 'connected-spaces-platform-bindings';

/*
 * Tests for the binding of awaitable callbacks into the typescript boundary.
 */

describe('Awaitable Callbacks', () => {
  let csp: MainModule;

  beforeAll(async () => {
    csp = await loadCSP();
  });

  // Basic

  it('Await primitive result', async () => {
    const helper = csp.AwaitableCallbacksBindingMechanismsTestType.create();

    const result = await helper.awaitableCallbackFunctionPrimitiveArg();

    expect(result).toBe(10);
  });

  it('Primitive result with promise', async () => {
    const helper = csp.AwaitableCallbacksBindingMechanismsTestType.create();

    helper.awaitableCallbackFunctionPrimitiveArg().then((value) => {
      expect(value).toBe(10);
    });
  });

  it('Await value type result', async () => {
    const helper = csp.AwaitableCallbacksBindingMechanismsTestType.create();

    using result = await helper.awaitableCallbackFunctionValueArg();

    expect(result.value).toBe(1);
    expect(result.name).toBe('One');
  });

  it('Value type result with promise', async () => {
    const helper = csp.AwaitableCallbacksBindingMechanismsTestType.create();

    helper.awaitableCallbackFunctionValueArg().then((result) => {
      expect(result.value).toBe(1);
      expect(result.name).toBe('One');
    });
  });

  it('Await const ref result', async () => {
    const helper = csp.AwaitableCallbacksBindingMechanismsTestType.create();

    using result = await helper.awaitableCallbackFunctionConstRefArg();

    expect(result.value).toBe(1);
    expect(result.name).toBe('One');
  });

  it('Const ref result with promise', async () => {
    const helper = csp.AwaitableCallbacksBindingMechanismsTestType.create();

    helper.awaitableCallbackFunctionConstRefArg().then((result) => {
      expect(result.value).toBe(1);
      expect(result.name).toBe('One');
    });
  });

  it('Await pointer result', async () => {
    const helper = csp.AwaitableCallbacksBindingMechanismsTestType.create();

    // Pointer result is non-owning: no using
    const result = await helper.awaitableCallbackFunctionPointerArg();

    expect(result.value).toBe(1);
    expect(result.name).toBe('One');
  });

  it('Pointer result with promise', async () => {
    const helper = csp.AwaitableCallbacksBindingMechanismsTestType.create();

    helper.awaitableCallbackFunctionPointerArg().then((result) => {
      expect(result.value).toBe(1);
      expect(result.name).toBe('One');
    });
  });

  // ----------
  // TEST ONLY - Try with disposable promises

  it('TEST ONLY - Returned promise is disposable', async () => {
    const helper = csp.AwaitableCallbacksBindingMechanismsTestType.create();

    {
      // @ts-expect-error Disposable is not added to type declaration
      using resultPromise = helper.awaitableCallbackFunctionValueArgDisposablePromise();

      // @ts-expect-error Disposable is not added to type declaration
      expect(resultPromise[Symbol.dispose]).toBeDefined();
    }
  });

  it('TEST ONLY - Returned promise is disposed at end of scope', async () => {
    const helper = csp.AwaitableCallbacksBindingMechanismsTestType.create();

    {
      // @ts-expect-error Disposable is not added to type declaration
      using resultPromise = helper.awaitableCallbackFunctionValueArgDisposablePromise();

      resultPromise.then((result) => {
        expect(result.value).toBe(1);
        expect(result.name).toBe('One');
      });

      // The then function can itself be async
      resultPromise.then(async (result) => {
        await Promise.resolve();

        // Result has been deleted by the promise disposal by this point
        expect(result.isDeleted()).toBe(true);

        // Accessing the object will thow
        expect(() => result.value).toThrow();
        expect(() => result.name).toThrow();
      });
    }

    await Promise.resolve();
  });

  // ----------
  // Pointer lifetime

  it('Pointer result is non-owning', async () => {
    const helper = csp.AwaitableCallbacksBindingMechanismsTestType.create();

    const beforeCallbackCount = csp.BindingsTestType.aliveCount;

    // Pointer result is non-owning: no using
    const result = await helper.awaitableCallbackFunctionPointerArg();

    expect(result.value).toBe(1);
    expect(result.name).toBe('One');

    // Pointer is non-owning: no copy made, no aliveCount change, no disposal needed
    expect(csp.BindingsTestType.aliveCount).toBe(beforeCallbackCount);

    expect(() => result.delete()).toThrow();
    expect(() => result[Symbol.dispose]()).toThrow();
  });

  // ----------
  // TEST ONLY (skipped) - Tests for callback argument lifetime with auto disposal

  describe.skip('Awaitable Callbacks - Auto disposal', () => {
    it('Await value type lifetime, no disposal needed', async () => {
      const helper = csp.AwaitableCallbacksBindingMechanismsTestType.create();

      const beforeCallbackCount = csp.BindingsTestType.aliveCount;

      // No `using` here: the result is disposed automatically, same as a regular callback argument.
      const result = await helper.awaitableCallbackFunctionValueArg();

      expect(result.value).toBe(1);

      const afterCallbackCount = csp.BindingsTestType.aliveCount;
      expect(afterCallbackCount).toBe(beforeCallbackCount + 1);

      // Disposal happens right after this synchronous continuation finishes, so yield once to observe it.
      await Promise.resolve();

      expect(csp.BindingsTestType.aliveCount).toBe(beforeCallbackCount);
    });

    it('Value type lifetime with promise, no disposal needed', async () => {
      const helper = csp.AwaitableCallbacksBindingMechanismsTestType.create();

      const beforeCallbackCount = csp.BindingsTestType.aliveCount;

      const resultPromise = helper.awaitableCallbackFunctionValueArg().then((result) => {
        expect(result.value).toBe(1);

        const afterCallbackCount = csp.BindingsTestType.aliveCount;
        expect(afterCallbackCount).toBe(beforeCallbackCount + 1);
      });

      // Await the promise to ensure the then() callback has run.
      // The result will be disposed automatically.
      await resultPromise;

      expect(csp.BindingsTestType.aliveCount).toBe(beforeCallbackCount);
    });

    it('Await const ref type lifetime, no disposal needed', async () => {
      const helper = csp.AwaitableCallbacksBindingMechanismsTestType.create();

      const beforeCallbackCount = csp.BindingsTestType.aliveCount;

      const result = await helper.awaitableCallbackFunctionConstRefArg();

      expect(result.value).toBe(1);

      const afterCallbackCount = csp.BindingsTestType.aliveCount;
      expect(afterCallbackCount).toBe(beforeCallbackCount + 1);

      await Promise.resolve();

      expect(csp.BindingsTestType.aliveCount).toBe(beforeCallbackCount);
    });

    it('Const ref type lifetime with promise, no disposal needed', async () => {
      const helper = csp.AwaitableCallbacksBindingMechanismsTestType.create();

      const beforeCallbackCount = csp.BindingsTestType.aliveCount;

      const resultPromise = helper.awaitableCallbackFunctionConstRefArg().then((result) => {
        expect(result.value).toBe(1);

        const afterCallbackCount = csp.BindingsTestType.aliveCount;
        expect(afterCallbackCount).toBe(beforeCallbackCount + 1);
      });

      await resultPromise;

      expect(csp.BindingsTestType.aliveCount).toBe(beforeCallbackCount);
    });
  });

  // ----------
  // Tests for callback argument lifetime without auto disposal

  describe('Awaitable Callbacks - No auto disposal', () => {
    it('Await value type lifetime', async () => {
      const helper = csp.AwaitableCallbacksBindingMechanismsTestType.create();

      const beforeCallbackCount = csp.BindingsTestType.aliveCount;

      {
        using result = await helper.awaitableCallbackFunctionValueArg();

        expect(result.value).toBe(1);

        const afterCallbackCount = csp.BindingsTestType.aliveCount;
        expect(afterCallbackCount).toBe(beforeCallbackCount + 1);
      }

      expect(csp.BindingsTestType.aliveCount).toBe(beforeCallbackCount);
    });

    it('Value type lifetime with promise', async () => {
      const helper = csp.AwaitableCallbacksBindingMechanismsTestType.create();

      const beforeCallbackCount = csp.BindingsTestType.aliveCount;

      const resultPromise = helper.awaitableCallbackFunctionValueArg().then((result) => {
        // Can also use a DisposableStack instead (disposableStack.use(result))
        using disposableResult = result;

        expect(result.value).toBe(1);

        const afterCallbackCount = csp.BindingsTestType.aliveCount;
        expect(afterCallbackCount).toBe(beforeCallbackCount + 1);
      });

      // Await the promise - the then() callback will run first
      await resultPromise;

      expect(csp.BindingsTestType.aliveCount).toBe(beforeCallbackCount);
    });

    it('Await const ref type lifetime', async () => {
      const helper = csp.AwaitableCallbacksBindingMechanismsTestType.create();

      const beforeCallbackCount = csp.BindingsTestType.aliveCount;

      {
        using result = await helper.awaitableCallbackFunctionConstRefArg();

        expect(result.value).toBe(1);

        const afterCallbackCount = csp.BindingsTestType.aliveCount;
        expect(afterCallbackCount).toBe(beforeCallbackCount + 1);
      }

      expect(csp.BindingsTestType.aliveCount).toBe(beforeCallbackCount);
    });

    it('Const ref type lifetime with promise', async () => {
      const helper = csp.AwaitableCallbacksBindingMechanismsTestType.create();

      const beforeCallbackCount = csp.BindingsTestType.aliveCount;

      const resultPromise = helper.awaitableCallbackFunctionConstRefArg().then((result) => {
        using disposableResult = result;

        expect(result.value).toBe(1);

        const afterCallbackCount = csp.BindingsTestType.aliveCount;
        expect(afterCallbackCount).toBe(beforeCallbackCount + 1);
      });

      await resultPromise;

      expect(csp.BindingsTestType.aliveCount).toBe(beforeCallbackCount);
    });
  });

  // ----------

  it('Value type lifetime, multiple await', async () => {
    const helper = csp.AwaitableCallbacksBindingMechanismsTestType.create();

    const beforeCallbackCount = csp.BindingsTestType.aliveCount;

    const resultPromise = helper.awaitableCallbackFunctionValueArg();

    {
      using result1 = await resultPromise;
      // Needs to be const here to avoid double dispose
      const result2 = await resultPromise;

      expect(result1.value).toBe(1);
      expect(result1.name).toBe('One');

      const afterCallbackCount = csp.BindingsTestType.aliveCount;
      expect(afterCallbackCount).toBe(beforeCallbackCount + 1);
    }

    // This will throw - underlying value has already been deleted here
    await expect(async () => {
      using result3 = await resultPromise;
    }).rejects.toThrow();

    expect(csp.BindingsTestType.aliveCount).toBe(beforeCallbackCount);
  });

  it('Value type lifetime, count does not increase with multiple awaits', async () => {
    const helper = csp.AwaitableCallbacksBindingMechanismsTestType.create();

    const beforeCallbackCount = csp.BindingsTestType.aliveCount;

    const resultPromise = helper.awaitableCallbackFunctionValueArg();

    // Note no 'using' here to not dispose the result
    const result1 = await resultPromise;

    expect(csp.BindingsTestType.aliveCount).toBe(beforeCallbackCount + 1);

    const result2 = await resultPromise;

    // We're accessing the same underlying object again so the alive count will not have increased
    expect(csp.BindingsTestType.aliveCount).toBe(beforeCallbackCount + 1);
  });

  it('Value type lifetime, disposing after first use means value is deleted for subsequent awaits', async () => {
    const helper = csp.AwaitableCallbacksBindingMechanismsTestType.create();

    const resultPromise = helper.awaitableCallbackFunctionValueArg();

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
    const helper = csp.AwaitableCallbacksBindingMechanismsTestType.create();

    const beforeCallbackCount = csp.BindingsTestType.aliveCount;

    const resultPromise = helper.awaitableCallbackFunctionValueArg();

    resultPromise.then((result) => {
      expect(result.value).toBe(1);
      expect(result.name).toBe('One');

      expect(csp.BindingsTestType.aliveCount).toBe(beforeCallbackCount + 1);
    });

    resultPromise.then((result) => {
      // We've not disposed the result in the first then() callback so we can still access it here

      expect(result.value).toBe(1);
      expect(result.name).toBe('One');

      // It's the same underlying object so the alive count will not have increased
      expect(csp.BindingsTestType.aliveCount).toBe(beforeCallbackCount + 1);
    });

    await Promise.resolve();

    // No disposal happened so the count will not have reduced
    expect(csp.BindingsTestType.aliveCount).toBe(beforeCallbackCount + 1);
  });

  // ----------
  // Tests to show that auto-disposal using micro/macro-tasks is not workable

  describe.skip('TEST ONLY - Test auto disposal using micro/macro-tasks', () => {
    it('TEST ONLY - Const ref type lifetime with promise, then after promise resolve', async () => {
      const helper = csp.AwaitableCallbacksBindingMechanismsTestType.create();

      const beforeCallbackCount = csp.BindingsTestType.aliveCount;

      const resultPromise = helper.awaitableCallbackFunctionConstRefArg();

      await Promise.resolve();

      resultPromise.then((result) => {
        expect(result.value).toBe(1);
        expect(result.name).toBe('One');

        const afterCallbackCount = csp.BindingsTestType.aliveCount;
        expect(afterCallbackCount).toBe(beforeCallbackCount + 1);
      });

      expect(csp.BindingsTestType.aliveCount).toBe(beforeCallbackCount);
    });

    it('TEST ONLY - Const ref type lifetime with promise, then after timeout', async () => {
      const helper = csp.AwaitableCallbacksBindingMechanismsTestType.create();

      const beforeCallbackCount = csp.BindingsTestType.aliveCount;

      const resultPromise = helper.awaitableCallbackFunctionConstRefArg();

      setTimeout(() => {
        resultPromise.then((result) => {
          expect(result.value).toBe(1);
          expect(result.name).toBe('One');

          const afterCallbackCount = csp.BindingsTestType.aliveCount;
          expect(afterCallbackCount).toBe(beforeCallbackCount + 1);
        });
      }, 0);
    });
  });
});
