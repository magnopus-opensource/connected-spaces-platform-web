import { expectTypeOf, test } from 'vitest';
import type { MainModule, ClassHandle, BindingsTestType } from 'connected-spaces-platform-bindings';

type DisposeElementFn = MainModule['disposeElement'];
type DisposeArrayFn = MainModule['disposeArray'];
type DisposeMapFn = MainModule['disposeMap'];

describe('Memory type overlay', () => {
  let csp: MainModule = {} as MainModule;

  test('Required memory functions exist', () => {
    expectTypeOf<MainModule>().toHaveProperty('disposeElement');
    expectTypeOf<MainModule>().toHaveProperty('disposeArray');
    expectTypeOf<MainModule>().toHaveProperty('disposeMap');
  });

  // disposeElement

  test('disposeElement does not accept any (type overlay is applied)', () => {
    expectTypeOf<DisposeElementFn>().parameter(0).not.toEqualTypeOf<any>();
  });

  test('disposeElement accepts ClassHandle', () => {
    expectTypeOf<DisposeElementFn>().toBeCallableWith({} as ClassHandle);
  });

  test('disposeElement accepts subtypes of ClassHandle', () => {
    expectTypeOf<DisposeElementFn>().toBeCallableWith({} as BindingsTestType);
  });

  test('disposeElement rejects non-ClassHandle types', () => {
    // @ts-expect-error plain object is not a ClassHandle
    expectTypeOf<DisposeElementFn>().toBeCallableWith({});

    // @ts-expect-error number is not a ClassHandle
    expectTypeOf<DisposeElementFn>().toBeCallableWith(42);

    // @ts-expect-error string is not a ClassHandle
    expectTypeOf<DisposeElementFn>().toBeCallableWith('hello');
  });

  // disposeArray

  test('disposeArray does not accept any (type overlay is applied)', () => {
    expectTypeOf<DisposeArrayFn>().parameter(0).not.toEqualTypeOf<any>();
  });

  test('disposeArray accepts typed arrays', () => {
    expectTypeOf(csp.disposeArray([] as number[])).toEqualTypeOf<void>();
    expectTypeOf(csp.disposeArray([] as string[])).toEqualTypeOf<void>();
    expectTypeOf(csp.disposeArray([] as BindingsTestType[])).toEqualTypeOf<void>();
  });

  test('disposeArray rejects non-array arguments', () => {
    // @ts-expect-error plain object is not an array
    expectTypeOf<DisposeArrayFn>().toBeCallableWith({});

    // @ts-expect-error number is not an array
    expectTypeOf<DisposeArrayFn>().toBeCallableWith(42);
  });

  // disposeMap

  test('disposeMap does not accept any (type overlay is applied)', () => {
    expectTypeOf<DisposeMapFn>().parameter(0).not.toEqualTypeOf<any>();
  });

  test('disposeMap accepts typed Maps', () => {
    expectTypeOf(csp.disposeMap(new Map<string, number>())).toEqualTypeOf<void>();
    expectTypeOf(csp.disposeMap(new Map<number, BindingsTestType>())).toEqualTypeOf<void>();
  });

  test('disposeMap rejects non-Map arguments', () => {
    // @ts-expect-error plain object is not a Map
    expectTypeOf<DisposeMapFn>().toBeCallableWith({});

    // @ts-expect-error array is not a Map
    expectTypeOf<DisposeMapFn>().toBeCallableWith([]);
  });
});
