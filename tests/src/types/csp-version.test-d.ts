import { expectTypeOf, test } from 'vitest';
import { cspVersion } from 'connected-spaces-platform-bindings/version';

describe('CSP version', () => {
  test('Is a string', () => {
    expectTypeOf(cspVersion).toEqualTypeOf<string>();
  });
});
