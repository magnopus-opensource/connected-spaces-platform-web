import { describe, it, expect, beforeAll } from 'vitest';
import { loadCSP } from '../loadModule';
import createModule, { BindingsTestType, type MainModule } from 'connected-spaces-platform-bindings';

/*
 * Tests that ensure our theory regarding taking arguments for use outside of
 * callbacks make sense
 */

describe('Callbacks', () => {
  let csp: MainModule;

  beforeAll(async () => {
    csp = await loadCSP();
  });

  it('Embind exceptions are catchable', () => {
    using helper = csp.CallbacksBindingMechanismsTestType.create();

    let callbackCalled = false;
    let liftedValContainerArg: BindingsTestType[] = [];
    helper.callbackFunctionOnThreadContainerOfValues((valueContainerArg) => {
      callbackCalled = true;
      expect(valueContainerArg[0]?.name).toBe('One');
      liftedValContainerArg = valueContainerArg;
    });

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

  it('Try to reference a value arg throws', () => {
    using helper = csp.CallbacksBindingMechanismsTestType.create();

    let callbackCalled = false;
    let liftedValArg: BindingsTestType | undefined;
    helper.callbackFunctionOnThreadValueArg((valueArg) => {
      callbackCalled = true;
      expect(valueArg.name).toBe('One');
      liftedValArg = valueArg;
    });

    expect(callbackCalled).toBe(true);
    /* We reference the value arg out, which should throw an emscripten error as it's already been deleted */
    expect(() => liftedValArg?.name).toThrow();
  });

  it('Lift a value arg', () => {
    using helper = csp.CallbacksBindingMechanismsTestType.create();

    let callbackCalled = false;
    let liftedValArg: BindingsTestType | undefined;
    let beforeAliveCount = csp.BindingsTestType.aliveCount;
    helper.callbackFunctionOnThreadValueArgByConstRef((valueArg) => {
      callbackCalled = true;
      expect(valueArg.name).toBe('One');

      /* Cloning is a reference count shallow copy, should not incur lifetimes */
      let expectedLiftimes = beforeAliveCount + 1;
      expect(csp.BindingsTestType.aliveCount).toBe(expectedLiftimes);
      liftedValArg = valueArg.clone();
      expect(csp.BindingsTestType.aliveCount).toBe(expectedLiftimes);
    });

    expect(callbackCalled).toBe(true);
    expect(liftedValArg?.name).toBe('One');

    /* Doing the delete now should actually delete C++ memory, as the reference count will hit zero */
    expect(csp.BindingsTestType.aliveCount).toBe(beforeAliveCount + 1);
    liftedValArg?.delete();
    expect(csp.BindingsTestType.aliveCount).toBe(beforeAliveCount);

    /* Deleting again should throw, not crash */
    expect(() => liftedValArg?.delete()).toThrow();
  });

  it('Try to reference a pointer arg does not throw', () => {
    using helper = csp.CallbacksBindingMechanismsTestType.create();

    let callbackCalled = false;
    let liftedPointerArg: BindingsTestType | undefined;
    helper.callbackFunctionOnThreadPointerArg((pointerArg) => {
      callbackCalled = true;
      liftedPointerArg = pointerArg;
    });

    expect(callbackCalled).toBe(true);

    /*
     * Whilst we encourage use of .clone() for consisency, there's nothing formally wrong with lifting a non owning pointer directly.
     * We'd probably forbid this if we could, but you can't do much about assignment in JS.
     */
    expect(liftedPointerArg?.name).toBe('One');
  });

  it('Lift a pointer arg', () => {
    using helper = csp.CallbacksBindingMechanismsTestType.create();

    let callbackCalled = false;
    let beforeAliveCount = csp.BindingsTestType.aliveCount; // We should never modify lifetimes dealing with pointers
    let liftedPointerArg: BindingsTestType | undefined;

    helper.callbackFunctionOnThreadPointerArg((pointerArg) => {
      callbackCalled = true;
      liftedPointerArg = pointerArg.clone();
      expect(csp.BindingsTestType.aliveCount).toBe(beforeAliveCount);
    });

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

  it('Try to reference a container value arg throws', () => {
    using helper = csp.CallbacksBindingMechanismsTestType.create();

    let callbackCalled = false;
    let liftedValContainerArg: BindingsTestType[] = [];
    helper.callbackFunctionOnThreadContainerOfValues((valueContainerArg) => {
      callbackCalled = true;
      expect(valueContainerArg[0]?.name).toBe('One');
      liftedValContainerArg = valueContainerArg;
    });

    expect(callbackCalled).toBe(true);
    /* We reference the value arg out, which should throw an emscripten error as it's already been deleted */
    expect(() => liftedValContainerArg[0]?.name).toThrow();
  });

  it('Lift a container value arg', () => {
    using helper = csp.CallbacksBindingMechanismsTestType.create();

    let callbackCalled = false;
    let liftedValContainerArg: BindingsTestType[] = [];
    let beforeAliveCount = csp.BindingsTestType.aliveCount;
    helper.callbackFunctionOnThreadContainerOfValuesByConstRef((valueContainerArg) => {
      callbackCalled = true;
      expect(valueContainerArg[0]?.name).toBe('One');

      /* Cloning is a reference count shallow copy, should not incur lifetimes */
      let expectedLifetimes = beforeAliveCount + 2;
      expect(csp.BindingsTestType.aliveCount).toBe(expectedLifetimes);

      /*
       * TODO, we probably want to add a nice .clone() method here for CSP containers, rather than doing it manually.
       * It would just do this behind the scenes though.
       */
      liftedValContainerArg = valueContainerArg.map((x) => x.clone());

      expect(csp.BindingsTestType.aliveCount).toBe(expectedLifetimes);
    });

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
