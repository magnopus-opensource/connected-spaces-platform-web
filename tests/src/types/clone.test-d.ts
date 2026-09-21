import { expectTypeOf, test } from 'vitest';
import type { BindingsTestType, ClassHandle, MainModule } from 'connected-spaces-platform-bindings';

type CloneElementFn = MainModule['cloneElement'];
type CloneArrayFn = MainModule['cloneArray'];
type CloneMapFn = MainModule['cloneMap'];

describe('Clone type overlay', () => {
  let csp: MainModule = {} as MainModule;

  test('Required clone functions exist', () => {
    expectTypeOf<MainModule>().toHaveProperty('cloneElement');
    expectTypeOf<MainModule>().toHaveProperty('cloneArray');
    expectTypeOf<MainModule>().toHaveProperty('cloneMap');
  });

  // cloneElement

  test('cloneElement does not accept any (type overlay is applied)', () => {
    expectTypeOf<CloneElementFn>().parameter(0).not.toEqualTypeOf<any>();
  });

  test('cloneElement accepts ClassHandle', () => {
    expectTypeOf<CloneElementFn>().toBeCallableWith({} as ClassHandle);
  });

  test('cloneElement accepts subtypes of ClassHandle', () => {
    expectTypeOf<CloneElementFn>().toBeCallableWith({} as BindingsTestType);
  });

  test('cloneElement rejects non-ClassHandle types', () => {
    // @ts-expect-error plain object is not a ClassHandle
    expectTypeOf<CloneElementFn>().toBeCallableWith({});

    // @ts-expect-error number is not a ClassHandle
    expectTypeOf<CloneElementFn>().toBeCallableWith(42);

    // @ts-expect-error string is not a ClassHandle
    expectTypeOf<CloneElementFn>().toBeCallableWith('hello');
  });

  test('cloneElement returns the same type as its argument', () => {
    const handle = {} as BindingsTestType;
    expectTypeOf(csp.cloneElement(handle)).toEqualTypeOf<BindingsTestType>();

    const base = {} as ClassHandle;
    expectTypeOf(csp.cloneElement(base)).toEqualTypeOf<ClassHandle>();
  });

  test('cloneElement preserves the generic type parameter from argument to return', () => {
    type BindingsTestTypeClone = (handle: BindingsTestType) => BindingsTestType;

    expectTypeOf<CloneElementFn>().toExtend<BindingsTestTypeClone>();
  });

  // cloneArray

  test('cloneArray does not accept any (type overlay is applied)', () => {
    expectTypeOf<CloneArrayFn>().parameter(0).not.toEqualTypeOf<any>();
  });

  test('cloneArray accepts typed arrays and returns the same array type', () => {
    expectTypeOf(csp.cloneArray([] as number[])).toEqualTypeOf<number[]>();
    expectTypeOf(csp.cloneArray([] as string[])).toEqualTypeOf<string[]>();
    expectTypeOf(csp.cloneArray([] as BindingsTestType[])).toEqualTypeOf<BindingsTestType[]>();
  });

  test('cloneArray rejects non-array arguments', () => {
    // @ts-expect-error plain object is not an array
    expectTypeOf<CloneArrayFn>().toBeCallableWith({});

    // @ts-expect-error number is not an array
    expectTypeOf<CloneArrayFn>().toBeCallableWith(42);

    // @ts-expect-error string is not an array
    expectTypeOf<CloneArrayFn>().toBeCallableWith('hello');
  });

  test('cloneArray infers T from its argument', () => {
    // When called with number[], T is inferred as number and the return type is number[].
    function callCloneArray(fn: CloneArrayFn) {
      return fn([1, 2, 3]);
    }
    type Call = ReturnType<typeof callCloneArray>;

    expectTypeOf<Call>().toEqualTypeOf<number[]>();
  });

  test('cloneArray preserves the generic type parameter from argument to return', () => {
    type NumberCloneArray = (arr: number[]) => number[];

    expectTypeOf<CloneArrayFn>().toExtend<NumberCloneArray>();
  });

  // cloneMap

  test('cloneMap does not accept any (type overlay is applied)', () => {
    expectTypeOf<CloneMapFn>().parameter(0).not.toEqualTypeOf<any>();
  });

  test('cloneMap accepts typed Maps and returns the same map type', () => {
    expectTypeOf(csp.cloneMap(new Map<string, number>())).toEqualTypeOf<Map<string, number>>();
    expectTypeOf(csp.cloneMap(new Map<number, BindingsTestType>())).toEqualTypeOf<Map<number, BindingsTestType>>();
  });

  test('cloneMap rejects non-Map arguments', () => {
    // @ts-expect-error plain object is not a Map
    expectTypeOf<CloneMapFn>().toBeCallableWith({});

    // @ts-expect-error array is not a Map
    expectTypeOf<CloneMapFn>().toBeCallableWith([]);
  });

  test('cloneMap infers K and V from its argument', () => {
    // When called with Map<string, number>, K/V are inferred and the return type is preserved.
    function callCloneMap(fn: CloneMapFn) {
      return fn(new Map<string, number>());
    }
    type Call = ReturnType<typeof callCloneMap>;

    expectTypeOf<Call>().toEqualTypeOf<Map<string, number>>();
  });

  test('cloneMap preserves both K and V type parameters from argument to return', () => {
    type StringNumberCloneMap = (map: Map<string, number>) => Map<string, number>;

    expectTypeOf<CloneMapFn>().toExtend<StringNumberCloneMap>();
  });
});
