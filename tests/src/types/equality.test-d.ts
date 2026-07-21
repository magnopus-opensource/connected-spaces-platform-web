import { expectTypeOf, test } from 'vitest';
import { MainModule } from 'connected-spaces-platform-bindings';

type ArrayEqualsFn = MainModule['arrayEquals'];
type ElementEqualsFn = MainModule['elementEquals'];
type MapEqualsFn = MainModule['mapEquals'];
type OptionalEqualsFn = MainModule['optionalEquals'];

describe('Equality type overlay', () => {
  let csp: MainModule = {} as MainModule;

  test('Required equality functions exist', () => {
    expectTypeOf<MainModule>().toHaveProperty('arrayEquals');
    expectTypeOf<MainModule>().toHaveProperty('elementEquals');
    expectTypeOf<MainModule>().toHaveProperty('mapEquals');
    expectTypeOf<MainModule>().toHaveProperty('optionalEquals');
  });

  // arrayEquals

  test('arrayEquals returns boolean', () => {
    expectTypeOf<ArrayEqualsFn>().returns.toEqualTypeOf<boolean>();
  });

  test('arrayEquals does not accept any (type overlay is applied)', () => {
    expectTypeOf<ArrayEqualsFn>().parameter(0).not.toEqualTypeOf<any>();
  });

  test('arrayEquals infers T from the arguments', () => {
    // When called with number[], T is inferred as number - both params must match
    function callArrayEquals(fn: ArrayEqualsFn) {
      return fn([1], [2]);
    }
    type Call = ReturnType<typeof callArrayEquals>;

    expectTypeOf<Call>().toEqualTypeOf<boolean>();
  });

  test('arrayEquals rejects mismatched array element types', () => {
    // Below are two ways of testing the same thing: that arrayEquals does not allow mismatched
    // array element types. Leaving both here for reference.
    //
    // Note that toBeCallableWith does not work as expected with generic functions, so the
    // following example will pass as the generic T is resolved to unknown, hence the alternative
    // methods used for this test case.
    // ```ts
    //   // Passes, unexpectedly, because T is resolved to unknown by toBeCallableWith
    //   expectTypeOf<ArrayEqualsFn>().toBeCallableWith([1, 2],['a', 'b']);
    // ```

    const fn = {} as unknown as ArrayEqualsFn;
    // @ts-expect-error mismatched types
    fn([1, 2] as number[], ['a'] as string[]);

    // @ts-expect-error mismatched types
    expectTypeOf(csp.arrayEquals([1], ['a'])).toEqualTypeOf<boolean>();
  });

  test('arrayEquals rejects non-array arguments', () => {
    // @ts-expect-error plain values are not arrays
    expectTypeOf<ArrayEqualsFn>().toBeCallableWith(1, 2);
  });

  // elementEquals

  test('elementEquals returns boolean', () => {
    expectTypeOf<ElementEqualsFn>().returns.toEqualTypeOf<boolean>();
  });

  test('elementEquals does not accept any (type overlay is applied)', () => {
    expectTypeOf<ElementEqualsFn>().parameter(0).not.toEqualTypeOf<any>();
  });

  test('elementEquals works with primitive types', () => {
    expectTypeOf(csp.elementEquals(1, 2)).toEqualTypeOf<boolean>();
    expectTypeOf(csp.elementEquals('a', 'b')).toEqualTypeOf<boolean>();
    expectTypeOf(csp.elementEquals(true, false)).toEqualTypeOf<boolean>();
  });

  test('elementEquals works with object types', () => {
    interface Foo {
      x: number;
    }
    const a: Foo = { x: 1 };
    const b: Foo = { x: 2 };

    expectTypeOf<ElementEqualsFn>().toBeCallableWith(a, b);
  });

  test('elementEquals rejects mismatched types', () => {
    // @ts-expect-error mismatched types
    expectTypeOf(csp.elementEquals(1, 'a')).toEqualTypeOf<boolean>();
  });

  // mapEquals

  test('mapEquals returns boolean', () => {
    expectTypeOf<MapEqualsFn>().returns.toEqualTypeOf<boolean>();
  });

  test('mapEquals does not accept any (type overlay is applied)', () => {
    expectTypeOf<MapEqualsFn>().parameter(0).not.toEqualTypeOf<any>();
  });

  test('mapEquals accepts Maps with matching key and value types', () => {
    expectTypeOf(csp.mapEquals(new Map<string, number>(), new Map<string, number>())).toEqualTypeOf<boolean>();
  });

  test('mapEquals rejects Maps with mismatched value types', () => {
    // @ts-expect-error mismatched types
    expectTypeOf(csp.mapEquals(new Map<string, number>(), new Map<string, boolean>())).toEqualTypeOf<boolean>();
  });

  test('mapEquals rejects Maps with mismatched key types', () => {
    // @ts-expect-error mismatched types
    expectTypeOf(csp.mapEquals(new Map<string, number>(), new Map<number, number>())).toEqualTypeOf<boolean>();
  });

  test('mapEquals rejects non-Map arguments', () => {
    // @ts-expect-error mismatched types
    expectTypeOf<MapEqualsFn>().toBeCallableWith({}, {});
  });

  // optionalEquals

  test('optionalEquals returns boolean', () => {
    expectTypeOf<OptionalEqualsFn>().returns.toEqualTypeOf<boolean>();
  });

  test('optionalEquals does not accept any (type overlay is applied)', () => {
    expectTypeOf<OptionalEqualsFn>().parameter(0).not.toEqualTypeOf<any>();
  });

  test('optionalEquals accepts T | undefined for both args', () => {
    expectTypeOf(csp.optionalEquals(42, undefined)).toEqualTypeOf<boolean>();
    expectTypeOf(csp.optionalEquals(undefined, 'hello')).toEqualTypeOf<boolean>();
    expectTypeOf(csp.optionalEquals(undefined, undefined)).toEqualTypeOf<boolean>();
  });

  test('optionalEquals accepts two defined values of the same type', () => {
    expectTypeOf(csp.optionalEquals(1, 2)).toEqualTypeOf<boolean>();
    expectTypeOf(csp.optionalEquals('a', 'b')).toEqualTypeOf<boolean>();
  });

  test('optionalEquals rejects mismatched non-undefined types', () => {
    // @ts-expect-error mismatched types
    expectTypeOf(csp.optionalEquals(1, 'a')).toEqualTypeOf<boolean>();
  });

  // Generic constraints

  test('arrayEquals preserves generic type parameter across arguments', () => {
    // Ensure T is unified - if we explicitly type the call, both args must match T[]
    type NumberArrayEquals = (a: number[], b: number[]) => boolean;

    expectTypeOf<ArrayEqualsFn>().toExtend<NumberArrayEquals>();
  });

  test('mapEquals preserves both K and V type parameters', () => {
    type StringNumberMapEquals = (a: Map<string, number>, b: Map<string, number>) => boolean;

    expectTypeOf<MapEqualsFn>().toExtend<StringNumberMapEquals>();
  });

  test('elementEquals preserves generic type parameter across arguments', () => {
    type StringElementEquals = (a: string, b: string) => boolean;

    expectTypeOf<ElementEqualsFn>().toExtend<StringElementEquals>();
  });

  test('optionalEquals preserves generic type parameter across arguments', () => {
    type BooleanOptionalEquals = (a: boolean | undefined, b: boolean | undefined) => boolean;

    expectTypeOf<OptionalEqualsFn>().toExtend<BooleanOptionalEquals>();
  });
});
