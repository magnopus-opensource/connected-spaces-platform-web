import { describe, it, expect, beforeAll } from 'vitest';
import { loadCSP } from '../loadModule';
import type { BindingsTestType, MainModule } from 'connected-spaces-platform-bindings';

/*
 * Exercise our custom container clone helpers, cloneArray and cloneMap, across every
 * container shape a callback argument can take (see CallbacksBindingMechanismsTestType).
 * Not concerned with regular handle clones.
 *
 * Tests done via callback lifting, as that's the main point of the helper methods:
 * cloning is how you take an owning value out of a callback with a lifetime you control.
 * Cloning is a reference-count shallow copy, so it never incurs new C++ lifetimes; the
 * clones merely keep the underlying objects alive until you dispose them yourself.
 */

describe('clone ', () => {
  let csp: MainModule;

  beforeAll(async () => {
    csp = await loadCSP();
  });

  it('Clone Array', () => {
    using helper = csp.CallbacksBindingMechanismsTestType.create();

    let callbackCalled = false;
    let liftedValContainerArg: BindingsTestType[] = [];
    let beforeAliveCount = csp.BindingsTestType.aliveCount;
    helper.callbackFunctionOnThreadContainerOfValuesByConstRef((arrayAry) => {
      callbackCalled = true;

      let expectedLifetimes = beforeAliveCount + 2;
      expect(csp.BindingsTestType.aliveCount).toBe(expectedLifetimes);
      liftedValContainerArg = csp.cloneArray(arrayAry);
      expect(csp.BindingsTestType.aliveCount).toBe(expectedLifetimes);
    });

    expect(callbackCalled).toBe(true);
    expect(liftedValContainerArg?.[0]?.name).toBe('One');
    expect(liftedValContainerArg?.[1]?.name).toBe('Two');

    /* Doing the delete now should actually delete C++ memory, as the reference count will hit zero */
    expect(csp.BindingsTestType.aliveCount).toBe(beforeAliveCount + 2);
    csp.disposeArray(liftedValContainerArg);
    expect(csp.BindingsTestType.aliveCount).toBe(beforeAliveCount);

    /* Deleting again will throw */
    expect(() => csp.disposeArray(liftedValContainerArg)).toThrow();
  });

  it('Clone Array of Pointers', () => {
    using helper = csp.CallbacksBindingMechanismsTestType.create();

    let callbackCalled = false;
    let liftedPointerContainerArg: BindingsTestType[] = [];
    let beforeAliveCount = csp.BindingsTestType.aliveCount;
    helper.callbackFunctionOnThreadContainerOfPointers((pointerAry) => {
      callbackCalled = true;

      expect(csp.BindingsTestType.aliveCount).toBe(beforeAliveCount);
      liftedPointerContainerArg = csp.cloneArray(pointerAry);
      expect(csp.BindingsTestType.aliveCount).toBe(beforeAliveCount);
    });

    expect(callbackCalled).toBe(true);

    expect(liftedPointerContainerArg?.[0]?.name).toBe('One');
    expect(liftedPointerContainerArg?.[1]?.name).toBe('Two');

    /* Nothing was owned, so there is nothing to dispose.*/
    expect(csp.BindingsTestType.aliveCount).toBe(beforeAliveCount);
  });

  it('Clone Map', () => {
    using helper = csp.CallbacksBindingMechanismsTestType.create();

    let callbackCalled = false;
    let liftedValMapArg: Map<number, BindingsTestType[]> = new Map();
    let beforeAliveCount = csp.BindingsTestType.aliveCount;
    helper.callbackFunctionOnThreadNestedContainerOfValuesByConstRef((mapArg) => {
      callbackCalled = true;

      let expectedLifetimes = beforeAliveCount + 4;
      expect(csp.BindingsTestType.aliveCount).toBe(expectedLifetimes);
      liftedValMapArg = csp.cloneMap(mapArg);
      expect(csp.BindingsTestType.aliveCount).toBe(expectedLifetimes);
    });

    expect(callbackCalled).toBe(true);
    expect(liftedValMapArg.get(0)?.[0]?.name).toBe('One');
    expect(liftedValMapArg.get(0)?.[1]?.name).toBe('Two');
    expect(liftedValMapArg.get(1)?.[0]?.name).toBe('Three');

    expect(csp.BindingsTestType.aliveCount).toBe(beforeAliveCount + 4);
    csp.disposeMap(liftedValMapArg);
    expect(csp.BindingsTestType.aliveCount).toBe(beforeAliveCount);

    /* Deleting again will throw */
    expect(() => csp.disposeMap(liftedValMapArg)).toThrow();
  });

  it('Clone Map of Pointers', () => {
    using helper = csp.CallbacksBindingMechanismsTestType.create();

    let callbackCalled = false;
    let liftedPointerMapArg: Map<number, BindingsTestType[]> = new Map();
    let beforeAliveCount = csp.BindingsTestType.aliveCount;
    helper.callbackFunctionOnThreadNestedContainerOfPointers((mapArg) => {
      callbackCalled = true;

      expect(csp.BindingsTestType.aliveCount).toBe(beforeAliveCount);
      liftedPointerMapArg = csp.cloneMap(mapArg);
      expect(csp.BindingsTestType.aliveCount).toBe(beforeAliveCount);
    });

    expect(callbackCalled).toBe(true);

    expect(liftedPointerMapArg.get(0)?.[0]?.name).toBe('One');
    expect(liftedPointerMapArg.get(0)?.[1]?.name).toBe('Two');
    expect(liftedPointerMapArg.get(1)?.[0]?.name).toBe('Three');

    /* Nothing was owned, so there is nothing to dispose. */
    expect(csp.BindingsTestType.aliveCount).toBe(beforeAliveCount);
  });

  it('Clone Optional of Array', () => {
    using helper = csp.CallbacksBindingMechanismsTestType.create();

    let callbackCalled = false;
    let liftedOptOfArrayArg: BindingsTestType[] = [];
    let beforeAliveCount = csp.BindingsTestType.aliveCount;
    helper.callbackFunctionOnThreadOptOfArray((optOfArrayArg) => {
      callbackCalled = true;

      /* The optional is an array of two owned elements */
      expect(optOfArrayArg).toBeDefined();
      let expectedLifetimes = beforeAliveCount + 2;
      expect(csp.BindingsTestType.aliveCount).toBe(expectedLifetimes);

      /* An optional is just `T[] | undefined` on the JS side, so it clones as a plain array */
      liftedOptOfArrayArg = csp.cloneArray(optOfArrayArg!);
      expect(csp.BindingsTestType.aliveCount).toBe(expectedLifetimes);
    });

    expect(callbackCalled).toBe(true);
    expect(liftedOptOfArrayArg?.[0]?.name).toBe('One');
    expect(liftedOptOfArrayArg?.[1]?.name).toBe('Two');

    /* The clones keep both elements alive until we dispose them */
    expect(csp.BindingsTestType.aliveCount).toBe(beforeAliveCount + 2);
    csp.disposeArray(liftedOptOfArrayArg);
    expect(csp.BindingsTestType.aliveCount).toBe(beforeAliveCount);

    /* Deleting again will throw */
    expect(() => csp.disposeArray(liftedOptOfArrayArg)).toThrow();
  });

  it('Clone Array of Optionals with an undefined element', () => {
    using helper = csp.CallbacksBindingMechanismsTestType.create();

    let callbackCalled = false;
    let liftedArrayOfOptArg: (BindingsTestType | undefined)[] = [];
    let beforeAliveCount = csp.BindingsTestType.aliveCount;
    helper.callbackFunctionOnThreadArrayOfSomeNullOpt((arrayOfSomeNullOptArg) => {
      callbackCalled = true;

      /* One null optional, one present, thus only a single lifetime */
      let expectedLifetimes = beforeAliveCount + 1;
      expect(csp.BindingsTestType.aliveCount).toBe(expectedLifetimes);
      /* cloneArray walk should be lenient to undefined, and allow the clone */
      liftedArrayOfOptArg = csp.cloneArray(arrayOfSomeNullOptArg);
      expect(csp.BindingsTestType.aliveCount).toBe(expectedLifetimes);
    });

    expect(callbackCalled).toBe(true);
    expect(liftedArrayOfOptArg?.[0]).toBeUndefined();
    expect(liftedArrayOfOptArg?.[1]?.name).toBe('Two');

    expect(csp.BindingsTestType.aliveCount).toBe(beforeAliveCount + 1);
    csp.disposeArray(liftedArrayOfOptArg);
    expect(csp.BindingsTestType.aliveCount).toBe(beforeAliveCount);

    /* Deleting again will throw */
    expect(() => csp.disposeArray(liftedArrayOfOptArg)).toThrow();
  });

  /* Not really a necessary part of the interface, but ensure that we don't explode when doing a grab-bag array */
  it('Clone a mixed array of handles and plain JS values', () => {
    let beforeAliveCount = csp.BindingsTestType.aliveCount;

    let handleA = csp.BindingsTestType.create(1, 'One');
    let handleB = csp.BindingsTestType.create(2, 'Two');

    /* Bunch of random types */
    let mixed: any[] = [handleA, 42, 'a string', { key: 'val' }, null, [handleB, { boolkey: true }]];

    /* Only the two handles incur lifetimes; the plain values do not */
    expect(csp.BindingsTestType.aliveCount).toBe(beforeAliveCount + 2);

    let cloned = csp.cloneArray(mixed);
    /* Get rid of the original handles, reference count will be held */
    csp.disposeArray(mixed);
    expect(csp.BindingsTestType.aliveCount).toBe(beforeAliveCount + 2);

    /* Cloning is a reference count shallow copy, should not incur lifetimes */
    expect(csp.BindingsTestType.aliveCount).toBe(beforeAliveCount + 2);

    expect(cloned).not.toBe(mixed);
    expect(cloned[0]).not.toBe(handleA);
    expect(cloned[0]?.name).toBe('One');

    expect(cloned[1]).toBe(42);
    expect(cloned[2]).toBe('a string');
    expect(cloned[3]).toBe(mixed[3]);
    expect(cloned[4]).toBeNull();

    /* Nested arrays are rebuilt, their handles will not be the same, but they still have the same values */
    expect(cloned[5]).not.toBe(mixed[5]);
    expect(cloned[5][0]).not.toBe(handleB);
    expect(cloned[5][0]?.name).toBe('Two');
    expect(cloned[5][1]).toBe(mixed[5][1]);

    csp.disposeArray(cloned);
    expect(csp.BindingsTestType.aliveCount).toBe(beforeAliveCount);

    /* Deleting again will throw */
    expect(() => csp.disposeArray(cloned)).toThrow();
  });
});
