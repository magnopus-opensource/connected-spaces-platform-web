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

describe('equality', () => {
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
    // @ts-expect-error: intentionally testing type mismatch
    expect(csp.elementEquals(1, '1')).toBe(false);

    // typeof "boolean" vs typeof "number"
    // @ts-expect-error: intentionally testing type mismatch
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
    expect(
      csp.elementEquals(
        [
          [1, 2],
          [3, 4]
        ],
        [
          [1, 2],
          [3, 4]
        ]
      )
    ).toBe(true);
    expect(
      csp.elementEquals(
        [
          [1, 2],
          [3, 4]
        ],
        [
          [1, 2],
          [3, 5]
        ]
      )
    ).toBe(false);
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
    // @ts-expect-error: intentionally testing type mismatch
    expect(csp.elementEquals([1, 2, 3], '123')).toBe(false);
  });

  it('Array of arrays of bound objects compares recursively through every layer', () => {
    // Exercise the full recursive chain you get when you have arrays of arrays.
    const a = [
      [csp.BindingsTestType.create(1, 'one'), csp.BindingsTestType.create(2, 'two')],
      [csp.BindingsTestType.create(3, 'three')]
    ];
    const b = [
      [csp.BindingsTestType.create(1, 'one'), csp.BindingsTestType.create(2, 'two')],
      [csp.BindingsTestType.create(3, 'three')]
    ];
    const differingInnerType = [
      [csp.BindingsTestType.create(1, 'one'), csp.BindingsTestType.create(2, 'two')],
      [csp.BindingsTestType.create(3, 'oops')]
    ];
    const differingOuterShape = [[csp.BindingsTestType.create(1, 'one'), csp.BindingsTestType.create(2, 'two')]];

    expect(csp.elementEquals(a, b)).toBe(true);
    expect(csp.elementEquals(a, differingInnerType)).toBe(false);
    expect(csp.elementEquals(a, differingOuterShape)).toBe(false);
  });

  it('Array of pointers equate based on underlying object equality', () => {
    using bindingsArrayHelper = csp.ContainerBindingMechanismsTestType.create();

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
    if (ptrList1[0] != null) {
      ptrList1[0].name = 'mutatedOne';
    }

    expect(csp.arrayEquals(ptrList1, ptrList2)).toBe(false);
    expect(ptrList1[0]?.name).equals('mutatedOne');
    expect(ptrList2[0]?.name).equals('One');
  });

  /*
   * Maps. mapEquals is the Map analogue of arrayEquals and recurses through elementEquals
   * the same way, so most of the tests below are deliberately named to mirror the array
   * tests above.
   */
  it('Empty maps are equal', () => {
    expect(csp.mapEquals(new Map(), new Map())).toBe(true);
  });

  it('Maps of primitives equality', () => {
    const a = new Map([
      [1, 10],
      [2, 20]
    ]);
    const b = new Map([
      [1, 10],
      [2, 20]
    ]);
    expect(a).not.equals(b); // Distinct references...
    expect(csp.mapEquals(a, b)).toBe(true); // ...but equal by content...
    expect(
      csp.mapEquals(
        a,
        new Map([
          [1, 10],
          [2, 99]
        ])
      )
    ).toBe(false); // ...and not when a value differs.
  });

  it('Maps of differing sizes are not equal', () => {
    const a = new Map([
      [1, 10],
      [2, 20]
    ]);
    const b = new Map([[1, 10]]);
    expect(csp.mapEquals(a, b)).toBe(false);
  });

  it('Nested maps compare structurally', () => {
    const a = new Map([
      [1, new Map([[10, 100]])],
      [2, new Map([[20, 200]])]
    ]);
    const b = new Map([
      [1, new Map([[10, 100]])],
      [2, new Map([[20, 200]])]
    ]);
    const differing = new Map([
      [1, new Map([[10, 100]])],
      [2, new Map([[20, 999]])]
    ]);
    expect(csp.mapEquals(a, b)).toBe(true);
    expect(csp.mapEquals(a, differing)).toBe(false);
  });

  it('Map of bound objects compares value-wise via equals()', () => {
    using a1 = csp.BindingsTestType.create(1, 'one');
    using a2 = csp.BindingsTestType.create(2, 'two');
    using b1 = csp.BindingsTestType.create(1, 'one');
    using b2 = csp.BindingsTestType.create(2, 'two');

    // Distinct handles with equal field values, so equality is by value, not identity.
    const a = new Map([
      [1, a1],
      [2, a2]
    ]);
    const b = new Map([
      [1, b1],
      [2, b2]
    ]);
    expect(csp.mapEquals(a, b)).toBe(true);
  });

  it('Map of bound objects with one differing value is not equal', () => {
    using a1 = csp.BindingsTestType.create(1, 'one');
    using a2 = csp.BindingsTestType.create(2, 'two');
    using b1 = csp.BindingsTestType.create(1, 'one');
    using b2 = csp.BindingsTestType.create(99, 'two');

    const a = new Map([
      [1, a1],
      [2, a2]
    ]);
    const b = new Map([
      [1, b1],
      [2, b2]
    ]);
    expect(csp.mapEquals(a, b)).toBe(false);
  });

  it('Map of maps of arrays of bound objects compares recursively through every layer', () => {
    // Big test. outer Map (string keys) -> inner Map (number keys) -> array -> bound objects.
    // A true result here means the whole recursive chain fired:
    // mapEquals -> elementEquals -> mapEquals -> elementEquals -> arrayEquals -> elementEquals -> bound equals().

    //a and b equal each other
    const a = new Map([
      [
        'group1',
        new Map([
          [1, [csp.BindingsTestType.create(1, 'one'), csp.BindingsTestType.create(2, 'two')]],
          [2, [csp.BindingsTestType.create(3, 'three')]]
        ])
      ],
      ['group2', new Map([[9, [csp.BindingsTestType.create(9, 'nine')]]])]
    ]);
    const b = new Map([
      [
        'group1',
        new Map([
          [1, [csp.BindingsTestType.create(1, 'one'), csp.BindingsTestType.create(2, 'two')]],
          [2, [csp.BindingsTestType.create(3, 'three')]]
        ])
      ],
      ['group2', new Map([[9, [csp.BindingsTestType.create(9, 'nine')]]])]
    ]);

    // Each variant below tries to perturb from a/b in just one way
    const differingLeafObject = new Map([
      [
        'group1',
        new Map([
          [1, [csp.BindingsTestType.create(1, 'one'), csp.BindingsTestType.create(2, 'two')]],
          [2, [csp.BindingsTestType.create(3, 'oops')]]
        ])
      ],
      ['group2', new Map([[9, [csp.BindingsTestType.create(9, 'nine')]]])]
    ]);

    const differingInnerKey = new Map([
      [
        'group1',
        new Map([
          [1, [csp.BindingsTestType.create(1, 'one'), csp.BindingsTestType.create(2, 'two')]],
          [99, [csp.BindingsTestType.create(3, 'three')]]
        ])
      ],
      ['group2', new Map([[9, [csp.BindingsTestType.create(9, 'nine')]]])]
    ]);

    const differingArrayShape = new Map([
      [
        'group1',
        new Map([
          [1, [csp.BindingsTestType.create(1, 'one')]],
          [2, [csp.BindingsTestType.create(3, 'three')]]
        ])
      ],
      ['group2', new Map([[9, [csp.BindingsTestType.create(9, 'nine')]]])]
    ]);

    const differingOuterShape = new Map([
      [
        'group1',
        new Map([
          [1, [csp.BindingsTestType.create(1, 'one'), csp.BindingsTestType.create(2, 'two')]],
          [2, [csp.BindingsTestType.create(3, 'three')]]
        ])
      ]
    ]);

    expect(csp.mapEquals(a, b)).toBe(true);
    expect(csp.mapEquals(a, differingLeafObject)).toBe(false);
    expect(csp.mapEquals(a, differingInnerKey)).toBe(false);
    expect(csp.mapEquals(a, differingArrayShape)).toBe(false);
    expect(csp.mapEquals(a, differingOuterShape)).toBe(false);
  });

  // Keep in mind mapEquals(x, x) would return true because the reference equality
  // would short-circuit. Not sure about this, feels like a javascripty thing that
  // could be unexpected no matter what side of the fence you come down on.
  it('Map vs non-Map is not equal', () => {
    // @ts-expect-error: intentionally testing non-maps
    expect(csp.mapEquals([], [])).toBe(false); // Arrays not maps

    // @ts-expect-error: intentionally testing non-maps
    expect(csp.mapEquals({}, {})).toBe(false); // Objects not maps

    // @ts-expect-error: intentionally testing non-maps
    expect(csp.mapEquals(new Map(), [])).toBe(false);

    // @ts-expect-error: intentionally testing non-maps
    expect(csp.mapEquals(new Map(), {})).toBe(false);

    // @ts-expect-error: intentionally testing non-maps
    expect(csp.mapEquals(new Map(), null)).toBe(false);

    // @ts-expect-error: intentionally testing non-maps
    expect(csp.mapEquals(1, 2)).toBe(false);
  });

  it('Same Map reference is equal to itself', () => {
    const m = new Map([
      [1, 10],
      [2, 20]
    ]);
    expect(csp.mapEquals(m, m)).toBe(true);
  });

  it('Maps with the same pairs in a different insertion order are still equal', () => {
    const a = new Map([
      [1, 10],
      [2, 20]
    ]);
    const b = new Map([
      [2, 20],
      [1, 10]
    ]);
    expect(csp.mapEquals(a, b)).toBe(true);
  });

  it('Maps with different keys are not equal', () => {
    const a = new Map([
      [1, 10],
      [2, 20]
    ]);
    const b = new Map([
      [1, 10],
      [3, 20]
    ]);
    expect(csp.mapEquals(a, b)).toBe(false);
  });

  it('String-keyed maps compare by key and value', () => {
    const a = new Map([
      ['one', 1],
      ['two', 2]
    ]);
    const b = new Map([
      ['one', 1],
      ['two', 2]
    ]);
    const differingKey = new Map([
      ['one', 1],
      ['xxx', 2]
    ]);
    expect(csp.mapEquals(a, b)).toBe(true);
    expect(csp.mapEquals(a, differingKey)).toBe(false);
  });

  it('Map values of null compare correctly', () => {
    using a1 = csp.BindingsTestType.create(1, 'one');
    using b1 = csp.BindingsTestType.create(1, 'one');

    const a = new Map([
      [1, a1],
      [2, null]
    ]);
    const b = new Map([
      [1, b1],
      [2, null]
    ]);
    expect(csp.mapEquals(a, b)).toBe(true);

    const c = new Map([
      [1, a1],
      [2, null]
    ]);
    const d = new Map([
      [1, b1],
      [2, b1]
    ]);
    expect(csp.mapEquals(c, d)).toBe(false);
  });

  it('Map values of undefined', () => {
    // @ts-expect-error: intentionally testing undefined values
    expect(csp.mapEquals(undefined, undefined)).toBe(true);
  });

  // optionalEquals tests

  it('Optional undefined equals undefined', () => {
    expect(csp.optionalEquals(undefined, undefined)).toBe(true);
  });

  it('Optional basic type not equal to undefined', () => {
    const value = 123;

    expect(csp.optionalEquals(value, undefined)).toBe(false);
    expect(csp.optionalEquals(undefined, value)).toBe(false);
  });

  it('Optional full type not equal to undefined', () => {
    using bound = csp.BindingsTestType.create(1, 'one');

    expect(csp.optionalEquals(bound, undefined)).toBe(false);
    expect(csp.optionalEquals(undefined, bound)).toBe(false);
  });

  it('Optional basic type equality', () => {
    const value1 = 123;
    const value2 = 123;

    expect(csp.optionalEquals(value1, value1)).toBe(true);
    expect(csp.optionalEquals(value1, value2)).toBe(true);
  });

  it('Optional full type equality', () => {
    using elem1 = csp.BindingsTestType.create(1, 'one');
    using elem2 = csp.BindingsTestType.create(1, 'one');

    expect(csp.optionalEquals(elem1, elem1)).toBe(true);
    expect(csp.optionalEquals(elem1, elem2)).toBe(true);
  });
});
