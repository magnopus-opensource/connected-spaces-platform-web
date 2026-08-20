import { describe, it, expect, beforeAll } from 'vitest';
import { loadCSP } from '../loadModule';
import { until, untilCallbacksSettled, forceHeapGrowth } from './testUtils';
import createModule, { type MainModule } from 'connected-spaces-platform-bindings';

/*
 * Tests for the binding of callbacks into the typescript boundary.
 * Note that this is not async/await, although that feature is build atop this feature
 * This is "Long running callbacks" as I've tended to call them. Callbacks that
 * are defined in TS, and may even be triggered more than once.
 *
 * There's an argument that we should be using mock functions to test here (ie vi.fn())
 * but I am a bit unsure. There are unusual memory management implications due to
 * bridging to c++, and any "magic" might get in the way of understanding that.
 *
 * Every test runs twice, once on-thread, once off-thread. That's why we're doing
 * `await untilCallbacksSettled` everywhere.
 * There is an internal latch that keeps track of how many off-thread callbacks are in flight, so doing this keeps
 * us synchronized.
 *
 * Note that if your test does not interact with lifetimes, in that is does not create ANY csp objects,
 * (this effectively means that it only uses zero-arg or primitive callbacks), then you don't need to use
 * `untilCallbacksSettled`, a regular `await` is fine. `untilCallbacksSettled` is only a thing that matters
 * for lifetime inspection of dying objects, which isn't a normal thing to be doing outside of tests.
 */

/* "mode" is just used to print the test name nicely. */
const CALLBACK_THREADING_MODE = [
  { mode: 'On Thread', offThread: false },
  { mode: 'Off Thread', offThread: true }
];

for (const { mode, offThread } of CALLBACK_THREADING_MODE) {
  describe(`Callbacks (${mode})`, () => {
    let csp: MainModule;

    beforeAll(async () => {
      csp = await loadCSP();
    });

    it('Callback No Args', async () => {
      using helper = csp.CallbacksBindingMechanismsTestType.create(offThread);

      let callbackCalled = false;
      helper.callbackFunctionNoArgs(() => {
        callbackCalled = true;
      });

      await untilCallbacksSettled(csp, () => callbackCalled);
      expect(callbackCalled).toBe(true);
    });

    it('Callback Primitive Arg', async () => {
      using helper = csp.CallbacksBindingMechanismsTestType.create(offThread);

      let callbackCalled = false;
      helper.callbackFunctionPrimitiveArg((primitiveArg) => {
        callbackCalled = true;
        expect(primitiveArg).toBe(10);
      });

      await untilCallbacksSettled(csp, () => callbackCalled);
      expect(callbackCalled).toBe(true);
    });

    it('Callback Pointer Arg', async () => {
      using helper = csp.CallbacksBindingMechanismsTestType.create(offThread);

      let callbackCalled = false;
      helper.callbackFunctionPointerArg((pointerArg) => {
        callbackCalled = true;
        expect(pointerArg.value).toBe(1);
        expect(pointerArg.name).toBe('One');
      });

      await untilCallbacksSettled(csp, () => callbackCalled);
      expect(callbackCalled).toBe(true);
    });

    it('Callback Value Arg', async () => {
      using helper = csp.CallbacksBindingMechanismsTestType.create(offThread);

      let callbackCalled = false;
      helper.callbackFunctionValueArg((valueArg) => {
        callbackCalled = true;
        expect(valueArg.value).toBe(1);
        expect(valueArg.name).toBe('One');
      });

      await untilCallbacksSettled(csp, () => callbackCalled);
      expect(callbackCalled).toBe(true);
    });

    it('Callback Container of Pointers Arg', async () => {
      using helper = csp.CallbacksBindingMechanismsTestType.create(offThread);

      let callbackCalled = false;
      helper.callbackFunctionContainerOfPointers((pointerContainerArg) => {
        callbackCalled = true;
        expect(pointerContainerArg.length).toBe(2);
        expect(pointerContainerArg[0]?.value).toBe(1);
        expect(pointerContainerArg[0]?.name).toBe('One');
        expect(pointerContainerArg[1]?.value).toBe(2);
        expect(pointerContainerArg[1]?.name).toBe('Two');
      });

      await untilCallbacksSettled(csp, () => callbackCalled);
      expect(callbackCalled).toBe(true);
    });

    it('Callback Container of Values Arg', async () => {
      using helper = csp.CallbacksBindingMechanismsTestType.create(offThread);

      let callbackCalled = false;
      helper.callbackFunctionContainerOfValues((valueContainerArg) => {
        callbackCalled = true;
        expect(valueContainerArg.length).toBe(2);
        expect(valueContainerArg[0]?.value).toBe(1);
        expect(valueContainerArg[0]?.name).toBe('One');
        expect(valueContainerArg[1]?.value).toBe(2);
        expect(valueContainerArg[1]?.name).toBe('Two');
      });

      await untilCallbacksSettled(csp, () => callbackCalled);
      expect(callbackCalled).toBe(true);
    });

    it('Callback Multiple Args', async () => {
      using helper = csp.CallbacksBindingMechanismsTestType.create(offThread);

      let callbackCalled = false;
      helper.callbackFunctionMultipleArgs((primitiveArg1, primitiveArg2) => {
        callbackCalled = true;
        expect(primitiveArg1).toBe(1);
        expect(primitiveArg2).toBe(2);
      });

      await untilCallbacksSettled(csp, () => callbackCalled);
      expect(callbackCalled).toBe(true);
    });

    it.skipIf(offThread)('Callback That Throws On Thread', () => {
      using helper = csp.CallbacksBindingMechanismsTestType.create(offThread);

      // Synchronous callbacks should behave just like nested calls */
      let callbackCalled = false;
      expect(() => {
        helper.callbackFunctionNoArgs(() => {
          callbackCalled = true;
          throw new Error('test error');
        });
      }).toThrow('test error');

      expect(callbackCalled).toBe(true);

      // The module must remain usable after an exception has unwound through it.
      let secondCallbackCalled = false;
      helper.callbackFunctionNoArgs(() => {
        secondCallbackCalled = true;
      });
      expect(secondCallbackCalled).toBe(true);
    });

    it.skipIf(!offThread)('Callback That Throws Off Thread', async () => {
      using helper = csp.CallbacksBindingMechanismsTestType.create(offThread);

      // Off thread callbacks do not throw, they convert to errors
      let callbackCalled = false;

      const errorSpy = vi.spyOn(console, 'error').mockImplementation(() => {});

      helper.callbackFunctionNoArgs(() => {
        callbackCalled = true;
        throw new Error('test error');
      });

      await until(() => callbackCalled);
      expect(callbackCalled).toBe(true);

      expect(errorSpy).toHaveBeenCalledWith(
        'Unhandled exception in off-thread callback:',
        expect.objectContaining({ message: 'test error' })
      );

      // The module must remain usable after an exception has happened.
      let secondCallbackCalled = false;
      helper.callbackFunctionNoArgs(() => {
        secondCallbackCalled = true;
      });
      await until(() => secondCallbackCalled);
      expect(secondCallbackCalled).toBe(true);
    });

    it('Pointer arg is non-owning', async () => {
      using helper = csp.CallbacksBindingMechanismsTestType.create(offThread);

      let callbackCalled = false;
      helper.callbackFunctionPointerArg((pointerArg) => {
        callbackCalled = true;
        expect(() => pointerArg.delete()).toThrow();
        expect(() => pointerArg.deleteLater()).toThrow();
        expect(() => pointerArg[Symbol.dispose]()).toThrow();
      });

      await untilCallbacksSettled(csp, () => callbackCalled);
      expect(callbackCalled).toBe(true);
    });

    it('Callback Optional Value Arg', async () => {
      using helper = csp.CallbacksBindingMechanismsTestType.create(offThread);

      let callbackCalled = false;
      helper.callbackFunctionValueOpt((valueArg) => {
        callbackCalled = true;
        expect(valueArg?.value).toBe(1);
        expect(valueArg?.name).toBe('One');
      });

      await untilCallbacksSettled(csp, () => callbackCalled);
      expect(callbackCalled).toBe(true);
    });

    it('Callback Optional Pointer Arg', async () => {
      using helper = csp.CallbacksBindingMechanismsTestType.create(offThread);

      let callbackCalled = false;
      helper.callbackFunctionPointerOpt((pointerArg) => {
        callbackCalled = true;
        expect(pointerArg?.value).toBe(1);
        expect(pointerArg?.name).toBe('One');
      });

      await untilCallbacksSettled(csp, () => callbackCalled);
      expect(callbackCalled).toBe(true);
    });

    it('Callback Array of Optional Arg', async () => {
      using helper = csp.CallbacksBindingMechanismsTestType.create(offThread);

      let callbackCalled = false;
      helper.callbackFunctionArrayOfOpt((arrayOfOptArg) => {
        callbackCalled = true;
        expect(arrayOfOptArg.length).toBe(2);
        expect(arrayOfOptArg[0]?.value).toBe(1);
        expect(arrayOfOptArg[0]?.name).toBe('One');
        expect(arrayOfOptArg[1]?.value).toBe(2);
        expect(arrayOfOptArg[1]?.name).toBe('Two');
      });

      await untilCallbacksSettled(csp, () => callbackCalled);
      expect(callbackCalled).toBe(true);
    });

    it('Callback Optional of Array Arg', async () => {
      using helper = csp.CallbacksBindingMechanismsTestType.create(offThread);

      let callbackCalled = false;
      helper.callbackFunctionOptOfArray((optOfArrayArg) => {
        callbackCalled = true;
        expect(optOfArrayArg?.length).toBe(2);

        expect(optOfArrayArg?.[0]?.value).toBe(1);
        expect(optOfArrayArg?.[0]?.name).toBe('One');
        expect(optOfArrayArg?.[1]?.value).toBe(2);
        expect(optOfArrayArg?.[1]?.name).toBe('Two');
      });

      await untilCallbacksSettled(csp, () => callbackCalled);
      expect(callbackCalled).toBe(true);
    });

    it('Callback Array of Some Null Optional Arg', async () => {
      using helper = csp.CallbacksBindingMechanismsTestType.create(offThread);

      let callbackCalled = false;
      helper.callbackFunctionArrayOfSomeNullOpt((arrayOfSomeNullOptArg) => {
        callbackCalled = true;
        expect(arrayOfSomeNullOptArg?.length).toBe(2);

        expect(arrayOfSomeNullOptArg?.[0]).toBeUndefined();
        expect(arrayOfSomeNullOptArg?.[1]?.value).toBe(2);
        expect(arrayOfSomeNullOptArg?.[1]?.name).toBe('Two');
      });

      await untilCallbacksSettled(csp, () => callbackCalled);
      expect(callbackCalled).toBe(true);
    });

    it('Callback Optional Null Value Arg', async () => {
      using helper = csp.CallbacksBindingMechanismsTestType.create(offThread);

      let callbackCalled = false;
      helper.callbackFunctionNullValueOpt((nullValueOptArg) => {
        callbackCalled = true;
        expect(nullValueOptArg).toBeUndefined();
      });

      await untilCallbacksSettled(csp, () => callbackCalled);
      expect(callbackCalled).toBe(true);
    });

    it('Callback Optional Null Pointer Arg', async () => {
      using helper = csp.CallbacksBindingMechanismsTestType.create(offThread);

      let callbackCalled = false;
      helper.callbackFunctionNullPointerOpt((nullPointerOptArg) => {
        callbackCalled = true;
        expect(nullPointerOptArg).toBeUndefined();
      });

      await untilCallbacksSettled(csp, () => callbackCalled);
      expect(callbackCalled).toBe(true);
    });

    it('Callback called multiple times', async () => {
      using helper = csp.CallbacksBindingMechanismsTestType.create(offThread);

      let callbackCalledCount = 0;
      helper.setStoredCallbackNoArgs(() => {
        callbackCalledCount++;
      });

      helper.invokeStoredCallbackNoArgs();
      helper.invokeStoredCallbackNoArgs();
      helper.invokeStoredCallbackNoArgs();

      await until(() => callbackCalledCount === 3);
      expect(callbackCalledCount).toBe(3);
    });

    it('Re-register callback', async () => {
      using helper = csp.CallbacksBindingMechanismsTestType.create(offThread);

      let callbackCalledOne = false;
      helper.setStoredCallbackNoArgs(() => {
        callbackCalledOne = true;
      });

      helper.invokeStoredCallbackNoArgs();

      let callbackCalledTwo = false;
      helper.setStoredCallbackNoArgs(() => {
        callbackCalledTwo = true;
      });

      helper.invokeStoredCallbackNoArgs();

      await until(() => callbackCalledOne);
      await until(() => callbackCalledTwo);
      expect(callbackCalledOne).toBe(true);
      expect(callbackCalledTwo).toBe(true);
    });

    it('Callback registration does not leak', async () => {
      const freshCsp = await createModule();
      forceHeapGrowth(freshCsp);

      using helper = freshCsp.CallbacksBindingMechanismsTestType.create(offThread);

      const heapBefore = (freshCsp as unknown as { HEAPU8: Uint8Array }).HEAPU8.byteLength;
      let callbackInvocationCount = 0;
      // Spawning + joining 50k threads would probably take literal minutes.
      const targetIterations = offThread ? 250 : 50000;

      for (let i = 0; i < targetIterations; ++i) {
        // Handles are probably only 8 bytes ... it's difficult to imagine even this many iterations causing any heap growth we could detect.
        // I did run this specific registration with _a lot_ more iterations manually, overriding the test timeout, and there was no heap growth.
        helper.setStoredCallbackWithArgs((valueContainer) => {
          callbackInvocationCount++;
        });

        using one = freshCsp.BindingsTestType.create(1, 'One');
        using two = freshCsp.BindingsTestType.create(2, 'Two');
        using three = freshCsp.BindingsTestType.create(3, 'Three');
        using four = freshCsp.BindingsTestType.create(4, 'Four');
        using five = freshCsp.BindingsTestType.create(5, 'Five');

        // Over all the iterations, this would allocate about 17MB if it was leaking.
        helper.invokeStoredCallbackWithArgs([one, two, three, four, five]);
        await until(() => callbackInvocationCount === i + 1);
      }

      const heapAfter = (freshCsp as unknown as { HEAPU8: Uint8Array }).HEAPU8.byteLength;
      expect(heapAfter).toBe(heapBefore);
      expect(callbackInvocationCount).toBe(targetIterations);
    });

    /*
     * Tests callback argument lifetime specificities.
     * The theory here is that all owning arguments (read: value types), are COPIED into the JS runtime.
     * These copies are scoped to live ONLY for the scope of the callback, and will be automatically
     * disposed. The idea being that we are trading off leak-protection + unlikelihood of crashes for
     * some annoyance in how you must copy variables out of callback scope, rather than naturally referencing them,
     * if you wish to use them external to the callback scope.
     *
     * The lifetime value delta represents the amount of additional copies the binding layer itself adds
     * to the objects. The backend CSP interop container types can add a fair amount of redundant copies
     * themselves (default constructions), but we separate that in the test infra so it's not a concern here.
     */
    it('Callback by const ref arg disposes at end of scope automatically', async () => {
      using helper = csp.CallbacksBindingMechanismsTestType.create(offThread);

      let callbackCalled = false;
      const beforeCallbackCount = csp.BindingsTestType.aliveCount();

      helper.callbackFunctionValueArgByConstRef((byRefArg) => {
        callbackCalled = true;
        expect(csp.BindingsTestType.aliveCount()).toBe(beforeCallbackCount + 1);
      });

      await untilCallbacksSettled(csp, () => callbackCalled);
      expect(callbackCalled).toBe(true);
      expect(csp.BindingsTestType.aliveCount()).toBe(beforeCallbackCount);
    });

    it('Callback value arg disposes at end of scope automatically', async () => {
      using helper = csp.CallbacksBindingMechanismsTestType.create(offThread);

      let callbackCalled = false;
      const beforeCallbackCount = csp.BindingsTestType.aliveCount();

      helper.callbackFunctionValueArg((valueArg) => {
        callbackCalled = true;
        expect(csp.BindingsTestType.aliveCount()).toBe(beforeCallbackCount + 2);
      });

      await untilCallbacksSettled(csp, () => callbackCalled);
      expect(callbackCalled).toBe(true);
      expect(csp.BindingsTestType.aliveCount()).toBe(beforeCallbackCount);
    });

    it('Callback non-owning pointer arg does nothing at end of scope automatically', async () => {
      using helper = csp.CallbacksBindingMechanismsTestType.create(offThread);

      let callbackCalled = false;
      const beforeCallbackCount = csp.BindingsTestType.aliveCount();

      helper.callbackFunctionPointerArg((pointerArg) => {
        callbackCalled = true;
        // Pointers should not incur additional lifetimes
        expect(csp.BindingsTestType.aliveCount()).toBe(beforeCallbackCount);
      });

      await untilCallbacksSettled(csp, () => callbackCalled);
      expect(callbackCalled).toBe(true);
      expect(csp.BindingsTestType.aliveCount()).toBe(beforeCallbackCount);
    });

    it('Callback value container by const ref disposes contents at end of scope automatically', async () => {
      using helper = csp.CallbacksBindingMechanismsTestType.create(offThread);

      let callbackCalled = false;
      const beforeCallbackCount = csp.BindingsTestType.aliveCount();

      helper.callbackFunctionContainerOfValuesByConstRef((valueContainerArg) => {
        callbackCalled = true;
        expect(valueContainerArg.length).toBe(2);
        expect(csp.BindingsTestType.aliveCount()).toBe(beforeCallbackCount + 2);
      });

      await untilCallbacksSettled(csp, () => callbackCalled);
      expect(callbackCalled).toBe(true);
      expect(csp.BindingsTestType.aliveCount()).toBe(beforeCallbackCount);
    });

    it('Callback value container disposes contents at end of scope automatically', async () => {
      using helper = csp.CallbacksBindingMechanismsTestType.create(offThread);

      let callbackCalled = false;
      const beforeCallbackCount = csp.BindingsTestType.aliveCount();

      helper.callbackFunctionContainerOfValues((valueContainerArg) => {
        callbackCalled = true;
        expect(valueContainerArg.length).toBe(2);
        // We incur an additional copy due to not being const ref
        expect(csp.BindingsTestType.aliveCount()).toBe(beforeCallbackCount + 4);
      });

      await untilCallbacksSettled(csp, () => callbackCalled);
      expect(callbackCalled).toBe(true);
      expect(csp.BindingsTestType.aliveCount()).toBe(beforeCallbackCount);
    });

    it('Callback non-owning pointer container does nothing with contents at end of scope automatically', async () => {
      using helper = csp.CallbacksBindingMechanismsTestType.create(offThread);

      let callbackCalled = false;
      const beforeCallbackCount = csp.BindingsTestType.aliveCount();

      helper.callbackFunctionContainerOfPointers((pointerContainerArg) => {
        callbackCalled = true;
        expect(pointerContainerArg.length).toBe(2);
        // Pointers should not incur any new lifetimes
        expect(csp.BindingsTestType.aliveCount()).toBe(beforeCallbackCount);
      });

      await untilCallbacksSettled(csp, () => callbackCalled);
      expect(callbackCalled).toBe(true);
      expect(csp.BindingsTestType.aliveCount()).toBe(beforeCallbackCount);
    });

    it('Callback nested value container by const ref disposes contents at end of scope automatically', async () => {
      using helper = csp.CallbacksBindingMechanismsTestType.create(offThread);

      let callbackCalled = false;
      const beforeCallbackCount = csp.BindingsTestType.aliveCount();

      helper.callbackFunctionNestedContainerOfValuesByConstRef((nestedValueContainerArg) => {
        callbackCalled = true;
        expect(nestedValueContainerArg.size).toBe(2);
        expect(nestedValueContainerArg.get(0)?.length).toBe(2);
        expect(nestedValueContainerArg.get(1)?.length).toBe(2);
        expect(csp.BindingsTestType.aliveCount()).toBe(beforeCallbackCount + 4);
      });

      await untilCallbacksSettled(csp, () => callbackCalled);
      expect(callbackCalled).toBe(true);
      expect(csp.BindingsTestType.aliveCount()).toBe(beforeCallbackCount);
    });

    it('Callback nested value container disposes contents at end of scope automatically', async () => {
      using helper = csp.CallbacksBindingMechanismsTestType.create(offThread);

      let callbackCalled = false;
      const beforeCallbackCount = csp.BindingsTestType.aliveCount();

      helper.callbackFunctionNestedContainerOfValues((nestedValueContainerArg) => {
        callbackCalled = true;
        expect(nestedValueContainerArg.size).toBe(2);
        expect(nestedValueContainerArg.get(0)?.length).toBe(2);
        expect(nestedValueContainerArg.get(1)?.length).toBe(2);
        // We incur an additional copy per element due to not using const ref
        expect(csp.BindingsTestType.aliveCount()).toBe(beforeCallbackCount + 8);
      });

      await untilCallbacksSettled(csp, () => callbackCalled);
      expect(callbackCalled).toBe(true);
      expect(csp.BindingsTestType.aliveCount()).toBe(beforeCallbackCount);
    });

    it('Callback nested pointer container does nothing with contents at end of scope automatically', async () => {
      using helper = csp.CallbacksBindingMechanismsTestType.create(offThread);

      let callbackCalled = false;
      const beforeCallbackCount = csp.BindingsTestType.aliveCount();

      helper.callbackFunctionNestedContainerOfPointers((nestedPointerContainerArg) => {
        callbackCalled = true;
        expect(nestedPointerContainerArg.size).toBe(2);
        expect(nestedPointerContainerArg.get(0)?.length).toBe(2);
        expect(nestedPointerContainerArg.get(1)?.length).toBe(2);
        // Pointers should not incur additional lifetimes
        expect(csp.BindingsTestType.aliveCount()).toBe(beforeCallbackCount);
      });

      await untilCallbacksSettled(csp, () => callbackCalled);
      expect(callbackCalled).toBe(true);
      expect(csp.BindingsTestType.aliveCount()).toBe(beforeCallbackCount);
    });

    it('Callback with many mixed argument types', async () => {
      using helper = csp.CallbacksBindingMechanismsTestType.create(offThread);

      let callbackCalled = false;
      const beforeCallbackCount = csp.BindingsTestType.aliveCount();

      helper.callbackFunctionMixedArgs(
        (valueContainerArg, constRefContainerArg, pointerContainerArg, primitiveArg, valueArg, pointerArg) => {
          callbackCalled = true;
          expect(valueContainerArg.length).toBe(2);
          expect(constRefContainerArg.length).toBe(2);
          expect(pointerContainerArg.length).toBe(2);
          expect(primitiveArg).toBe(1);
          expect(valueArg.name).toBe('One');
          expect(pointerArg.name).toBe('One');
        }
      );

      await untilCallbacksSettled(csp, () => callbackCalled);
      expect(callbackCalled).toBe(true);

      // Should have the same lifetimes even despite all the args
      expect(csp.BindingsTestType.aliveCount()).toBe(beforeCallbackCount);
    });

    it('Function with callback and multiple primitive input args', async () => {
      using helper = csp.CallbacksBindingMechanismsTestType.create(offThread);

      let callbackCalled = false;

      helper.callbackFunctionMultiInputPrimitiveArg(1, 2, (primitiveArg) => {
        callbackCalled = true;

        expect(primitiveArg).toBe(3);
      });

      await until(() => callbackCalled);

      expect(callbackCalled).toBe(true);
    });

    it('Callback Optional Value Arg Disposal', async () => {
      using helper = csp.CallbacksBindingMechanismsTestType.create(offThread);

      const beforeCallbackCount = csp.BindingsTestType.aliveCount();

      let callbackCalled = false;
      helper.callbackFunctionValueOpt((valueArg) => {
        callbackCalled = true;
        expect(csp.BindingsTestType.aliveCount()).toBe(beforeCallbackCount + 1);
      });

      await untilCallbacksSettled(csp, () => callbackCalled);
      expect(callbackCalled).toBe(true);
      expect(csp.BindingsTestType.aliveCount()).toBe(beforeCallbackCount);
    });

    it('Callback Optional Pointer Arg Disposal', async () => {
      using helper = csp.CallbacksBindingMechanismsTestType.create(offThread);

      const beforeCallbackCount = csp.BindingsTestType.aliveCount();

      let callbackCalled = false;
      helper.callbackFunctionPointerOpt((pointerArg) => {
        callbackCalled = true;
        expect(csp.BindingsTestType.aliveCount()).toBe(beforeCallbackCount);
      });

      await untilCallbacksSettled(csp, () => callbackCalled);
      expect(callbackCalled).toBe(true);
      expect(csp.BindingsTestType.aliveCount()).toBe(beforeCallbackCount);
    });
  });
}
