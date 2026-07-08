import { describe, it, expect, beforeAll } from 'vitest';
import { loadCSP } from '../loadModule';
import createModule, { type MainModule } from 'connected-spaces-platform-bindings';

/*
 * Tests for the binding of CSP common::String, using internal binding test types.
 */

describe('String bindings', () => {
  let csp: MainModule;

  beforeAll(async () => {
    csp = await loadCSP();
  });

  it('String round trip by value', () => {
    using helper = csp.ContainerBindingMechanismsTestType.create();
    const testString = 'Hello, world, by value!';

    helper.setCspStringByValue(testString);
    const roundTrip: string = helper.getCspStringByValue();

    expect(testString).toBe(roundTrip);
  });

  it('String round trip by const reference', () => {
    using helper = csp.ContainerBindingMechanismsTestType.create();
    const testString = 'Hello, world, by const reference!';

    helper.setCspStringByConstRef(testString);
    const roundTrip: string = helper.getCspStringByConstRef();

    expect(testString).toBe(roundTrip);
  });

  it('String round trip by value and const reference are equal', () => {
    using helper = csp.ContainerBindingMechanismsTestType.create();
    const testString = 'Hello, world, by const reference!';

    helper.setCspStringByValue(testString);
    const roundTripByValue: string = helper.getCspStringByValue();

    helper.setCspStringByConstRef(testString);
    const roundTripByConstRef: string = helper.getCspStringByConstRef();

    expect(roundTripByValue).toBe(roundTripByConstRef);
  });

  it('String replacement returns updated value', () => {
    using helper = csp.ContainerBindingMechanismsTestType.create();

    const testString1 = 'Hello, world!';
    helper.setCspStringByConstRef(testString1);
    const roundTrip: string = helper.getCspStringByConstRef();

    const testStringUpdated2 = 'Hello, universe!';
    helper.setCspStringByConstRef(testStringUpdated2);
    const roundTripUpdated: string = helper.getCspStringByConstRef();

    expect(testStringUpdated2).toBe(roundTripUpdated);
    expect(roundTrip).not.toBe(roundTripUpdated);
  });

  it('String with unicode round trip', () => {
    using helper = csp.ContainerBindingMechanismsTestType.create();
    const testString = 'впечатляващо! すばらしい! 🚀';

    helper.setCspStringByValue(testString);
    const roundTrip: string = helper.getCspStringByValue();

    helper.setCspStringByConstRef(testString);
    const roundTripConstRef: string = helper.getCspStringByConstRef();

    expect(testString.length).toBe(roundTrip.length);
    expect(testString).toBe(roundTrip);

    expect(testString.length).toBe(roundTripConstRef.length);
    expect(testString).toBe(roundTripConstRef);
  });

  it('String with null character round trip', () => {
    using helper = csp.ContainerBindingMechanismsTestType.create();
    const testString = 'This should not be \0 truncated!';

    helper.setCspStringByValue(testString);
    const roundTrip: string = helper.getCspStringByValue();

    expect(testString.length).toBe(roundTrip.length);
    expect(testString).toBe(roundTrip);
  });

  it('String with only null characters round trip', () => {
    using helper = csp.ContainerBindingMechanismsTestType.create();
    const testString = '\0'.repeat(10);

    helper.setCspStringByValue(testString);
    const roundTrip: string = helper.getCspStringByValue();

    expect(testString.length).toBe(roundTrip.length);
    expect(testString).toBe(roundTrip);
  });

  it('Empty string round trip by value', () => {
    using helper = csp.ContainerBindingMechanismsTestType.create();
    const testString = '';

    helper.setCspStringByValue(testString);
    const roundTrip: string = helper.getCspStringByValue();

    expect(testString.length).toBe(roundTrip.length);
    expect(testString).toBe(roundTrip);
  });

  it('String undefined round trip by value', () => {
    using helper = csp.ContainerBindingMechanismsTestType.create();
    const testStringUndefined = 'undefined';

    helper.setCspStringByValue(testStringUndefined);
    const roundTrip: string = helper.getCspStringByValue();

    expect(testStringUndefined.length).toBe(roundTrip.length);
    expect(testStringUndefined).toBe(roundTrip);
  });

  it('Long string round trip', () => {
    using helper = csp.ContainerBindingMechanismsTestType.create();
    // 70KB string - There was a stack size of 64KB in the previous CSP wrapper generator.
    // Ensure that we can handle strings larger than that.
    const testString = 'abcdefghij'.repeat(7 * 1024);

    helper.setCspStringByValue(testString);
    const roundTrip: string = helper.getCspStringByValue();

    expect(testString.length).toBe(roundTrip.length);
    expect(testString).toBe(roundTrip);
  });

  it('Setting null or undefined string throws', () => {
    using helper = csp.ContainerBindingMechanismsTestType.create();

    // @ts-expect-error - Typescript will not allow us to set null but want to see what happens anyways
    expect(() => helper.setCspStringByValue(null)).toThrow('Cannot pass non-string to std::string');
    // @ts-expect-error
    expect(() => helper.setCspStringByValue(null)).toThrow('Cannot pass non-string to std::string');

    // @ts-expect-error - Typescript will not allow us to set undefined but want to see what happens anyways
    expect(() => helper.setCspStringByValue(undefined)).toThrow('Cannot pass non-string to std::string');
    // @ts-expect-error
    expect(() => helper.setCspStringByValue(undefined)).toThrow('Cannot pass non-string to std::string');
  });

  it('Setting non-string values throws', () => {
    using helper = csp.ContainerBindingMechanismsTestType.create();

    // @ts-expect-error - Typescript will not allow us to set non-string values but want to see what happens anyways
    expect(() => helper.setCspStringByValue(123)).toThrow('Cannot pass non-string to std::string');
    // @ts-expect-error
    expect(() => helper.setCspStringByValue(123)).toThrow('Cannot pass non-string to std::string');

    // @ts-expect-error
    expect(() => helper.setCspStringByValue(true)).toThrow('Cannot pass non-string to std::string');
    // @ts-expect-error
    expect(() => helper.setCspStringByValue(true)).toThrow('Cannot pass non-string to std::string');

    // @ts-expect-error
    expect(() => helper.setCspStringByValue({ x: 1 })).toThrow('Cannot pass non-string to std::string');
    // @ts-expect-error
    expect(() => helper.setCspStringByValue({ x: 1 })).toThrow('Cannot pass non-string to std::string');
  });

  // Array of string tests

  it('Array of string round trip by value', () => {
    using helper = csp.ContainerBindingMechanismsTestType.create();
    const testArray = ['alpha', 'beta', 'gamma'];

    helper.setArrayStringByValue(testArray);
    using roundTrip = helper.getArrayStringByValue();

    expect(csp.arrayEquals(roundTrip, testArray)).toBe(true);
  });

  it('Array of string round trip by const reference', () => {
    using helper = csp.ContainerBindingMechanismsTestType.create();
    const testArray = ['hello', 'world'];

    helper.setArrayStringByConstRef(testArray);
    using roundTrip = helper.getArrayStringByConstRef();

    expect(csp.arrayEquals(roundTrip, testArray)).toBe(true);
  });

  it('Array of string empty array round trip', () => {
    using helper = csp.ContainerBindingMechanismsTestType.create();
    const testArray: string[] = [];

    helper.setArrayStringByValue(testArray);
    using roundTrip = helper.getArrayStringByValue();

    expect(csp.arrayEquals(roundTrip, testArray)).toBe(true);
  });

  // Map with string tests

  it('Map with string keys round trip by value', () => {
    using helper = csp.ContainerBindingMechanismsTestType.create();
    const testMap = new Map<string, number>([
      ['key1', 1],
      ['key2', 2]
    ]);

    helper.setMapStringIntByValue(testMap);
    using roundTrip = helper.getMapStringIntByValue();

    expect(csp.mapEquals(roundTrip, testMap)).toBe(true);
  });

  it('Map with string keys round trip by const reference', () => {
    using helper = csp.ContainerBindingMechanismsTestType.create();
    const testMap = new Map<string, number>([
      ['key1', 1],
      ['key2', 2]
    ]);

    helper.setMapStringIntByConstRef(testMap);
    using roundTrip = helper.getMapStringIntByConstRef();

    expect(csp.mapEquals(roundTrip, testMap)).toBe(true);
  });

  it('Map with empty string key round trip', () => {
    using helper = csp.ContainerBindingMechanismsTestType.create();
    const testMap = new Map<string, number>([
      ['', 1],
      ['key2', 2]
    ]);

    helper.setMapStringIntByValue(testMap);
    using roundTrip = helper.getMapStringIntByValue();

    expect(csp.mapEquals(roundTrip, testMap)).toBe(true);
  });

  it('Map with string values round trip by value', () => {
    using helper = csp.ContainerBindingMechanismsTestType.create();
    const testMap = new Map<string, string>([
      ['key1', 'value1'],
      ['key2', 'value2']
    ]);

    helper.setMapStringStringByValue(testMap);
    using roundTrip = helper.getMapStringStringByValue();

    expect(csp.mapEquals(roundTrip, testMap)).toBe(true);
  });

  it('Map with string values round trip by const reference', () => {
    using helper = csp.ContainerBindingMechanismsTestType.create();
    const testMap = new Map<string, string>([
      ['key1', 'value1'],
      ['key2', 'value2']
    ]);

    helper.setMapStringStringByConstRef(testMap);
    using roundTrip = helper.getMapStringStringByConstRef();

    expect(csp.mapEquals(roundTrip, testMap)).toBe(true);
  });

  it('Map with string values round trip by const reference', () => {
    using helper = csp.ContainerBindingMechanismsTestType.create();
    const testMap = new Map<string, string>([
      ['key1', 'value1'],
      ['key2', 'value2']
    ]);

    helper.setMapStringStringByConstRef(testMap);
    using roundTrip = helper.getMapStringStringByConstRef();

    expect(csp.mapEquals(roundTrip, testMap)).toBe(true);
  });

  /*
   * Strings are pure-value types with no explicit disposal on the JS side.
   * These tests prove that the underlying C++ memory is properly freed when reassigned or when
   * containers holding strings are destroyed.
   *
   * The following tests are based on the Emscripten compiler settings at the time of writing
   * (see CMakeLists.txt).
   * If those ever change the tests may need to be updated to remain valid:
   *  - INITIAL_MEMORY=32MB
   *  - ALLOW_MEMORY_GROWTH=1 (enabled)
   *  - MEMORY_GROWTH_GEOMETRIC_STEP unset (defaults to 0.20)
   *  - MEMORY_GROWTH_LINEAR_STEP unset (growth is geometric by default)
   * (Documentation: https://emscripten.org/docs/tools_reference/settings_reference.html)
   */
  describe('String memory tests', () => {
    // Each test gets a fresh module instance starting at exactly INITIAL_MEMORY so heap-growth
    // assertions are independent of test ordering and shared module state.
    let freshCsp: MainModule;

    beforeEach(async () => {
      freshCsp = await createModule();
    });

    it('Repeated string sets do not grow wasm heap', () => {
      using helper = freshCsp.ContainerBindingMechanismsTestType.create();

      // Leak large objects until the heap is forced to grow for the first time, then take
      // the baseline.
      //
      // With INITIAL_MEMORY=32MB and MEMORY_GROWTH_GEOMETRIC_STEP=0.20, the first growth
      // target (38.4MB) rounds up to the next 64KB page: 40304640 bytes.
      // Headroom added = 40304640 - 33554432 = 6750208 bytes (~6.44MB).
      // With 50K iterations and a 256-byte string, a per-call leak would accumulate
      // 256 * 50K = 12.8MB >> 6.44MB, guaranteeing heap growth if strings are not freed.
      // Sensitivity: 6750208 / 50K = ~135 bytes/call - any per-call leak of 256 bytes is detected.

      // 512KB string
      const bigPayload = 'x'.repeat(512 * 1024);

      // Leak memory until the heap grows
      let sentinel = (freshCsp as unknown as { HEAPU8: Uint8Array }).HEAPU8.byteLength;
      while ((freshCsp as unknown as { HEAPU8: Uint8Array }).HEAPU8.byteLength === sentinel) {
        freshCsp.BindingsTestType.create(0, bigPayload); // Intentional leak - no using / dispose
      }

      // Heap has just grown - take the baseline measurement.
      const heapBefore = (freshCsp as unknown as { HEAPU8: Uint8Array }).HEAPU8.byteLength;

      // If the previous string isn't freed on reassignment, each 256-byte string leaks and the
      // accumulated ~12.8MB would exceed the ~6.44MB headroom.
      const testString = 'x'.repeat(256);
      for (let i = 0; i < 50_000; i++) {
        helper.setCspStringByValue(testString);
      }

      const heapAfter = (freshCsp as unknown as { HEAPU8: Uint8Array }).HEAPU8.byteLength;
      expect(heapAfter).toBe(heapBefore);
    });

    it('Strings in arrays do not leak when container is disposed', () => {
      // No need to force an initial heap growth:
      // 1K iterations produces ~100MB of potential leaks >> the 32MB initial memory size.
      using helper = freshCsp.ContainerBindingMechanismsTestType.create();
      // 100 strings of ~1KB each
      const largeStrings = Array.from({ length: 100 }, (_, i) => 'x'.repeat(1024) + i);
      helper.setArrayStringByValue(largeStrings);

      const heapBefore = (freshCsp as unknown as { HEAPU8: Uint8Array }).HEAPU8.byteLength;

      // Each getter returns a fresh array whose strings will all be freed on dispose with `using`.
      // If the strings inside aren't destroyed: 100 * ~1KB * 1K iterations = ~100MB leak.
      for (let i = 0; i < 1_000; i++) {
        using arr = helper.getArrayStringByValue();
        void arr.length;
      }

      const heapAfter = (freshCsp as unknown as { HEAPU8: Uint8Array }).HEAPU8.byteLength;
      expect(heapAfter).toBe(heapBefore);
    });

    it('Strings as map keys and values do not leak when container is disposed', () => {
      // No need to force an initial heap growth:
      // 1K iterations produces ~100MB of potential leaks >> the 32MB initial memory size.
      using helper = freshCsp.ContainerBindingMechanismsTestType.create();
      const testMap = new Map<string, string>(
        Array.from({ length: 100 }, (_, i) => [`key-${'k'.repeat(512)}-${i}`, `val-${'v'.repeat(512)}-${i}`])
      );
      helper.setMapStringStringByValue(testMap);

      const heapBefore = (freshCsp as unknown as { HEAPU8: Uint8Array }).HEAPU8.byteLength;

      // Each getter returns a fresh map whose strings will all be freed on dispose with `using`.
      // If the strings inside aren't destroyed: 100 * ~1KB * 1K iterations = ~100MB leak.
      for (let i = 0; i < 1_000; i++) {
        using map = helper.getMapStringStringByValue();
        void map.size;
      }

      const heapAfter = (freshCsp as unknown as { HEAPU8: Uint8Array }).HEAPU8.byteLength;
      expect(heapAfter).toBe(heapBefore);
    });
  });
});
