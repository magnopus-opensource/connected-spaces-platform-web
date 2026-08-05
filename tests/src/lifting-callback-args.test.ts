import { describe, it, expect, beforeAll, vi } from 'vitest';
import { loadCSP } from '../loadModule';
import { until } from '../testUtils';
import { BindingsTestType, type MainModule } from 'connected-spaces-platform-bindings';

/*
 * Tests that ensure our theory regarding taking arguments for use outside of
 * callbacks make sense
 */

const CALLBACK_THREADING_MODE = [
  { mode: 'On Thread', offThread: false },
  { mode: 'Off Thread', offThread: true }
];

for (const { mode, offThread } of CALLBACK_THREADING_MODE) {
  describe(`Lifting-Callback-Args (${mode})`, () => {
    let csp: MainModule;

    beforeAll(async () => {
      csp = await loadCSP();
    });

    it('Embind exceptions are catchable', async () => {
      using helper = csp.CallbacksBindingMechanismsTestType.create(offThread);

      let callbackCalled = false;
      let liftedValContainerArg: BindingsTestType[] = [];
      helper.callbackFunctionContainerOfValues((valueContainerArg) => {
        callbackCalled = true;
        expect(valueContainerArg[0]?.name).toBe('One');
        liftedValContainerArg = valueContainerArg;
      });

      await until(() => callbackCalled);
      expect(callbackCalled).toBe(true);
      let caughtDeletedAccess = false;
      try {
        let x = liftedValContainerArg?.[0]?.name;
      } catch (e) {
        caughtDeletedAccess = true;
      }
      expect(caughtDeletedAccess).toBe(true);

      let caughtDoubleDelete = false;
      try {
        liftedValContainerArg?.[0]?.delete();
      } catch (e) {
        caughtDoubleDelete = true;
      }
      expect(caughtDoubleDelete).toBe(true);
    });

    it('Try to reference a value arg throws', async () => {
      using helper = csp.CallbacksBindingMechanismsTestType.create(offThread);

      let callbackCalled = false;
      let liftedValArg: BindingsTestType | undefined;
      helper.callbackFunctionValueArg((valueArg) => {
        callbackCalled = true;
        expect(valueArg.name).toBe('One');
        liftedValArg = valueArg;
      });

      await until(() => callbackCalled);
      expect(callbackCalled).toBe(true);
      /* We reference the value arg out, which should throw an emscripten error as it's already been deleted */
      expect(() => liftedValArg?.name).toThrow();
    });

    it('Lift a value arg', async () => {
      using helper = csp.CallbacksBindingMechanismsTestType.create(offThread);

      let callbackCalled = false;
      let liftedValArg: BindingsTestType | undefined;
      let beforeAliveCount = csp.BindingsTestType.aliveCount;
      helper.callbackFunctionValueArgByConstRef((valueArg) => {
        callbackCalled = true;
        expect(valueArg.name).toBe('One');

        /* Cloning is a reference count shallow copy, should not incur lifetimes */
        let expectedLifetimes = beforeAliveCount + 1;
        expect(csp.BindingsTestType.aliveCount).toBe(expectedLifetimes);
        liftedValArg = valueArg.clone();
        expect(csp.BindingsTestType.aliveCount).toBe(expectedLifetimes);
      });

      await until(() => callbackCalled);
      expect(callbackCalled).toBe(true);
      expect(liftedValArg?.name).toBe('One');

      /* Doing the delete now should actually delete C++ memory, as the reference count will hit zero */
      expect(csp.BindingsTestType.aliveCount).toBe(beforeAliveCount + 1);
      liftedValArg?.delete();
      expect(csp.BindingsTestType.aliveCount).toBe(beforeAliveCount);

      /* Deleting again should throw, not crash */
      expect(() => liftedValArg?.delete()).toThrow();
    });

    it('RAII disposal of an already deleted argument warns instead of throwing', async () => {
      using helper = csp.CallbacksBindingMechanismsTestType.create(offThread);

      /*
       * This isn't a behaviour we really expect to happen.
       * But in the case that _somehow_, a RAII managed object is explicitly
       * deleted, the error needs to be a warning, rather than a hard crash.
       * This is because this is destruction behaviour, so throws can't happen.
       */
      const warnSpy = vi.spyOn(console, 'warn').mockImplementation(() => {});

      let callbackCalled = false;
      helper.callbackFunctionValueArg((valueArg) => {
        /* Callback args are non owning, should not allow clients to delete in the natural way */
        expect(valueArg.delete).toBeUndefined();

        /*
         * This is what we do internally to allow us to delete objects that we don't expose
         * .delete() methods to the client for (ie non-owning).
         */
        Object.getPrototypeOf(valueArg).delete.call(valueArg);
        callbackCalled = true;
      });

      await until(() => callbackCalled);

      /* On scope exit, we should have got the warning */
      expect(warnSpy).toHaveBeenCalledTimes(1);
      expect(warnSpy.mock.calls[0]?.[0]).toContain('already deleted');
    });

    it('RAII disposal of a regular arg does not warn', async () => {
      /* This test only exists because warns (from above test) are silent as far as automatic tests are concerned */

      using helper = csp.CallbacksBindingMechanismsTestType.create(offThread);
      const warnSpy = vi.spyOn(console, 'warn').mockImplementation(() => {});

      let callbackCalled = false;
      helper.callbackFunctionValueArg((valueArg) => {
        callbackCalled = true;
        expect(valueArg.name).toBe('One');
      });

      await until(() => callbackCalled);
      expect(callbackCalled).toBe(true);
      expect(warnSpy).not.toHaveBeenCalled();
    });

    it('Try to reference a pointer arg does not throw', async () => {
      using helper = csp.CallbacksBindingMechanismsTestType.create(offThread);

      let callbackCalled = false;
      let liftedPointerArg: BindingsTestType | undefined;
      helper.callbackFunctionPointerArg((pointerArg) => {
        callbackCalled = true;
        liftedPointerArg = pointerArg;
      });

      await until(() => callbackCalled);
      expect(callbackCalled).toBe(true);

      /*
       * Whilst we encourage use of .clone() for consisency, there's nothing formally wrong with lifting a non owning pointer directly.
       * We'd probably forbid this if we could, but you can't do much about assignment in JS.
       */
      expect(liftedPointerArg?.name).toBe('One');
    });

    it('Lift a pointer arg', async () => {
      using helper = csp.CallbacksBindingMechanismsTestType.create(offThread);

      let callbackCalled = false;
      let beforeAliveCount = csp.BindingsTestType.aliveCount; // We should never modify lifetimes dealing with pointers
      let liftedPointerArg: BindingsTestType | undefined;

      helper.callbackFunctionPointerArg((pointerArg) => {
        callbackCalled = true;
        liftedPointerArg = pointerArg.clone();
        expect(csp.BindingsTestType.aliveCount).toBe(beforeAliveCount);
      });

      await until(() => callbackCalled);
      expect(callbackCalled).toBe(true);
      expect(liftedPointerArg?.name).toBe('One');
      expect(csp.BindingsTestType.aliveCount).toBe(beforeAliveCount);

      /* Even if we delete the clone, it doesn't effect underlying memory, and cloning to get a new handle incurs no new lifetimes */
      let alternateHandleToPtr = liftedPointerArg?.clone();
      expect(csp.BindingsTestType.aliveCount).toBe(beforeAliveCount);

      liftedPointerArg?.delete();
      /* Accessing liftedPointerArg still throws however, you've invalidated the handle nonetheless */
      expect(() => liftedPointerArg?.delete()).toThrow();
      expect(alternateHandleToPtr?.name).toBe('One');

      expect(csp.BindingsTestType.aliveCount).toBe(beforeAliveCount);
    });

    it('Try to reference a container value arg throws', async () => {
      using helper = csp.CallbacksBindingMechanismsTestType.create(offThread);

      let callbackCalled = false;
      let liftedValContainerArg: BindingsTestType[] = [];
      helper.callbackFunctionContainerOfValues((valueContainerArg) => {
        callbackCalled = true;
        expect(valueContainerArg[0]?.name).toBe('One');
        liftedValContainerArg = valueContainerArg;
      });

      await until(() => callbackCalled);
      expect(callbackCalled).toBe(true);
      /* We reference the value arg out, which should throw an emscripten error as it's already been deleted */
      expect(() => liftedValContainerArg[0]?.name).toThrow();
    });

    it('Lift a container value arg', async () => {
      using helper = csp.CallbacksBindingMechanismsTestType.create(offThread);

      let callbackCalled = false;
      let liftedValContainerArg: BindingsTestType[] = [];
      let beforeAliveCount = csp.BindingsTestType.aliveCount;
      helper.callbackFunctionContainerOfValuesByConstRef((valueContainerArg) => {
        callbackCalled = true;
        expect(valueContainerArg[0]?.name).toBe('One');

        /* Cloning is a reference count shallow copy, should not incur lifetimes */
        let expectedLifetimes = beforeAliveCount + 2;
        expect(csp.BindingsTestType.aliveCount).toBe(expectedLifetimes);

        liftedValContainerArg = csp.cloneArray(valueContainerArg);

        expect(csp.BindingsTestType.aliveCount).toBe(expectedLifetimes);
      });

      await until(() => callbackCalled);
      expect(callbackCalled).toBe(true);
      expect(liftedValContainerArg?.[0]?.name).toBe('One');

      /* Doing the delete now should actually delete C++ memory, as the reference count will hit zero */
      expect(csp.BindingsTestType.aliveCount).toBe(beforeAliveCount + 2);
      csp.disposeArray(liftedValContainerArg);
      expect(csp.BindingsTestType.aliveCount).toBe(beforeAliveCount);

      /* Deleting again will throw */
      expect(() => csp.disposeArray(liftedValContainerArg)).toThrow();
    });
  });
}
