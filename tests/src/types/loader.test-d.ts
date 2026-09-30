import { expectTypeOf, test } from 'vitest';
import type { MainModule } from 'connected-spaces-platform-bindings';
import { loadCsp, type LoadCspOptions } from 'connected-spaces-platform-bindings/loader';

describe('Loader', () => {
  test('Resolves to the same module type as the bindings', () => {
    expectTypeOf(loadCsp).returns.resolves.toEqualTypeOf<MainModule>();
  });

  test('Options are optional', () => {
    expectTypeOf(loadCsp).toBeCallableWith();
    expectTypeOf(loadCsp).toBeCallableWith({});
    expectTypeOf(loadCsp).toBeCallableWith({ debug: true });
    expectTypeOf(loadCsp).toBeCallableWith({ debug: undefined, moduleOptions: {} });
  });

  test('debug must be a boolean', () => {
    expectTypeOf<LoadCspOptions['debug']>().toEqualTypeOf<boolean | undefined>();
  });
});
