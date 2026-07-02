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
      using arg = valueArg;
      callbackCalled = true;
      expect(arg.value).toBe(1);
      expect(arg.name).toBe('One');
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
      using arg = valueContainerArg;
      callbackCalled = true;
      expect(arg.length).toBe(2);
      expect(arg[0]?.value).toBe(1);
      expect(arg[0]?.name).toBe('One');
      expect(arg[1]?.value).toBe(2);
      expect(arg[1]?.name).toBe('Two');
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

  it('Value arg is disposable (owned copy)', () => {
    using helper = csp.CallbacksBindingMechanismsTestType.create();

    let callbackCalled = false;
    helper.callbackFunctionOnThreadValueArg((valueArg) => {
      callbackCalled = true;
      // A by-value arg is an owning copy, so it carries the dispose property.
      expect(Symbol.dispose in valueArg).toBe(true);

      // Disposing it frees that owned copy: the live count drops by one.
      // (In reality, you might rebind to `using` instead of explicit disposal, although I would rather an approach that didn't require any manual disposal)
      const beforeDispose = csp.BindingsTestType.aliveCount;
      valueArg[Symbol.dispose]();
      expect(csp.BindingsTestType.aliveCount).toBe(beforeDispose - 1);
    });

    expect(callbackCalled).toBe(true);
  });

  /*
   * Pointers are a tad weird. embind mechanics give every ClassHandle the dispose symbol, but we
   * deliberately don't put `& Disposable` on them in the TypeScript signature, because a pointer
   * arg is a non-owning borrow (almost always — Custom Materials are the one present exception).
   *
   * At present, pointers do not protect themselves well. You can still call .delete() on them
   * and it will delete CSP memory (they are however immune from garbage collection).
   * This is un-ideal, we should do further work in the bindings to remove the ability to do
   * this by setting a custom no-op Symbol.Dispose.
   */
  it('Pointer arg is a non-owning', () => {
    using helper = csp.CallbacksBindingMechanismsTestType.create();

    let callbackCalled = false;
    helper.callbackFunctionOnThreadPointerArg((pointerArg) => {
      callbackCalled = true;
      // The dispose symbol is present (inherited from ClassHandle) even though the TS type omits it.
      expect(Symbol.dispose in pointerArg).toBe(true);

      // Disposing however, currently DOES free the CSP-owned pointee.
      // This should change, it's not acceptable to have to just know ownership.
      const beforeDispose = csp.BindingsTestType.aliveCount;
      pointerArg[Symbol.dispose]();
      expect(csp.BindingsTestType.aliveCount).toBe(beforeDispose - 1);
    });

    expect(callbackCalled).toBe(true);
  });

  it('Container of values is disposable (owns its element copies)', () => {
    using helper = csp.CallbacksBindingMechanismsTestType.create();

    let callbackCalled = false;
    helper.callbackFunctionOnThreadContainerOfValues((valueContainerArg) => {
      callbackCalled = true;
      // The owning container carries the dispose marker...
      expect(Symbol.dispose in valueContainerArg).toBe(true);

      // Disposing it frees every owned element copy, ie, the live count drops by the element count.
      const elementCount = valueContainerArg.length;
      const beforeDispose = csp.BindingsTestType.aliveCount;
      valueContainerArg[Symbol.dispose]();
      expect(csp.BindingsTestType.aliveCount).toBe(beforeDispose - elementCount);
    });

    expect(callbackCalled).toBe(true);
  });

  it('Container of pointers is not disposable (non-owning)', () => {
    using helper = csp.CallbacksBindingMechanismsTestType.create();

    let callbackCalled = false;
    helper.callbackFunctionOnThreadContainerOfPointers((pointerContainerArg) => {
      callbackCalled = true;
      expect(Symbol.dispose in pointerContainerArg).toBe(false);

      // Pointer elements are non owning, but pointer containers are also non-disposable, so no disposal test even needed here.
    });

    expect(callbackCalled).toBe(true);
  });
});
