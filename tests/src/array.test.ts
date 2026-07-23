import { describe, it, expect, beforeAll } from 'vitest';
import { loadCSP } from '../loadModule';
import type { MainModule } from 'connected-spaces-platform-bindings';

describe('CSPFoundation', () => {
  let csp: MainModule;

  // embind's primitive type-conversion checks (e.g. "Cannot convert ... to int")
  // live inside ASSERTIONS-gated blocks, so they only throw in debug builds.
  // Release silently coerces bad input, so we probe the live module once and
  // gate the conversion-error tests on what this build actually does.
  let embindAssertions: boolean;

  beforeAll(async () => {
    csp = await loadCSP();

    using probe = csp.ContainerBindingMechanismsTestType.create();
    try {
      probe.setArrayBasicTypeByValue([undefined as unknown as number]);
      embindAssertions = false;
    } catch {
      embindAssertions = true;
    }
  });

  /*
   * Test the binding of Array<T>, using internal binding test types (at least at the moment)
   *
   * Remember that [1,2] === [1,2] is a FALSE statement in JS. I know, I know, I find
   * this troubling too, but that's the way it is.
   * Native style deep equality for containers is provided via utility methods, which
   * seems to be the JS way of doing this sort of thing, you can't overload operators
   * or do any sort of fudging to make JS equality operators not perform identity equality.
   */

  it('Array equality', () => {
    using elem1 = csp.BindingsTestType.create(1, 'one');
    using elem2 = csp.BindingsTestType.create(2, 'two');
    using wrong = csp.BindingsTestType.create(999, 'nope');

    // Vitest's `toEqual` would have wrongly passed here because both arrays
    // contain BindingsTestType instances without JS enumerable properties - that's the whole reason
    // we use arrayEquals instead.
    expect(csp.arrayEquals([elem1, elem2], [elem1, wrong])).toBe(false);
    expect(csp.arrayEquals([elem1, elem2], [elem1, elem2])).toBe(true);
  });

  it('Array round trip basic type', () => {
    using bindingsArrayHelper = csp.ContainerBindingMechanismsTestType.create();
    const newArr = [1, 2, 3];

    bindingsArrayHelper.setArrayBasicTypeByValue(newArr);
    using roundTripArr = bindingsArrayHelper.getArrayBasicTypeByValue();

    expect(csp.arrayEquals(newArr, roundTripArr)).toBe(true);
  });

  it('Array round trip class type', () => {
    using bindingsArrayHelper = csp.ContainerBindingMechanismsTestType.create();
    using elem1 = csp.BindingsTestType.create(1, 'one');
    using elem2 = csp.BindingsTestType.create(2, 'two');
    const newArr = [elem1, elem2];

    bindingsArrayHelper.setArrayFullTypeByValue(newArr);
    using roundTripArr = bindingsArrayHelper.getArrayFullTypeByValue();

    expect(csp.elementEquals(newArr[0], elem1)).toBe(true);
    expect(csp.elementEquals(newArr[1], elem2)).toBe(true);
    // I know, the above and below are checking the same thing, but I'm extra paranoid with fundamental interop mechanisms.
    expect(csp.arrayEquals(newArr, roundTripArr)).toBe(true);
  });

  it('Array round trip single element', () => {
    using bindingsArrayHelper = csp.ContainerBindingMechanismsTestType.create();
    const newArr = [1];

    bindingsArrayHelper.setArrayBasicTypeByValue(newArr);
    using roundTripArr = bindingsArrayHelper.getArrayBasicTypeByValue();

    expect(csp.arrayEquals(newArr, roundTripArr)).toBe(true);
  });

  it('Array round trip large array', () => {
    using bindingsArrayHelper = csp.ContainerBindingMechanismsTestType.create();
    const newArr = Array.from({ length: 10000 }, (_, i) => i);

    bindingsArrayHelper.setArrayBasicTypeByValue(newArr);
    using roundTripArr = bindingsArrayHelper.getArrayBasicTypeByValue();

    expect(csp.arrayEquals(newArr, roundTripArr)).toBe(true);
  });

  it('Array round trip by const ref and by value are equivalent', () => {
    using bindingsArrayHelper = csp.ContainerBindingMechanismsTestType.create();
    const newArr = [1, 2, 3];

    bindingsArrayHelper.setArrayBasicTypeByValue(newArr);
    using roundTripArrByValue = bindingsArrayHelper.getArrayBasicTypeByValue();

    bindingsArrayHelper.setArrayBasicTypeByConstRef(newArr);
    using roundTripArrByConstRef = bindingsArrayHelper.getArrayBasicTypeByConstRef();

    expect(csp.arrayEquals(roundTripArrByValue, roundTripArrByConstRef)).toBe(true);
  });

  it('Post-Getter Mutations do not impact underlying data', () => {
    using bindingsArrayHelper = csp.ContainerBindingMechanismsTestType.create();
    using elem1 = csp.BindingsTestType.create(1, 'one');
    using elem2 = csp.BindingsTestType.create(2, 'two');
    const newArr = [elem1, elem2];

    bindingsArrayHelper.setArrayFullTypeByValue(newArr);
    using roundTripArr = bindingsArrayHelper.getArrayFullTypeByConstRef();
    expect(csp.arrayEquals(newArr, roundTripArr)).toBe(true);

    //Mutate the array, underlying data should not have changed.
    using replacement = csp.BindingsTestType.create(3, 'three');
    roundTripArr[0] = replacement;
    using afterMutation = bindingsArrayHelper.getArrayFullTypeByConstRef();
    expect(csp.arrayEquals(newArr, afterMutation)).toBe(true);
  });

  it('Post-Setter Mutations do not impact underlying data', () => {
    using bindingsArrayHelper = csp.ContainerBindingMechanismsTestType.create();
    using elem1 = csp.BindingsTestType.create(1, 'one');
    using elem2 = csp.BindingsTestType.create(2, 'two');
    const newArr = [elem1, elem2];

    bindingsArrayHelper.setArrayFullTypeByValue(newArr);

    //Mutate the array we just passed
    using replacement = csp.BindingsTestType.create(3, 'three');
    newArr[0] = replacement;

    // Underlying data should not have changed when we fetch it
    using cppSnapshot1 = bindingsArrayHelper.getArrayFullTypeByConstRef();
    expect(csp.arrayEquals(newArr, cppSnapshot1)).toBe(false);
    using cppSnapshot2 = bindingsArrayHelper.getArrayFullTypeByConstRef();
    expect(csp.arrayEquals([elem1, elem2], cppSnapshot2)).toBe(true);
  });

  it('Getters return independent arrays', () => {
    using bindingsArrayHelper = csp.ContainerBindingMechanismsTestType.create();
    using elem1 = csp.BindingsTestType.create(1, 'one');
    using elem2 = csp.BindingsTestType.create(2, 'two');
    const newArr = [elem1, elem2];

    bindingsArrayHelper.setArrayFullTypeByValue(newArr);
    using roundTripArr1 = bindingsArrayHelper.getArrayFullTypeByConstRef();
    using roundTripArr2 = bindingsArrayHelper.getArrayFullTypeByConstRef();

    expect(csp.arrayEquals(roundTripArr1, roundTripArr2)).toBe(true);

    // Mutating one array should not effect the other
    using replacement = csp.BindingsTestType.create(3, 'three');
    roundTripArr1[0] = replacement;

    expect(csp.arrayEquals(roundTripArr1, roundTripArr2)).toBe(false);
  });

  it('Setting a sparse array throws', (ctx) => {
    ctx.skip(
      !embindAssertions,
      'release build: embind primitive type-conversion checks are ASSERTIONS-gated (debug only)'
    );
    using bindingsArrayHelper = csp.ContainerBindingMechanismsTestType.create();

    // JS arrays of primitives with holes in them
    expect(() => bindingsArrayHelper.setArrayBasicTypeByValue(new Array(5))).toThrow(
      'Cannot convert "undefined" to int'
    );
    // @ts-expect-error sparse literal types as `(BindingsTestType | undefined)[]` - Arguably typescript protects us from needing to test this case, but i'd like to be thorough.
    expect(() => bindingsArrayHelper.setArrayBasicTypeByValue([1, , 3])).toThrow('Cannot convert "undefined" to int');

    // The below will throw
    // - "Cannot read properties of undefined (reading '$$')" for Chromium-based browsers
    // - "TypeError: undefined is not an object (evaluating 'handle.$$')" for Webkit/Safari.

    // Arrays of C++ objects with holes in them
    using elem = csp.BindingsTestType.create(1, 'one');
    expect(() => bindingsArrayHelper.setArrayFullTypeByValue(new Array(3))).toThrow(TypeError);
    // @ts-expect-error sparse literal types as `(BindingsTestType | undefined)[]` - Arguably typescript protects us from needing to test this case, but i'd like to be thorough.
    expect(() => bindingsArrayHelper.setArrayFullTypeByValue([elem, , elem])).toThrow(TypeError);
  });

  it('Setting array with invalid type throws', (ctx) => {
    ctx.skip(
      !embindAssertions,
      'release build: embind primitive type-conversion checks are ASSERTIONS-gated (debug only)'
    );
    using bindingsArrayHelper = csp.ContainerBindingMechanismsTestType.create();

    // @ts-expect-error - Typescript will not allow us to set the wrong type, but want to see what happens anyways
    expect(() => bindingsArrayHelper.setArrayBasicTypeByValue(new Array('not a string'))).toThrow(
      'Cannot convert "not a string" to int'
    );

    // @ts-expect-error
    expect(() => bindingsArrayHelper.setArrayFullTypeByValue(new Array('not a string'))).toThrow(
      'Cannot pass "not a string" as a BindingsTestType'
    );
  });

  it('Setting null array throws', () => {
    using bindingsArrayHelper = csp.ContainerBindingMechanismsTestType.create();

    // These will throw
    // - "Cannot read properties of null (reading 'length')" for Chromium-based browsers
    // - "null is not an object (evaluating 'handle[key]')" for Webkit/Safari.

    // @ts-expect-error - Typescript will not allow us to set null but want to see what happens anyways
    expect(() => bindingsArrayHelper.setArrayBasicTypeByValue(null)).toThrow(TypeError);

    // @ts-expect-error
    expect(() => bindingsArrayHelper.setArrayFullTypeByValue(null)).toThrow(TypeError);
  });

  /*
   * There is a leak in operator= in Array that causes this test to fail.
   * In fact, I wrote this test with the intent of proving and documenting this leak somewhere.
   * We may fix it as part of this effort, although all our interop types tend to have issues
   * like this, which is part of why we want to replace them.
   * This leak only triggers on the second assignment of the same array, so all other tests
   * which just do single-pass stuff are fine. But it's a bit fragile eh? :P
   * https://github.com/magnopus-opensource/connected-spaces-platform/blob/96fd3de47c96ca67ae02cc4b7057bd155ec62ddd/Library/include/CSP/Common/Array.h#L138
   */
  it.skip('Repeated sets do not grow lifetime', () => {
    using bindingsArrayHelper = csp.ContainerBindingMechanismsTestType.create();
    const beforeAliveCount = csp.BindingsTestType.aliveCount;

    for (let i = 0; i < 10; ++i) {
      {
        using elem = csp.BindingsTestType.create(1, 'one');
        bindingsArrayHelper.setArrayFullTypeByValue([elem]);
        // elem disposed here, meaning the only copy is in the C++ backend
      }
      expect(csp.BindingsTestType.aliveCount).toBe(beforeAliveCount + 1);
    }

    // Should not have grown as the old data should have been destructed on each loop iteration.
    expect(csp.BindingsTestType.aliveCount).toBe(beforeAliveCount + 1);
  });

  /* Array of Pointers tests */

  it('Round trip array of pointers', () => {
    using bindingsArrayHelper = csp.ContainerBindingMechanismsTestType.create();
    const beforeAliveCount = csp.BindingsTestType.aliveCount;
    using elem1 = csp.BindingsTestType.create(1, 'one');
    using elem2 = csp.BindingsTestType.create(2, 'two');
    const newArr = [elem1, elem2];

    //This will have made 2 more elements, owned by the JS stack
    const afterAllocationAliveCount = csp.BindingsTestType.aliveCount;
    expect(afterAllocationAliveCount).equal(beforeAliveCount + 2);

    bindingsArrayHelper.setArrayOfPointersByValue(newArr);
    let roundTripArr = bindingsArrayHelper.getArrayOfPointersByValue();

    expect(csp.elementEquals(newArr[0], elem1)).toBe(true);
    expect(csp.elementEquals(newArr[1], elem2)).toBe(true);
    // I know, the above and below are checking the same thing, but I'm extra paranoid with fundamental interop mechanisms.
    expect(csp.arrayEquals(newArr, roundTripArr)).toBe(true);

    // There should have been no additional allocations caused by the round trip
    expect(csp.BindingsTestType.aliveCount).equals(afterAllocationAliveCount);
  });

  it('Empty pointer array round trip', () => {
    using bindingsArrayHelper = csp.ContainerBindingMechanismsTestType.create();
    const beforeAliveCount = csp.BindingsTestType.aliveCount;

    bindingsArrayHelper.setArrayOfPointersByValue([]);
    let roundTripArr = bindingsArrayHelper.getArrayOfPointersByValue();

    expect(roundTripArr.length).toBe(0);
    expect(csp.arrayEquals([], roundTripArr)).toBe(true);

    // There should have been no allocations caused by the round trip
    expect(csp.BindingsTestType.aliveCount).equals(beforeAliveCount);
  });

  it('Only nulls pointer array round trip', () => {
    using bindingsArrayHelper = csp.ContainerBindingMechanismsTestType.create();
    const beforeAliveCount = csp.BindingsTestType.aliveCount;

    bindingsArrayHelper.setArrayOfPointersByValue([null, null, null]);
    let roundTripArr = bindingsArrayHelper.getArrayOfPointersByValue();

    expect(roundTripArr.length).toBe(3);
    expect(csp.arrayEquals([null, null, null], roundTripArr)).toBe(true);

    // There should have been no allocations caused by the round trip
    expect(csp.BindingsTestType.aliveCount).equals(beforeAliveCount);
  });

  it('Array containing nulls pointer array round trip', () => {
    using bindingsArrayHelper = csp.ContainerBindingMechanismsTestType.create();
    const beforeAliveCount = csp.BindingsTestType.aliveCount;
    using elem1 = csp.BindingsTestType.create(1, 'one');
    using elem2 = csp.BindingsTestType.create(2, 'two');
    const newArr = [elem1, null, elem2];

    //This will have made 2 more elements, owned by the JS stack
    const afterAllocationAliveCount = csp.BindingsTestType.aliveCount;
    expect(afterAllocationAliveCount).equal(beforeAliveCount + 2);

    bindingsArrayHelper.setArrayOfPointersByValue(newArr);
    let roundTripArr = bindingsArrayHelper.getArrayOfPointersByValue();

    expect(csp.elementEquals(newArr[0], elem1)).toBe(true);
    expect(newArr[1]).toBe(null);
    expect(csp.elementEquals(newArr[2], elem2)).toBe(true);
    // I know, the above and below are checking the same thing, but I'm extra paranoid with fundamental interop mechanisms.
    expect(csp.arrayEquals(newArr, roundTripArr)).toBe(true);

    // There should have been no additional allocations caused by the round trip
    expect(csp.BindingsTestType.aliveCount).equals(afterAllocationAliveCount);
  });

  it('Mutating element via pointer array is reflected in original handle', () => {
    using bindingsArrayHelper = csp.ContainerBindingMechanismsTestType.create();

    let ptrList = bindingsArrayHelper.getArrayOfCppOwnedPointers();
    expect(ptrList[0]?.name).equals('One');

    if (ptrList[0] != null) {
      ptrList[0].name = 'mutatedOne';
    }

    expect(ptrList[0]?.name).equals('mutatedOne');
    expect(bindingsArrayHelper.getArrayOfCppOwnedPointers()[0]?.name).equals('mutatedOne');
    expect(csp.elementEquals(ptrList[0], bindingsArrayHelper.getArrayOfCppOwnedPointers()[0])).toBe(true);
  });

  it('Pointer arrays are independent, pointed to objects are not', () => {
    using bindingsArrayHelper = csp.ContainerBindingMechanismsTestType.create();

    let ptrList1 = bindingsArrayHelper.getArrayOfCppOwnedPointers();
    let ptrList2 = bindingsArrayHelper.getArrayOfCppOwnedPointers();
    expect(csp.arrayEquals(ptrList1, ptrList2)).toBe(true);

    //Change something in one of the lists, both should change
    if (ptrList1[0] != null) {
      ptrList1[0].name = 'mutatedOne';
    }

    expect(ptrList1[0]?.name).equals('mutatedOne');
    expect(ptrList2[0]?.name).equals('mutatedOne');

    //Add an element to one of the lists
    using elem1 = csp.BindingsTestType.create(1, 'one');
    ptrList1.push(elem1);

    //Only that list should have changed structurally
    expect(ptrList1.length).equals(3);
    expect(ptrList2.length).equals(2);
    expect(csp.arrayEquals(ptrList1, ptrList2)).toBe(false);
  });

  it('Array dispose function is not enumerable', () => {
    using bindingsArrayHelper = csp.ContainerBindingMechanismsTestType.create();
    const newArr = [1, 2, 3];

    bindingsArrayHelper.setArrayBasicTypeByValue(newArr);
    using roundTripArr = bindingsArrayHelper.getArrayBasicTypeByValue();

    // Check that the dispose function exists
    expect(Symbol.dispose in roundTripArr).toBe(true);
    expect(typeof roundTripArr[Symbol.dispose]).toBe('function');

    expect(roundTripArr.propertyIsEnumerable(Symbol.dispose)).toBe(false);
  });

  it('Array round trip strict Vitest equality', () => {
    using bindingsArrayHelper = csp.ContainerBindingMechanismsTestType.create();
    const newArr = [1, 2, 3];

    bindingsArrayHelper.setArrayBasicTypeByValue(newArr);
    using roundTripArr = bindingsArrayHelper.getArrayBasicTypeByValue();

    // Ensure that the round-tripped array is deeply equal to the original using the Vitest matcher.
    // It checks all enumerable properties as well as array contents, so it will fail if the dispose
    // function is enumerable. This fact could otherwise trip up developers writing tests on arrays
    // coming out of the bindings.
    expect(roundTripArr).toStrictEqual(newArr);
  });
});
