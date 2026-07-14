import { describe, it, expect, beforeAll } from 'vitest';
import { loadCSP } from '../loadModule';
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
 */

describe('Callbacks', () => {
  let csp: MainModule;

  beforeAll(async () => {
    csp = await loadCSP();
  });

  it('On Thread Callback No Args', () => {
    using helper = csp.CallbacksBindingMechanismsTestType.create();

    let callbackCalled = false;
    helper.callbackFunctionOnThreadNoArgs(() => {
      callbackCalled = true;
    });

    expect(callbackCalled).toBe(true);
  });

  it('On Thread Callback Primitive Arg', () => {
    using helper = csp.CallbacksBindingMechanismsTestType.create();

    let callbackCalled = false;
    helper.callbackFunctionOnThreadPrimitiveArg((primitiveArg) => {
      callbackCalled = true;
      expect(primitiveArg).toBe(10);
    });

    expect(callbackCalled).toBe(true);
  });

  it('On Thread Callback Pointer Arg', () => {
    using helper = csp.CallbacksBindingMechanismsTestType.create();

    let callbackCalled = false;
    helper.callbackFunctionOnThreadPointerArg((pointerArg) => {
      callbackCalled = true;
      expect(pointerArg.value).toBe(1);
      expect(pointerArg.name).toBe('One');
    });

    expect(callbackCalled).toBe(true);
  });

  it('On Thread Callback Value Arg', () => {
    using helper = csp.CallbacksBindingMechanismsTestType.create();

    let callbackCalled = false;
    helper.callbackFunctionOnThreadValueArg((valueArg) => {
      callbackCalled = true;
      expect(valueArg.value).toBe(1);
      expect(valueArg.name).toBe('One');
    });

    expect(callbackCalled).toBe(true);
  });

  it('On Thread Callback Container of Pointers Arg', () => {
    using helper = csp.CallbacksBindingMechanismsTestType.create();

    let callbackCalled = false;
    helper.callbackFunctionOnThreadContainerOfPointers((pointerContainerArg) => {
      callbackCalled = true;
      expect(pointerContainerArg.length).toBe(2);
      expect(pointerContainerArg[0]?.value).toBe(1);
      expect(pointerContainerArg[0]?.name).toBe('One');
      expect(pointerContainerArg[1]?.value).toBe(2);
      expect(pointerContainerArg[1]?.name).toBe('Two');
    });

    expect(callbackCalled).toBe(true);
  });

  it('On Thread Callback Container of Values Arg', () => {
    using helper = csp.CallbacksBindingMechanismsTestType.create();

    let callbackCalled = false;
    helper.callbackFunctionOnThreadContainerOfValues((valueContainerArg) => {
      callbackCalled = true;
      expect(valueContainerArg.length).toBe(2);
      expect(valueContainerArg[0]?.value).toBe(1);
      expect(valueContainerArg[0]?.name).toBe('One');
      expect(valueContainerArg[1]?.value).toBe(2);
      expect(valueContainerArg[1]?.name).toBe('Two');
    });

    expect(callbackCalled).toBe(true);
  });

  it('On Thread Callback Multiple Args', () => {
    using helper = csp.CallbacksBindingMechanismsTestType.create();

    let callbackCalled = false;
    helper.callbackFunctionOnThreadMultipleArgs((primitiveArg1, primitiveArg2) => {
      callbackCalled = true;
      expect(primitiveArg1).toBe(1);
      expect(primitiveArg2).toBe(2);
    });

    expect(callbackCalled).toBe(true);
  });

  it('On Thread Callback That Throws', () => {
    using helper = csp.CallbacksBindingMechanismsTestType.create();

    // Syncronous callbacks should behave just like nested calls */
    let callbackCalled = false;
    expect(() => {
      helper.callbackFunctionOnThreadNoArgs(() => {
        callbackCalled = true;
        throw new Error('test error');
      });
    }).toThrow('test error');

    expect(callbackCalled).toBe(true);

    // The module must remain usable after an exception has unwound through it.
    let secondCallbackCalled = false;
    helper.callbackFunctionOnThreadNoArgs(() => {
      secondCallbackCalled = true;
    });
    expect(secondCallbackCalled).toBe(true);
  });

  it('Pointer arg is non-owning', () => {
    using helper = csp.CallbacksBindingMechanismsTestType.create();

    let callbackCalled = false;
    helper.callbackFunctionOnThreadPointerArg((pointerArg) => {
      callbackCalled = true;
      expect(() => pointerArg.delete()).toThrow();
      expect(() => pointerArg.deleteLater()).toThrow();
      expect(() => pointerArg.clone()).toThrow();
      expect(() => pointerArg[Symbol.dispose]()).toThrow();
    });

    expect(callbackCalled).toBe(true);
  });

  /*
   * Tests callback argument lifetime specificities.
   * The theory here is that all owning arguments (read: value types), are COPIED into the JS runtime.
   * These copies are scoped to live ONLY for the scope of the callback, and will be automatically
   * disposed. The idea being that we are trading off leak-protection + unlikelihood of crashes for
   * some annoyance in how you must copy variables out of callback scope, rather than naturally referencing them,
   * if you wish to use them external to the callback scope.
   *
   * The lifetime value delta represents the amount of additiona copies the binding layer itself adds
   * to the objects. The backend CSP interop container types can add a fair amount of redundant copies
   * themselves (default constructions), but we seperate that in the test infra so it's not a concern here.
   *
   */

  it('Callback by const ref arg disposes at end of scope automatically', () => {
    using helper = csp.CallbacksBindingMechanismsTestType.create();

    let callbackCalled = false;
    const beforeCallbackCount = csp.BindingsTestType.aliveCount;

    helper.callbackFunctionOnThreadValueArgByConstRef((byRefArg) => {
      callbackCalled = true;
      expect(csp.BindingsTestType.aliveCount).toBe(beforeCallbackCount + 1);
    });

    expect(callbackCalled).toBe(true);
    expect(csp.BindingsTestType.aliveCount).toBe(beforeCallbackCount);
  });

  it('Callback value arg disposes at end of scope automatically', () => {
    using helper = csp.CallbacksBindingMechanismsTestType.create();

    let callbackCalled = false;
    const beforeCallbackCount = csp.BindingsTestType.aliveCount;

    helper.callbackFunctionOnThreadValueArg((valueArg) => {
      callbackCalled = true;
      expect(csp.BindingsTestType.aliveCount).toBe(beforeCallbackCount + 2);
    });

    expect(callbackCalled).toBe(true);
    expect(csp.BindingsTestType.aliveCount).toBe(beforeCallbackCount);
  });

  it('Callback non-owning pointer arg does nothing at end of scope automatically', () => {
    using helper = csp.CallbacksBindingMechanismsTestType.create();

    let callbackCalled = false;
    const beforeCallbackCount = csp.BindingsTestType.aliveCount;

    helper.callbackFunctionOnThreadPointerArg((pointerArg) => {
      callbackCalled = true;
      // Pointers should not incur additional lifetimes
      expect(csp.BindingsTestType.aliveCount).toBe(beforeCallbackCount);
    });

    expect(callbackCalled).toBe(true);
    expect(csp.BindingsTestType.aliveCount).toBe(beforeCallbackCount);
  });

  it('Callback value container by const ref disposes contents at end of scope automatically', () => {
    using helper = csp.CallbacksBindingMechanismsTestType.create();

    let callbackCalled = false;
    const beforeCallbackCount = csp.BindingsTestType.aliveCount;

    helper.callbackFunctionOnThreadContainerOfValuesByConstRef((valueContainerArg) => {
      callbackCalled = true;
      expect(valueContainerArg.length).toBe(2);
      expect(csp.BindingsTestType.aliveCount).toBe(beforeCallbackCount + 2);
    });

    expect(callbackCalled).toBe(true);
    expect(csp.BindingsTestType.aliveCount).toBe(beforeCallbackCount);
  });

  it('Callback value container disposes contents at end of scope automatically', () => {
    using helper = csp.CallbacksBindingMechanismsTestType.create();

    let callbackCalled = false;
    const beforeCallbackCount = csp.BindingsTestType.aliveCount;

    helper.callbackFunctionOnThreadContainerOfValues((valueContainerArg) => {
      callbackCalled = true;
      expect(valueContainerArg.length).toBe(2);
      // We incur an additional copy due to not being const ref
      expect(csp.BindingsTestType.aliveCount).toBe(beforeCallbackCount + 4);
    });

    expect(callbackCalled).toBe(true);
    expect(csp.BindingsTestType.aliveCount).toBe(beforeCallbackCount);
  });

  it('Callback non-owning pointer container does nothing with contents at end of scope automatically', () => {
    using helper = csp.CallbacksBindingMechanismsTestType.create();

    let callbackCalled = false;
    const beforeCallbackCount = csp.BindingsTestType.aliveCount;

    helper.callbackFunctionOnThreadContainerOfPointers((pointerContainerArg) => {
      callbackCalled = true;
      expect(pointerContainerArg.length).toBe(2);
      // Pointers should not incur any new lifetimes
      expect(csp.BindingsTestType.aliveCount).toBe(beforeCallbackCount);
    });

    expect(callbackCalled).toBe(true);
    expect(csp.BindingsTestType.aliveCount).toBe(beforeCallbackCount);
  });

  it('Callback nested value container by const ref disposes contents at end of scope automatically', () => {
    using helper = csp.CallbacksBindingMechanismsTestType.create();

    let callbackCalled = false;
    const beforeCallbackCount = csp.BindingsTestType.aliveCount;

    helper.callbackFunctionOnThreadNestedContainerOfValuesByConstRef((nestedValueContainerArg) => {
      callbackCalled = true;
      expect(nestedValueContainerArg.size).toBe(2);
      expect(nestedValueContainerArg.get(0)?.length).toBe(2);
      expect(nestedValueContainerArg.get(1)?.length).toBe(2);
      expect(csp.BindingsTestType.aliveCount).toBe(beforeCallbackCount + 4);
    });

    expect(callbackCalled).toBe(true);
    expect(csp.BindingsTestType.aliveCount).toBe(beforeCallbackCount);
  });

  it('Callback nested value container disposes contents at end of scope automatically', () => {
    using helper = csp.CallbacksBindingMechanismsTestType.create();

    let callbackCalled = false;
    const beforeCallbackCount = csp.BindingsTestType.aliveCount;

    helper.callbackFunctionOnThreadNestedContainerOfValues((nestedValueContainerArg) => {
      callbackCalled = true;
      expect(nestedValueContainerArg.size).toBe(2);
      expect(nestedValueContainerArg.get(0)?.length).toBe(2);
      expect(nestedValueContainerArg.get(1)?.length).toBe(2);
      // We incur an additional copy per element due to not using const ref
      expect(csp.BindingsTestType.aliveCount).toBe(beforeCallbackCount + 8);
    });

    expect(callbackCalled).toBe(true);
    expect(csp.BindingsTestType.aliveCount).toBe(beforeCallbackCount);
  });

  it('Callback nested pointer container does nothing with contents at end of scope automatically', () => {
    using helper = csp.CallbacksBindingMechanismsTestType.create();

    let callbackCalled = false;
    const beforeCallbackCount = csp.BindingsTestType.aliveCount;

    helper.callbackFunctionOnThreadNestedContainerOfPointers((nestedPointerContainerArg) => {
      callbackCalled = true;
      expect(nestedPointerContainerArg.size).toBe(2);
      expect(nestedPointerContainerArg.get(0)?.length).toBe(2);
      expect(nestedPointerContainerArg.get(1)?.length).toBe(2);
      // Pointers should not incur additional lifetimes
      expect(csp.BindingsTestType.aliveCount).toBe(beforeCallbackCount);
    });

    expect(callbackCalled).toBe(true);
    expect(csp.BindingsTestType.aliveCount).toBe(beforeCallbackCount);
  });

  it('Callback with many mixed argument types', () => {
    using helper = csp.CallbacksBindingMechanismsTestType.create();

    let callbackCalled = false;
    const beforeCallbackCount = csp.BindingsTestType.aliveCount;

    helper.callbackFunctionOnThreadMixedArgs(
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

    expect(callbackCalled).toBe(true);
    // Should have the same lifetimes even despite all the args
    expect(csp.BindingsTestType.aliveCount).toBe(beforeCallbackCount);
  });
});
