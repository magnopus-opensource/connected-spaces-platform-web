import { describe, it, expect, beforeAll } from 'vitest';
import { loadCSP } from '../loadModule';
import type { MainModule } from 'connected-spaces-platform-bindings';

/*
 * Exercises the C++-side `elementEquals` utility bound from Equality.cpp.
 * elementEquals delegates to JS === for primitives and identical handles,
 * recurses into arrayEquals for arrays, and defers to a bound `equals`
 * method for C++ class instances (BindingsTestType has one via operator==).
 * 
 * We provide this method mostly for container equality, which would be burdensome
 * to dispatch on manually every time, although in theory it should be a 
 * "one stop shop" for all equality when you don't know or care if you have a 
 * raw JS object or a bound C++ object, and don't want any surprises.
 * 
 * A large part of this file was LLM generated as it's mostly rote, then the tests were curated.
 */

describe('elementEquals', () => {
  let csp: MainModule;

  beforeAll(async () => {
    csp = await loadCSP();
  });

  /* Primitives */
  it('Number equality', () => {
    expect(csp.elementEquals(1, 1)).toBe(true);
    expect(csp.elementEquals(1, 2)).toBe(false);
  });

  it('Strings equality', () => {
    expect(csp.elementEquals('hello', 'hello')).toBe(true);
    expect(csp.elementEquals('hello', 'world')).toBe(false);
  });


  it('Boolean equality', () => {
    expect(csp.elementEquals(true, true)).toBe(true);
    expect(csp.elementEquals(false, false)).toBe(true);
    expect(csp.elementEquals(true, false)).toBe(false);
  });

  it('Mismatched primitive types are not equal', () => {
    // typeof "number" vs typeof "string"
    expect(csp.elementEquals(1, '1')).toBe(false);
    // typeof "boolean" vs typeof "number"
    expect(csp.elementEquals(true, 1)).toBe(false);
  });

  it('NaN is not equal to NaN (matches JS === semantics)', () => {
    expect(csp.elementEquals(NaN, NaN)).toBe(false);
  });

  it('null equals null', () => {
    expect(csp.elementEquals(null, null)).toBe(true);
  });

  it('undefined equals undefined', () => {
    expect(csp.elementEquals(undefined, undefined)).toBe(true);
  });

  it('null is not equal to an object', () => {
    using obj = csp.BindingsTestType.create(1, 'a');
    expect(csp.elementEquals(null, obj)).toBe(false);
    expect(csp.elementEquals(obj, null)).toBe(false);
  });

  it('null is not equal to undefined', () => {
    expect(csp.elementEquals(null, undefined)).toBe(false);
  });

  /* Bound C++ Objects */

  it('Same handle to a bound object is equal to itself', () => {
    using a = csp.BindingsTestType.create(1, 'one');
    expect(csp.elementEquals(a, a)).toBe(true);
  });

  it('Distinct handles with equal field values are equal via bound equality operator', () => {
    using a = csp.BindingsTestType.create(1, 'one');
    using b = csp.BindingsTestType.create(1, 'one');
    expect(a == b).toBe(false); // Identity comparison is still false here, demonstrating the difference.
    expect(csp.elementEquals(a, b)).toBe(true);
  });

  it('Distinct handles with different values are not equal', () => {
    using a = csp.BindingsTestType.create(1, 'one');
    using b = csp.BindingsTestType.create(2, 'two');
    expect(csp.elementEquals(a, b)).toBe(false);
  });

  it('Bound object is not equal to a plain JS object with identical properties', () => {
    using bound = csp.BindingsTestType.create(1, 'one');
    const shaped = { value: 1, name: 'one' };
    expect(csp.elementEquals(bound, shaped)).toBe(false);
  });

  /* JS Objects */

  it('Distinct JS objects with identical shape are not equal', () => {
    expect(csp.elementEquals({ x: 1 }, { x: 1 })).toBe(false);
  });

  it('Same JS object reference is equal to itself', () => {
    const o = { x: 1 };
    expect(csp.elementEquals(o, o)).toBe(true);
  });

  /* Arrays. Does internal recursion */

  it('Empty arrays are equal', () => {
    expect(csp.elementEquals([], [])).toBe(true);
  });

  it('Arrays of primitives equality', () => {
    expect(csp.elementEquals([1, 2, 3], [1, 2, 3])).toBe(true);
    expect(csp.elementEquals([1, 2, 3], [1, 2, 4])).toBe(false);
  });


  it('Arrays of differing lengths are not equal', () => {
    expect(csp.elementEquals([1, 2], [1, 2, 3])).toBe(false);
  });

  it('Nested arrays compare structurally', () => {
    expect(csp.elementEquals([[1, 2], [3, 4]], [[1, 2], [3, 4]])).toBe(true);
    expect(csp.elementEquals([[1, 2], [3, 4]], [[1, 2], [3, 5]])).toBe(false);
  });

  it('Array of bound objects compares element-wise via equals()', () => {
    const a = [csp.BindingsTestType.create(1, 'one'), csp.BindingsTestType.create(2, 'two')];
    const b = [csp.BindingsTestType.create(1, 'one'), csp.BindingsTestType.create(2, 'two')];
    expect(csp.elementEquals(a, b)).toBe(true);
  });

  it('Array of bound objects with one differing element is not equal', () => {
    const a = [csp.BindingsTestType.create(1, 'one'), csp.BindingsTestType.create(2, 'two')];
    const b = [csp.BindingsTestType.create(1, 'one'), csp.BindingsTestType.create(99, 'two')];
    expect(csp.elementEquals(a, b)).toBe(false);
  });

  it('Array vs non-array of same length is not equal', () => {
    expect(csp.elementEquals([1, 2, 3], '123')).toBe(false);
  });

  it('Array of arrays of bound objects compares recursively through every layer', () => {
    // Exercise the full recursive chain you get when you have arrays of arrays.
    const a = [
      [csp.BindingsTestType.create(1, 'one'), csp.BindingsTestType.create(2, 'two')],
      [csp.BindingsTestType.create(3, 'three')],
    ];
    const b = [
      [csp.BindingsTestType.create(1, 'one'), csp.BindingsTestType.create(2, 'two')],
      [csp.BindingsTestType.create(3, 'three')],
    ];
    const differingInnerType = [
      [csp.BindingsTestType.create(1, 'one'), csp.BindingsTestType.create(2, 'two')],
      [csp.BindingsTestType.create(3, 'oops')],
    ];
    const differingOuterShape = [
      [csp.BindingsTestType.create(1, 'one'), csp.BindingsTestType.create(2, 'two')],
    ];

    expect(csp.elementEquals(a, b)).toBe(true);
    expect(csp.elementEquals(a, differingInnerType)).toBe(false);
    expect(csp.elementEquals(a, differingOuterShape)).toBe(false);
  });

  it('Array of pointers equate based on underlying object equality', () => {
    using bindingsArrayHelper = csp.BindingsMechanismsTestType.create();
     
    let ptrList1 = bindingsArrayHelper.getArrayOfCppOwnedPointers();

    // Mirror the data the the CppOwnedPointers gives us
    using entry1 = csp.BindingsTestType.create(1, 'One');
    using entry2 = csp.BindingsTestType.create(2, 'Two');

    bindingsArrayHelper.setArrayOfPointersByValue([entry1, entry2]);
    let ptrList2 = bindingsArrayHelper.getArrayOfPointersByValue();

    // Different arrays, but still equivalent according to equality operators
    expect(ptrList1).not.equals(ptrList2);
    expect(csp.arrayEquals(ptrList1, ptrList2)).toBe(true);

    // Being paranoid, check that these are actually different arrays with disconnected elements
     if (ptrList1[0] != null){
      ptrList1[0].name = "mutatedOne";
     }

     expect(csp.arrayEquals(ptrList1, ptrList2)).toBe(false);
     expect(ptrList1[0]?.name).equals("mutatedOne");
     expect(ptrList2[0]?.name).equals("One");
  });
});
