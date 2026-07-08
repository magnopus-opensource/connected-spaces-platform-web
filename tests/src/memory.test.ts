import { describe, it, expect, beforeAll } from 'vitest';
import { loadCSP } from '../loadModule';
import createModule, { type MainModule } from 'connected-spaces-platform-bindings';

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
    expect(() => t.delete()).toThrow('BindingsTestType instance already deleted');
  });

  it('Throws when accessing a deleted handle', () => {
    const t = csp.BindingsTestType.create(42, 'hello');
    t.delete();
    expect(() => t.value).toThrow(
      'cannot call emscripten binding method BindingsTestType.value getter on deleted object'
    );
  });

  /*
   * This test is very important for our non-owning pointers in array strategy.
   * If we can rely on the embind proxies being GC'd, we don't need to do anything
   * very fancy to delete them on disposal. We can just not call delete, and thus
   * not need to attach a dispose Symbol, to non-owning pointer arrays.
   */
  it('reference-returned pointer proxies are eligible for GC when dropped', async () => {
    using bindingsArrayHelper = csp.ContainerBindingMechanismsTestType.create();
    using anchor = csp.BindingsTestType.create(1, 'one');
    bindingsArrayHelper.setArrayOfPointersByValue([anchor]);

    // FinalizationRegistry callbacks fire when the registered object is GC'd.
    // If proxies were pinned by some embind internal, ~zero callbacks would fire.
    // If they're GC-eligible, most will fire under memory pressure.
    let collected = 0;
    const registry = new FinalizationRegistry(() => {
      collected++;
    });
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
    await new Promise((r) => setTimeout(r, 100));
    await new Promise((r) => setTimeout(r, 0));

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
    using bindingsArrayHelper = csp.ContainerBindingMechanismsTestType.create();
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
  it('reference-returned proxies do not grow wasm heap', async () => {
    // The following is based on the Emscripten compiler settings at the time of writing
    // (see CMakeLists.txt).
    // If those ever change this test may need to be updated to remain valid:
    // - INITIAL_MEMORY=32MB
    // - ALLOW_MEMORY_GROWTH=1 (enabled)
    // - MEMORY_GROWTH_GEOMETRIC_STEP unset (defaults to 0.20)
    // - MEMORY_GROWTH_LINEAR_STEP unset (growth is geometric by default)
    // - MALLOC unset (defaults to dlmalloc)
    // (Documentation: https://emscripten.org/docs/tools_reference/settings_reference.html)

    // Use a fresh module instance so the heap starts at exactly INITIAL_MEMORY.
    // This guarantees the sensitivity analysis below holds regardless of what other
    // tests have already allocated in the shared csp instance.
    const freshCsp = await createModule();

    using bindingsArrayHelper = freshCsp.ContainerBindingMechanismsTestType.create();
    using anchor = freshCsp.BindingsTestType.create(1, 'one');
    bindingsArrayHelper.setArrayOfPointersByValue([anchor]);

    // Leak large value-copied allocations until the heap is forced to grow, then take the
    // baseline. This bounds how much free space the pointer loop has to absorb in order to detect
    // any leaks.
    //
    // Using an initial memory size of 32MB and a geometric growth step of 0.20, the first growth
    // event will expand the heap by 32MB * 0.20 = 6.4MB, rounded up to the next 64KB WASM page
    // boundary so ~6.44MB.
    // With 500k iterations we can detect leaks of ~6.44MB / 500k = ~13.5 bytes per call.
    // dlmalloc's (Emscripten's default malloc) minimum chunk size is ~24 bytes, so this catches any
    // non-trivial WASM heap allocation that isn't freed - including the transient Array<T*> backing
    // buffer (~20 bytes) if embind ever failed to destroy it.

    // 512 KB string
    const bigString = 'x'.repeat(512 * 1024);
    using filler = freshCsp.BindingsTestType.create(0, bigString);
    bindingsArrayHelper.setArrayFullTypeByValue([filler]);

    // Leak memory until the heap grows
    let sentinel = (freshCsp as unknown as { HEAPU8: Uint8Array }).HEAPU8.byteLength;
    while ((freshCsp as unknown as { HEAPU8: Uint8Array }).HEAPU8.byteLength === sentinel) {
      const arr = bindingsArrayHelper.getArrayFullTypeByValue();
      void arr[0]?.value;
      // Intentional leak - we want these to accumulate and force a heap growth.
    }

    // Heap has just grown - take the baseline measurement.
    const heapBefore = (freshCsp as unknown as { HEAPU8: Uint8Array }).HEAPU8.byteLength;

    for (let i = 0; i < 500_000; i++) {
      const arr = bindingsArrayHelper.getArrayOfPointersByValue();
      void arr[0]?.value;
    }

    const heapAfter = (freshCsp as unknown as { HEAPU8: Uint8Array }).HEAPU8.byteLength;
    expect(heapAfter).toBe(heapBefore);
  });

  /*
   * This test exists primarily to verify the assumptions in the other heap growth tests given the
   * Emscripten compiler settings used to build the library (see CMakeLists.txt).
   * It proves the sensitivity of the heap growth detection in the previous test, and that the heap
   * does in fact grow as we expect when we leak value-copied objects.
   */
  it('value-returned arrays do grow wasm heap', async () => {
    // Use a fresh module instance
    const freshCsp = await createModule();

    using bindingsArrayHelper = freshCsp.ContainerBindingMechanismsTestType.create();
    // 512 KB string
    const bigString = 'x'.repeat(512 * 1024);
    using filler = freshCsp.BindingsTestType.create(0, bigString);
    bindingsArrayHelper.setArrayFullTypeByValue([filler]);

    let sentinel = (freshCsp as unknown as { HEAPU8: Uint8Array }).HEAPU8.byteLength;
    expect(sentinel).toBe(32 * 1024 * 1024); // INITIAL_MEMORY = 32MB

    // Leak memory until the heap grows
    while ((freshCsp as unknown as { HEAPU8: Uint8Array }).HEAPU8.byteLength === sentinel) {
      const arr = bindingsArrayHelper.getArrayFullTypeByValue();
      void arr[0]?.value;
      // Intentional leak - we want these to accumulate and force a heap growth.
    }

    // Heap has just grown - take the baseline measurement.
    const heapBefore = (freshCsp as unknown as { HEAPU8: Uint8Array }).HEAPU8.byteLength;

    // Check it has grown by the expected amount.
    // Emscripten targets INITIAL_MEMORY * (1 + GEOMETRIC_STEP) = 32MB * 1.2 = 38.4MB,
    // then rounds up to the next 64KB WASM page boundary.
    const wasmPageSize = 65536;
    const growthTarget = (32 + 32 * 0.2) * 1024 * 1024; // 38.4MB
    const expectedHeapAfterFirstGrowth = Math.ceil(growthTarget / wasmPageSize) * wasmPageSize; // 40304640
    expect(heapBefore).toBe(expectedHeapAfterFirstGrowth);

    const string = 'x'.repeat(1024);
    using anchor = freshCsp.BindingsTestType.create(1, string);
    bindingsArrayHelper.setArrayFullTypeByValue([anchor]);

    // 10000 calls × 1KB = 10 MB of undisposed C++ allocations
    for (let i = 0; i < 10_000; i++) {
      const arr = bindingsArrayHelper.getArrayFullTypeByValue();
      void arr[0]?.value;
    }

    const heapAfter = (freshCsp as unknown as { HEAPU8: Uint8Array }).HEAPU8.byteLength;
    expect(heapAfter).toBeGreaterThan(heapBefore);
  });

  /*
   * Array<T> memory tests.
   * We've gone to some effort to allow Array<T>'s (and other containers) that come out of CSP
   * to be declared with `using`, and get automatically deallocated at scope exit.
   */

  it('Using on a returned array of handles releases every element at scope exit', () => {
    using bindingsArrayHelper = csp.ContainerBindingMechanismsTestType.create();
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

    using bindingsArrayHelper = csp.ContainerBindingMechanismsTestType.create();
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
    using bindingsArrayHelper = csp.ContainerBindingMechanismsTestType.create();
    bindingsArrayHelper.setArrayBasicTypeByValue([1, 2, 3]);
    expect(() => {
      using arr = bindingsArrayHelper.getArrayBasicTypeByValue();
      expect(arr.length).toBe(3);
    }).not.toThrow();
  });

  it('Releases array handles when the scope exits via exception', () => {
    using bindingsArrayHelper = csp.ContainerBindingMechanismsTestType.create();
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
    using bindingsArrayHelper = csp.ContainerBindingMechanismsTestType.create();
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
    using bindingsArrayHelper = csp.ContainerBindingMechanismsTestType.create();
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
    using bindingsArrayHelper = csp.ContainerBindingMechanismsTestType.create();
    using elem1 = csp.BindingsTestType.create(1, 'one');
    bindingsArrayHelper.setArrayFullTypeByValue([elem1]);

    const beforeGet = csp.BindingsTestType.aliveCount;
    {
      using arr = bindingsArrayHelper.getArrayFullTypeByValue();
    }
    expect(csp.BindingsTestType.aliveCount).toBe(beforeGet);
    // Storage survives despite the scope exit — we can fetch again.
    using arr2 = bindingsArrayHelper.getArrayFullTypeByValue();
    expect(arr2[0]!.value).toBe(1);
  });

  it('Pre-deleting an element in a returned array does not break dispose', () => {
    using bindingsArrayHelper = csp.ContainerBindingMechanismsTestType.create();
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
    using bindingsArrayHelper = csp.ContainerBindingMechanismsTestType.create();
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
    // @ts-expect-error: intentionally testing non-array input
    expect(() => csp.disposeArray(42)).toThrow();

    // @ts-expect-error: intentionally testing non-array input
    expect(() => csp.disposeArray('nope')).toThrow();

    // @ts-expect-error: intentionally testing non-array input
    expect(() => csp.disposeArray({})).toThrow();
  });

  it('Cpp Objects accessible via pointer arrays without allocating', () => {
    using bindingsArrayHelper = csp.ContainerBindingMechanismsTestType.create();
    const beforeAliveCount = csp.BindingsTestType.aliveCount;

    let pointerArray = bindingsArrayHelper.getArrayOfCppOwnedPointers();
    expect(pointerArray.length).toBe(2);
    expect(pointerArray[0]?.name).toBe('One');
    expect(pointerArray[1]?.value).toBe(2);
    expect(csp.BindingsTestType.aliveCount).toBe(beforeAliveCount);
  });

  it('JS owned object in pointer array that falls out of scope is undefined', () => {
    using bindingsArrayHelper = csp.ContainerBindingMechanismsTestType.create();
    const beforeAliveCount = csp.BindingsTestType.aliveCount;

    {
      using elem1 = csp.BindingsTestType.create(1, 'one');
      const newArr = [elem1];

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
    using bindingsArrayHelper = csp.ContainerBindingMechanismsTestType.create();
    const beforeAliveCount = csp.BindingsTestType.aliveCount;

    // No `using` here: we're managing lifetime explicitly via .delete() on the
    // round-trip handles below, to observe what that does to AliveCount.
    const elem1 = csp.BindingsTestType.create(1, 'one');
    const elem2 = csp.BindingsTestType.create(2, 'two');
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

  /*
   * Map<K, V> memory tests.
   * These mirror the Array<T> memory tests above — Maps that come out of CSP are likewise
   * `using`-enabled and route through disposeMap at scope exit. The key difference is that
   * disposeMap disposes the map's *values* only; keys are constrained to primitives and so
   * own no C++ memory (see the keys-untouched test below).
   */

  it('Using on a returned map of handles releases every value at scope exit', () => {
    using bindingsMapHelper = csp.ContainerBindingMechanismsTestType.create();
    using elem1 = csp.BindingsTestType.create(1, 'one');
    using elem2 = csp.BindingsTestType.create(2, 'two');
    bindingsMapHelper.setMapFullTypeByValue(
      new Map([
        [1, elem1],
        [2, elem2]
      ])
    );

    const beforeGet = csp.BindingsTestType.aliveCount;
    {
      using map = bindingsMapHelper.getMapFullTypeByValue();
      expect(csp.BindingsTestType.aliveCount).toBe(beforeGet + 2);
      expect(map.size).toBe(2);
    }
    // Scope exit, destructors should run.
    expect(csp.BindingsTestType.aliveCount).toBe(beforeGet);
  });

  it('Using on a const-ref returned map of handles releases every value at scope exit', () => {
    using bindingsMapHelper = csp.ContainerBindingMechanismsTestType.create();
    using elem1 = csp.BindingsTestType.create(1, 'one');
    bindingsMapHelper.setMapFullTypeByValue(new Map([[1, elem1]]));

    const beforeGet = csp.BindingsTestType.aliveCount;
    {
      using map = bindingsMapHelper.getMapFullTypeByConstRef();
      expect(csp.BindingsTestType.aliveCount).toBe(beforeGet + 1);
      expect(map.size).toBe(1);
    }
    // Scope exit, destructors should run.
    expect(csp.BindingsTestType.aliveCount).toBe(beforeGet);
  });

  it('Using on a map of basic types is a tolerated no-op on disposal', () => {
    using bindingsMapHelper = csp.ContainerBindingMechanismsTestType.create();
    bindingsMapHelper.setMapBasicTypeByValue(
      new Map([
        [1, 10],
        [2, 20],
        [3, 30]
      ])
    );
    expect(() => {
      using map = bindingsMapHelper.getMapBasicTypeByValue();
      expect(map.size).toBe(3);
    }).not.toThrow();
  });

  it('Releases map handles when the scope exits via exception', () => {
    using bindingsMapHelper = csp.ContainerBindingMechanismsTestType.create();
    using elem1 = csp.BindingsTestType.create(1, 'one');
    using elem2 = csp.BindingsTestType.create(2, 'two');
    bindingsMapHelper.setMapFullTypeByValue(
      new Map([
        [1, elem1],
        [2, elem2]
      ])
    );

    const beforeGet = csp.BindingsTestType.aliveCount;
    expect(() => {
      using map = bindingsMapHelper.getMapFullTypeByValue();
      expect(csp.BindingsTestType.aliveCount).toBe(beforeGet + 2);
      throw new Error('Oh no!');
    }).toThrow('Oh no!');
    expect(csp.BindingsTestType.aliveCount).toBe(beforeGet);
  });

  it('Disposing an empty returned map is a no-op', () => {
    using bindingsMapHelper = csp.ContainerBindingMechanismsTestType.create();
    bindingsMapHelper.setMapFullTypeByValue(new Map());
    const beforeGet = csp.BindingsTestType.aliveCount;
    {
      using map = bindingsMapHelper.getMapFullTypeByValue();
      expect(csp.BindingsTestType.aliveCount).toBe(beforeGet); // No values in empty map, no alive count increment
      expect(map.size).toBe(0);
    }
    expect(csp.BindingsTestType.aliveCount).toBe(beforeGet);
  });

  it('Map getter result without `using` leaks until manually disposed', () => {
    using bindingsMapHelper = csp.ContainerBindingMechanismsTestType.create();
    using elem1 = csp.BindingsTestType.create(1, 'one');
    bindingsMapHelper.setMapFullTypeByValue(new Map([[1, elem1]]));

    const beforeGet = csp.BindingsTestType.aliveCount;
    const leaked = bindingsMapHelper.getMapFullTypeByValue();
    expect(csp.BindingsTestType.aliveCount).toBe(beforeGet + 1);
    // Symbol.dispose is reachable and cleans up.
    leaked[Symbol.dispose]();
    expect(csp.BindingsTestType.aliveCount).toBe(beforeGet);
  });

  it('Disposing a returned map does not affect the underlying C++ storage', () => {
    using bindingsMapHelper = csp.ContainerBindingMechanismsTestType.create();
    using elem1 = csp.BindingsTestType.create(1, 'one');
    bindingsMapHelper.setMapFullTypeByValue(new Map([[1, elem1]]));

    const beforeGet = csp.BindingsTestType.aliveCount;
    {
      using map = bindingsMapHelper.getMapFullTypeByValue();
    }
    expect(csp.BindingsTestType.aliveCount).toBe(beforeGet);
    // Storage survives despite the scope exit — we can fetch again.
    using map2 = bindingsMapHelper.getMapFullTypeByValue();
    expect(map2.get(1)!.value).toBe(1);
  });

  it('Pre-deleting a value in a returned map does not break dispose', () => {
    using bindingsMapHelper = csp.ContainerBindingMechanismsTestType.create();
    using elem1 = csp.BindingsTestType.create(1, 'one');
    using elem2 = csp.BindingsTestType.create(2, 'two');
    bindingsMapHelper.setMapFullTypeByValue(
      new Map([
        [1, elem1],
        [2, elem2]
      ])
    );

    const beforeGet = csp.BindingsTestType.aliveCount;
    expect(() => {
      using map = bindingsMapHelper.getMapFullTypeByValue();
      map.get(1)!.delete();
    }).not.toThrow();
    expect(csp.BindingsTestType.aliveCount).toBe(beforeGet);

    // The returned map is a copy, so explicitly deleting a value must not touch the
    // underlying C++ storage — re-fetching still yields both entries.
    using map2 = bindingsMapHelper.getMapFullTypeByValue();
    expect(map2.size).toBe(2);
    expect(map2.get(1)!.value).toBe(1);
    expect(map2.get(2)!.value).toBe(2);
  });

  it('disposeMap is directly callable as a manual alternative to using', () => {
    using bindingsMapHelper = csp.ContainerBindingMechanismsTestType.create();
    using elem1 = csp.BindingsTestType.create(1, 'one');
    bindingsMapHelper.setMapFullTypeByValue(new Map([[1, elem1]]));

    const beforeGet = csp.BindingsTestType.aliveCount;
    const map = bindingsMapHelper.getMapFullTypeByValue();
    expect(csp.BindingsTestType.aliveCount).toBe(beforeGet + 1);
    csp.disposeMap(map);
    expect(csp.BindingsTestType.aliveCount).toBe(beforeGet);
  });

  it('disposeMap on a plain JS Map of primitives is tolerated', () => {
    expect(() =>
      csp.disposeMap(
        new Map([
          [1, 10],
          [2, 20]
        ])
      )
    ).not.toThrow();
    expect(() => csp.disposeMap(new Map())).not.toThrow();
  });

  it('disposeMap disposes handles nested in array values', () => {
    const a = csp.BindingsTestType.create(1, 'one');
    const b = csp.BindingsTestType.create(2, 'two');
    const before = csp.BindingsTestType.aliveCount;
    // Value disposal routes through Array.from(map.values()) then the array walker, so it
    // recurses into array values just like disposeArray recurses into nested arrays.
    csp.disposeMap(new Map([[1, [a, b]]]));
    expect(csp.BindingsTestType.aliveCount).toBe(before - 2);
  });

  it('disposeMap throws on non-Map input', () => {
    // @ts-expect-error: intentionally testing non-map input
    expect(() => csp.disposeMap(42)).toThrow();

    // @ts-expect-error: intentionally testing non-map input
    expect(() => csp.disposeMap('nope')).toThrow();

    // @ts-expect-error: intentionally testing non-map input
    expect(() => csp.disposeMap({})).toThrow();

    // @ts-expect-error: intentionally testing non-map input
    expect(() => csp.disposeMap([])).toThrow(); // arrays are not Maps
  });

  it('disposeMap disposes values but leaves keys untouched', () => {
    // Real bindings constrain keys to primitives, but disposeMap takes `any`, so we can
    // hand it a Map with a bound handle as *both* key and value to prove only the value
    // is disposed. This is the core contract of disposeMap.
    const keyHandle = csp.BindingsTestType.create(1, 'key');
    const valueHandle = csp.BindingsTestType.create(2, 'value');
    const before = csp.BindingsTestType.aliveCount;

    csp.disposeMap(new Map([[keyHandle, valueHandle]]));

    // Exactly one disposal: the value. The key handle is spared.
    expect(csp.BindingsTestType.aliveCount).toBe(before - 1);
    expect(keyHandle.isDeleted()).toBe(false);
    expect(valueHandle.isDeleted()).toBe(true);

    keyHandle.delete(); // Clean up the key we deliberately kept alive.
  });

  it('Cpp objects accessible via pointer maps without allocating', () => {
    using bindingsMapHelper = csp.ContainerBindingMechanismsTestType.create();
    const beforeAliveCount = csp.BindingsTestType.aliveCount;

    let pointerMap = bindingsMapHelper.getMapOfCppOwnedPointers();
    expect(pointerMap.size).toBe(2);
    expect(pointerMap.get(1)?.name).toBe('One');
    expect(pointerMap.get(2)?.value).toBe(2);
    expect(csp.BindingsTestType.aliveCount).toBe(beforeAliveCount);
  });

  it('JS owned object in pointer map that falls out of scope is undefined', () => {
    using bindingsMapHelper = csp.ContainerBindingMechanismsTestType.create();
    const beforeAliveCount = csp.BindingsTestType.aliveCount;

    {
      using elem1 = csp.BindingsTestType.create(1, 'one');
      bindingsMapHelper.setMapOfPointersByValue(new Map([[1, elem1]]));
      // elem1 falls out of scope, C++ map now holds a dangling pointer. Probably bad behavior from the JS developer :P
    }

    let roundTripMap = bindingsMapHelper.getMapOfPointersByValue();

    expect(roundTripMap.size).toBe(1);
    // Actually accessing the dangling pointer would be non-deterministic UB, so we'll just use aliveCount.
    // elem1 is disposed, despite the map size still being 1.
    expect(csp.BindingsTestType.aliveCount).toBe(beforeAliveCount);
  });

  it('Explicit delete of values in pointer map deletes underlying C++ memory', () => {
    using bindingsMapHelper = csp.ContainerBindingMechanismsTestType.create();
    const beforeAliveCount = csp.BindingsTestType.aliveCount;

    // No `using` here: we're managing lifetime explicitly via .delete() on the
    // round-trip handles below, to observe what that does to AliveCount.
    const elem1 = csp.BindingsTestType.create(1, 'one');
    const elem2 = csp.BindingsTestType.create(2, 'two');
    expect(csp.BindingsTestType.aliveCount).toBe(beforeAliveCount + 2);

    bindingsMapHelper.setMapOfPointersByValue(
      new Map([
        [1, elem1],
        [2, elem2]
      ])
    );
    let pointerMap = bindingsMapHelper.getMapOfPointersByValue();

    pointerMap.get(1)!.delete();
    expect(csp.BindingsTestType.aliveCount).toBe(beforeAliveCount + 1);

    pointerMap.get(2)!.delete();
    expect(csp.BindingsTestType.aliveCount).toBe(beforeAliveCount);

    // elem1 / elem2 are now dangling JS handles — their C++ objects were
    // destroyed via the round-trip handles. Don't dispose or .delete() them.
  });
});
