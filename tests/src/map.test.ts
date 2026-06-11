import { describe, it, expect, beforeAll } from 'vitest';
import { loadCSP } from '../loadModule';
import type { MainModule } from 'connected-spaces-platform-bindings';

/*
 * Tests for the binding of Map<K, V>, using internal binding test types.
 *
 * Maps present in JS as native Map instances, and most of the interesting behaviour —
 * structural/recursive equality, insertion-order sensitivity, lifetime/disposal, and
 * non-owning pointer semantics — is exercised thoroughly in equality.test.ts and
 * memory.test.ts. That behaviour is shared, so we don't repeat it all here.
 *
 * Just do some basic tests to validate binding is working as we expect for this type.
 */

describe('Map bindings', () => {
  let csp: MainModule;

  beforeAll(async () => {
    csp = await loadCSP();
  });

  it('Map round trip basic type', () => {
    using helper = csp.BindingsMechanismsTestType.create();
    const newMap = new Map([[1, 10], [2, 20], [3, 30]]);

    helper.setMapBasicTypeByValue(newMap);
    using roundTrip = helper.getMapBasicTypeByValue();

    expect(csp.mapEquals(newMap, roundTrip)).toBe(true);
  });

  it('Map round trip class type', () => {
    using helper = csp.BindingsMechanismsTestType.create();
    using elem1 = csp.BindingsTestType.create(1, 'one');
    using elem2 = csp.BindingsTestType.create(2, 'two');
    const newMap = new Map([[1, elem1], [2, elem2]]);

    helper.setMapFullTypeByValue(newMap);
    using roundTrip = helper.getMapFullTypeByValue();

    expect(csp.mapEquals(newMap, roundTrip)).toBe(true);
  });

  it('Map round trip by const ref and by value are equivalent', () => {
    using helper = csp.BindingsMechanismsTestType.create();
    const newMap = new Map([[1, 10], [2, 20], [3, 30]]);

    helper.setMapBasicTypeByValue(newMap);
    using byValue = helper.getMapBasicTypeByValue();

    helper.setMapBasicTypeByConstRef(newMap);
    using byConstRef = helper.getMapBasicTypeByConstRef();

    expect(csp.mapEquals(byValue, byConstRef)).toBe(true);
  });

  it('Round trip map of pointers', () => {
    using helper = csp.BindingsMechanismsTestType.create();
    const beforeAliveCount = csp.BindingsTestType.aliveCount;
    using elem1 = csp.BindingsTestType.create(1, 'one');
    using elem2 = csp.BindingsTestType.create(2, 'two');
    const newMap = new Map([[1, elem1], [2, elem2]]);

    // The two creates above are the only allocations; the round trip should add none.
    const afterAllocationAliveCount = csp.BindingsTestType.aliveCount;
    expect(afterAllocationAliveCount).equal(beforeAliveCount + 2);

    helper.setMapOfPointersByValue(newMap);
    const roundTrip = helper.getMapOfPointersByValue();

    expect(csp.mapEquals(newMap, roundTrip)).toBe(true);

    // Pointer values are non-owning references, so no copies were made.
    expect(csp.BindingsTestType.aliveCount).equals(afterAllocationAliveCount);
  });

  it('Mutating value via pointer map is reflected in original handle', () => {
    using helper = csp.BindingsMechanismsTestType.create();

    const ptrMap = helper.getMapOfCppOwnedPointers();
    expect(ptrMap.get(1)?.name).equals('One');

    const entry = ptrMap.get(1);
    if (entry != null) {
      entry.name = 'mutatedOne';
    }

    // The handle points at CSP-owned memory, so the mutation is visible on a fresh fetch.
    expect(helper.getMapOfCppOwnedPointers().get(1)?.name).equals('mutatedOne');
    expect(csp.elementEquals(ptrMap.get(1), helper.getMapOfCppOwnedPointers().get(1))).toBe(true);
  });

  it('Map dispose function is not enumerable', () => {
    using bindingsMapHelper = csp.BindingsMechanismsTestType.create();
    const newMap = new Map([[1, 10], [2, 20], [3, 30]]);

    bindingsMapHelper.setMapBasicTypeByValue(newMap);
    using roundTripMap = bindingsMapHelper.getMapBasicTypeByValue();

    // Check that the dispose function exists
    expect(Symbol.dispose in roundTripMap).toBe(true);
    expect(typeof roundTripMap[Symbol.dispose]).toBe('function');

    expect(roundTripMap.propertyIsEnumerable(Symbol.dispose)).toBe(false);
  });

  it('Map round trip strict Vitest equality', () => {
    using bindingsMapHelper = csp.BindingsMechanismsTestType.create();
    const newMap = new Map([[1, 10], [2, 20], [3, 30]]);

    bindingsMapHelper.setMapBasicTypeByValue(newMap);
    using roundTripMap = bindingsMapHelper.getMapBasicTypeByValue();

    // Ensure that the round-tripped map is deeply equal to the original using the Vitest matcher.
    // It checks all enumerable properties as well as map contents, so it will fail if the dispose
    // function is enumerable. This fact could otherwise trip up developers writing tests on maps
    // coming out of the bindings.
    expect(roundTripMap).toStrictEqual(newMap);
  });
});
