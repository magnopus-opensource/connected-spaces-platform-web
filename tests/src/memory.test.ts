import { describe, it, expect, beforeAll } from 'vitest';
import { loadCSP } from '../loadModule';
import type { MainModule } from 'connected-spaces-platform-bindings';

describe('CSPFoundation', () => {
  let csp: MainModule;

  beforeAll(async () => {
    csp = await loadCSP();
  });

/* 
 * Test some assumptions about emscripten and automatic memory management
 * Arguably we're testing the library here, but browsers are
 * mysterious things and this proves we can rely on this sort
 * of thing at least on the chromium target.
 * 
 * Quite important i'd say, we'll likely build user patterns around these assumptions.
 */

  it('Delete releases C++ memory', () => {
    const before = csp.BindingsTestType.aliveCount;

    const leaked = csp.BindingsTestType.create(42, 'hello');
    expect(csp.BindingsTestType.aliveCount).toBe(before + 1);

    leaked.delete();
    expect(csp.BindingsTestType.aliveCount).toBe(before);
  });

 it('Releases C++ memory at scope exit', () => {
    const before = csp.BindingsTestType.aliveCount;

    {
      using t = csp.BindingsTestType.create(42, 'hello');
      expect(csp.BindingsTestType.aliveCount).toBe(before + 1);
    }

    expect(csp.BindingsTestType.aliveCount).toBe(before);
  });

  it('Releases all C++ memory at scope exit', () => {
    const before = csp.BindingsTestType.aliveCount;
    {
      using a = csp.BindingsTestType.create(42, 'hello');
      using b = csp.BindingsTestType.create(42, 'hello');
      using c = csp.BindingsTestType.create(42, 'hello');
      expect(csp.BindingsTestType.aliveCount).toBe(before + 3);
    }
    expect(csp.BindingsTestType.aliveCount).toBe(before);
  });

  it('Releases C++ memory when scope exits via exception', () => {
    const before = csp.BindingsTestType.aliveCount;

    expect(() => {
      using t = csp.BindingsTestType.create(42, 'hello');
      expect(csp.BindingsTestType.aliveCount).toBe(before + 1);
      throw new Error('Oh no!');
    }).toThrow('Oh no!');

    expect(csp.BindingsTestType.aliveCount).toBe(before);
  });

  it('Throws on double delete', () => {
    const t = csp.BindingsTestType.create(42, 'hello');
    t.delete();
    expect(() => t.delete()).toThrow("BindingsTestType instance already deleted");
  });

  it('Throws when accessing a deleted handle', () => {
    const t = csp.BindingsTestType.create(42, 'hello');
    t.delete();
    expect(() => t.value).toThrow("cannot call emscripten binding method BindingsTestType.value getter on deleted object");
  });

  /*
   * This test is very important for our non-owning pointers in array strategy.
   * If we can rely on the embind proxies being GC'd, we don't need to do anything
   * very fancy to delete them on disposal. We can just not call delete, and thus
   * not need to attach a dispose Symbol, to non-owning pointer arrays.
   */
  it('reference-returned pointer proxies are eligible for GC when dropped', async () => {
    using bindingsArrayHelper = csp.BindingsMechanismsTestType.create();
    using anchor = csp.BindingsTestType.create(1, 'one');
    bindingsArrayHelper.setArrayOfPointersByValue([anchor]);

    // FinalizationRegistry callbacks fire when the registered object is GC'd.
    // If proxies were pinned by some embind internal, ~zero callbacks would fire.
    // If they're GC-eligible, most will fire under memory pressure.
    let collected = 0;
    const registry = new FinalizationRegistry(() => { collected++; });
    const N = 10_000;

    // Inner scope so references drop after the block.
    (() => {
      for (let i = 0; i < N; i++) {
        const arr = bindingsArrayHelper.getArrayOfPointersByValue();
        registry.register(arr[0]!, undefined);
        // Intentionally no dispose — that's the whole point.
      }
    })();

    // Encourage GC via large allocations. JS engines run major GC when heap
    // pressure rises; we can't force it but we can make it very likely.
    for (let i = 0; i < 50; i++) {
      // Allocate, then let the buffer drop. 
      new ArrayBuffer(10_000_000);
    }

    // Yield to the event loop so GC + finalization callbacks drain. Two ticks
    // because finalizers schedule on a separate microtask queue in some engines.
    await new Promise(r => setTimeout(r, 100));
    await new Promise(r => setTimeout(r, 0));

    // Threshold is intentionally generous. Across runs we expect 80-99%, but
    // GC timing isn't guaranteed; 50% is enough to distinguish "proxies can
    // be reclaimed" from "proxies are pinned" without flaking on slow GC.
    expect(collected).toBeGreaterThan(N * 0.5);
  });

  /*
   * Test that objects in C++ are disconnected from non-owning proxies.
   * No matter how many proxies we create, we don't call constructors
   * or destructors on underlying C++ objects
   */
  it('reference-returned proxies do not pin the underlying C++ object', () => {
    using bindingsArrayHelper = csp.BindingsMechanismsTestType.create();
    using anchor = csp.BindingsTestType.create(1, 'one');
    bindingsArrayHelper.setArrayOfPointersByValue([anchor]);

    const baseline = csp.BindingsTestType.aliveCount;

    // Create and drop 100K reference-returned proxies without disposing them.
    // If the proxies somehow took ownership of the underlying object, aliveCount
    // would diverge. We expect it to stay flat — only `anchor` owns the C++ instance.
    for (let i = 0; i < 100_000; i++) {
      const arr = bindingsArrayHelper.getArrayOfPointersByValue();
      expect(arr[0]?.value).toBe(1);
      // Intentionally no dispose — that's the whole point.
    }

    expect(csp.BindingsTestType.aliveCount).toBe(baseline);
  });

  /*
   * This _really_ proves the assumption that finalizers are clearing up any
   * CPP memory that embind might be using in its internal proxy implementation,
   * using the wasm heap rather than a semantic check via AliveCount.
   * This is because any C++ object is actually 2 objects right, it's the actual
   * C++ memory as you'd expect, but then embind has a proxy object in order
   * to marshall to JS. I'm concerned about the proxy objects here.
   * Non-owning pointers are truly non-owning and do not leak.
   */
  it('reference-returned proxies do not grow wasm heap', () => {
    using bindingsArrayHelper = csp.BindingsMechanismsTestType.create();
    using anchor = csp.BindingsTestType.create(1, 'one');
    bindingsArrayHelper.setArrayOfPointersByValue([anchor]);

    // Warm up so we can get to steady state behavior, early calls can always be doing
    // dynamic caching and whatnot, just for safety.
    for (let i = 0; i < 1000; i++) {
      const arr = bindingsArrayHelper.getArrayOfPointersByValue();
      void arr[0]?.value;
    }

    // Take baseline.
    const heapBefore = (csp as unknown as { HEAPU8: Uint8Array }).HEAPU8.byteLength;

    // Steady-state loop. If even 1 byte per proxy leaks in C++, ~100K iterations
    // crosses one 64 KB page boundary and HEAPU8 grows. If nothing leaks, flat.
    for (let i = 0; i < 100_000; i++) {
      const arr = bindingsArrayHelper.getArrayOfPointersByValue();
      void arr[0]?.value;
    }

    const heapAfter = (csp as unknown as { HEAPU8: Uint8Array }).HEAPU8.byteLength;
    expect(heapAfter).toBe(heapBefore);
  });

  /*
   * Array<T> memory tests.
   * We've gone to some effort to allow Array<T>'s (and other containers) that come out of CSP
   * to be declared with `using`, and get automatically deallocated at scope exit. 
   */

  it('Using on a returned array of handles releases every element at scope exit', () => {
    using bindingsArrayHelper = csp.BindingsMechanismsTestType.create();
    using elem1 = csp.BindingsTestType.create(1, 'one');
    using elem2 = csp.BindingsTestType.create(2, 'two');
    bindingsArrayHelper.setArrayFullTypeByValue([elem1, elem2]);

    const beforeGet = csp.BindingsTestType.aliveCount;
    {
      using arr = bindingsArrayHelper.getArrayFullTypeByValue();
      expect(csp.BindingsTestType.aliveCount).toBe(beforeGet + 2);
      expect(arr.length).toBe(2);
    }
    // Scope exit, destructors should run.
    expect(csp.BindingsTestType.aliveCount).toBe(beforeGet);
  });

  it('Using on a const-ref returned array of handles releases every element at scope exit', () => {
    /* Consider if this is a smell. We're testing const ref here just because it is a potential
     * shape of the API, but const-ref or not, we always copy every C++ object over the boundary,
     * and thus this deletion is necessary. But should we always be copying? That is the question.
     */

    using bindingsArrayHelper = csp.BindingsMechanismsTestType.create();
    using elem1 = csp.BindingsTestType.create(1, 'one');
    bindingsArrayHelper.setArrayFullTypeByValue([elem1]);

    const beforeGet = csp.BindingsTestType.aliveCount;
    {
      using arr = bindingsArrayHelper.getArrayFullTypeByConstRef();
      expect(csp.BindingsTestType.aliveCount).toBe(beforeGet + 1);
      expect(arr.length).toBe(1);
    }
    // Scope exit, destructors should run.
    expect(csp.BindingsTestType.aliveCount).toBe(beforeGet);
  });

  it('Using on an array of basic types is a tolerated no-op on disposal', () => {
    using bindingsArrayHelper = csp.BindingsMechanismsTestType.create();
    bindingsArrayHelper.setArrayBasicTypeByValue([1, 2, 3]);
    expect(() => {
      using arr = bindingsArrayHelper.getArrayBasicTypeByValue();
      expect(arr.length).toBe(3);
    }).not.toThrow();
  });

  it('Releases array handles when the scope exits via exception', () => {
    using bindingsArrayHelper = csp.BindingsMechanismsTestType.create();
    using elem1 = csp.BindingsTestType.create(1, 'one');
    using elem2 = csp.BindingsTestType.create(2, 'two');
    bindingsArrayHelper.setArrayFullTypeByValue([elem1, elem2]);

    const beforeGet = csp.BindingsTestType.aliveCount;
    expect(() => {
      using arr = bindingsArrayHelper.getArrayFullTypeByValue();
      expect(csp.BindingsTestType.aliveCount).toBe(beforeGet + 2);
      throw new Error('Oh no!');
    }).toThrow('Oh no!');
    expect(csp.BindingsTestType.aliveCount).toBe(beforeGet);
  });

  it('Disposing an empty returned array is a no-op', () => {
    using bindingsArrayHelper = csp.BindingsMechanismsTestType.create();
    bindingsArrayHelper.setArrayFullTypeByValue([]);
    const beforeGet = csp.BindingsTestType.aliveCount;
    {
      using arr = bindingsArrayHelper.getArrayFullTypeByValue();
      expect(csp.BindingsTestType.aliveCount).toBe(beforeGet); //No types in empty array, no alive count increment
      expect(arr.length).toBe(0);
    }
    expect(csp.BindingsTestType.aliveCount).toBe(beforeGet);
  });

  it('Array getter result without `using` leaks until manually disposed', () => {
    using bindingsArrayHelper = csp.BindingsMechanismsTestType.create();
    using elem1 = csp.BindingsTestType.create(1, 'one');
    bindingsArrayHelper.setArrayFullTypeByValue([elem1]);

    const beforeGet = csp.BindingsTestType.aliveCount;
    const leaked = bindingsArrayHelper.getArrayFullTypeByValue();
    expect(csp.BindingsTestType.aliveCount).toBe(beforeGet + 1);
    // Symbol.dispose is reachable and cleans up.
    leaked[Symbol.dispose]();
    expect(csp.BindingsTestType.aliveCount).toBe(beforeGet);
  });

  it('Disposing a returned array does not affect the underlying C++ storage', () => {
    using bindingsArrayHelper = csp.BindingsMechanismsTestType.create();
    using elem1 = csp.BindingsTestType.create(1, 'one');
    bindingsArrayHelper.setArrayFullTypeByValue([elem1]);

    const beforeGet = csp.BindingsTestType.aliveCount;
    { using arr = bindingsArrayHelper.getArrayFullTypeByValue(); }
    expect(csp.BindingsTestType.aliveCount).toBe(beforeGet);
    // Storage survives despite the scope exit — we can fetch again.
    using arr2 = bindingsArrayHelper.getArrayFullTypeByValue();
    expect(arr2[0]!.value).toBe(1);
  });

  it('Pre-deleting an element in a returned array does not break dispose', () => {
    using bindingsArrayHelper = csp.BindingsMechanismsTestType.create();
    using elem1 = csp.BindingsTestType.create(1, 'one');
    using elem2 = csp.BindingsTestType.create(2, 'two');
    bindingsArrayHelper.setArrayFullTypeByValue([elem1, elem2]);

    const beforeGet = csp.BindingsTestType.aliveCount;
    expect(() => {
      using arr = bindingsArrayHelper.getArrayFullTypeByValue();
      arr[0]!.delete();
    }).not.toThrow();
    expect(csp.BindingsTestType.aliveCount).toBe(beforeGet);

    // The returned array is a copy, so explicitly deleting an element must not
    // touch the underlying C++ storage — re-fetching still yields both elements.
    using arr2 = bindingsArrayHelper.getArrayFullTypeByValue();
    expect(arr2.length).toBe(2);
    expect(arr2[0]!.value).toBe(1);
    expect(arr2[1]!.value).toBe(2);
  });

  it('disposeArray is directly callable as a manual alternative to using', () => {
    using bindingsArrayHelper = csp.BindingsMechanismsTestType.create();
    using elem1 = csp.BindingsTestType.create(1, 'one');
    bindingsArrayHelper.setArrayFullTypeByValue([elem1]);

    const beforeGet = csp.BindingsTestType.aliveCount;
    const arr = bindingsArrayHelper.getArrayFullTypeByValue();
    expect(csp.BindingsTestType.aliveCount).toBe(beforeGet + 1);
    csp.disposeArray(arr);
    expect(csp.BindingsTestType.aliveCount).toBe(beforeGet);
  });

  it('disposeArray on a plain JS array of primitives is tolerated', () => {
    expect(() => csp.disposeArray([1, 2, 3])).not.toThrow();
    expect(() => csp.disposeArray([])).not.toThrow();
  });

  it('disposeArray recurses into nested arrays of handles', () => {
    const a = csp.BindingsTestType.create(1, 'one');
    const b = csp.BindingsTestType.create(2, 'two');
    const before = csp.BindingsTestType.aliveCount;
    csp.disposeArray([[a, b]]);
    expect(csp.BindingsTestType.aliveCount).toBe(before - 2);
  });

  it('disposeArray throws on non-array input', () => {
    expect(() => csp.disposeArray(42)).toThrow();
    expect(() => csp.disposeArray('nope')).toThrow();
    expect(() => csp.disposeArray({})).toThrow();
  });

  it('Cpp Objects accessible via pointer arrays without allocating', () => {
     using bindingsArrayHelper = csp.BindingsMechanismsTestType.create();
     const beforeAliveCount = csp.BindingsTestType.aliveCount;

     let pointerArray = bindingsArrayHelper.getArrayOfCppOwnedPointers();
     expect(pointerArray.length).toBe(2);
     expect(pointerArray[0]?.name).toBe("One");
     expect(pointerArray[1]?.value).toBe(2);
     expect(csp.BindingsTestType.aliveCount).toBe(beforeAliveCount);
  });

  it('JS owned object in pointer array that falls out of scope is undefined', () => {
    using bindingsArrayHelper = csp.BindingsMechanismsTestType.create();
    const beforeAliveCount = csp.BindingsTestType.aliveCount;

    {
      using elem1 = csp.BindingsTestType.create(1, "one");
      const newArr = [elem1]

      bindingsArrayHelper.setArrayOfPointersByValue(newArr);
      //elem1 falls out of scope, C++ array now holds a dangling pointer. Probably bad behavior from the JS developer :P
    }

    let roundTripArr = bindingsArrayHelper.getArrayOfPointersByValue();

    expect(roundTripArr.length).toBe(1);
    //Actually accessing the dangling pointer would be non-deterministic UB, so, we'll just use aliveCount
    //Elem1 is disposed, despite the array length still being 1.
    expect(csp.BindingsTestType.aliveCount).toBe(beforeAliveCount);
  });

  it('Explicit delete of elements in pointer array deletes underlying C++ memory', () => {
    using bindingsArrayHelper = csp.BindingsMechanismsTestType.create();
    const beforeAliveCount = csp.BindingsTestType.aliveCount;

    // No `using` here: we're managing lifetime explicitly via .delete() on the
    // round-trip handles below, to observe what that does to AliveCount.
    const elem1 = csp.BindingsTestType.create(1, "one");
    const elem2 = csp.BindingsTestType.create(2, "two");
    expect(csp.BindingsTestType.aliveCount).toBe(beforeAliveCount + 2);

    bindingsArrayHelper.setArrayOfPointersByValue([elem1, elem2]);
    let pointerArray = bindingsArrayHelper.getArrayOfPointersByValue();

    pointerArray[0]!.delete();
    expect(csp.BindingsTestType.aliveCount).toBe(beforeAliveCount + 1);

    pointerArray[1]!.delete();
    expect(csp.BindingsTestType.aliveCount).toBe(beforeAliveCount);

    // elem1 / elem2 are now dangling JS handles — their C++ objects were
    // destroyed via the round-trip handles. Don't dispose or .delete() them.
  });

});

