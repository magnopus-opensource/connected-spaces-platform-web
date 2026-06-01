import { describe, it, expect, beforeAll } from 'vitest';
import { loadCSP } from '../loadModule';
import type { MainModule } from 'connected-spaces-platform-bindings';

/*
 * Tests for the binding of List<T>, using internal binding test types.
 *
 * Lists and Arrays are completely identical from a JS/TS perspective (both present
 * as native JS arrays), and nearly identical at the binding layer too. The heavy
 * lifting — sparse arrays, null/invalid-type handling, lifetime/leak edge cases,
 * post-mutation independence — is exercised thoroughly in array.test.ts and the
 * behaviour is shared, so we don't repeat it all here.
 *
 * Just do some basic tests to validate binding is working as we expect for this type.
 */

describe('List bindings', () => {
  let csp: MainModule;

  beforeAll(async () => {
    csp = await loadCSP();
  });

  it('List round trip basic type', () => {
    using helper = csp.BindingsMechanismsTestType.create();
    const newList = [1, 2, 3];

    helper.setListBasicTypeByValue(newList);
    using roundTrip = helper.getListBasicTypeByValue();

    expect(csp.arrayEquals(newList, roundTrip)).toBe(true);
  });

  it('List round trip class type', () => {
    using helper = csp.BindingsMechanismsTestType.create();
    using elem1 = csp.BindingsTestType.create(1, 'one');
    using elem2 = csp.BindingsTestType.create(2, 'two');
    const newList = [elem1, elem2];

    helper.setListFullTypeByValue(newList);
    using roundTrip = helper.getListFullTypeByValue();

    expect(csp.arrayEquals(newList, roundTrip)).toBe(true);
  });

  it('List round trip by const ref and by value are equivalent', () => {
    using helper = csp.BindingsMechanismsTestType.create();
    const newList = [1, 2, 3];

    helper.setListBasicTypeByValue(newList);
    using byValue = helper.getListBasicTypeByValue();

    helper.setListBasicTypeByConstRef(newList);
    using byConstRef = helper.getListBasicTypeByConstRef();

    expect(csp.arrayEquals(byValue, byConstRef)).toBe(true);
  });

  it('Round trip list of pointers', () => {
    using helper = csp.BindingsMechanismsTestType.create();
    const beforeAliveCount = csp.BindingsTestType.aliveCount;
    using elem1 = csp.BindingsTestType.create(1, 'one');
    using elem2 = csp.BindingsTestType.create(2, 'two');
    const newList = [elem1, elem2];

    // The two creates above are the only allocations; the round trip should add none.
    const afterAllocationAliveCount = csp.BindingsTestType.aliveCount;
    expect(afterAllocationAliveCount).equal(beforeAliveCount + 2);

    helper.setListOfPointersByValue(newList);
    const roundTrip = helper.getListOfPointersByValue();

    expect(csp.arrayEquals(newList, roundTrip)).toBe(true);

    // Pointer elements are non-owning references, so no copies were made.
    expect(csp.BindingsTestType.aliveCount).equals(afterAllocationAliveCount);
  });

  it('Mutating element via pointer list is reflected in original handle', () => {
    using helper = csp.BindingsMechanismsTestType.create();

    const ptrList = helper.getListOfCppOwnedPointers();
    expect(ptrList[0]?.name).equals('One');

    if (ptrList[0] != null) {
      ptrList[0].name = 'mutatedOne';
    }

    // The handle points at CSP-owned memory, so the mutation is visible on a fresh fetch.
    expect(helper.getListOfCppOwnedPointers()[0]?.name).equals('mutatedOne');
    expect(csp.elementEquals(ptrList[0], helper.getListOfCppOwnedPointers()[0])).toBe(true);
  });
});
