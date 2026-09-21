import { expectTypeOf, test } from 'vitest';
import type {
  CspRequestError,
  ERequestFailureReason,
  EResponseCodes,
  EResultCode,
  MainModule
} from 'connected-spaces-platform-bindings';

/**
 * Type tests for CspRequestError.
 *
 * These exist to ensure that the type overlay for CspRequestError is correctly being applied atop
 * the generated types for the library.
 */
describe('CspRequestError type overlay', () => {
  /*
   * The following tests ensure the CspRequestError type is available on that of MainModule,
   * matching where the actual JavaScript class is exported from in the generated bindings JS file.
   * If this ever needs to change, make sure that the types match what is effectively happening with
   * the JavaScript.
   */

  test('MainModule exposes CspRequestError', () => {
    expectTypeOf<MainModule>().toHaveProperty('CspRequestError');
  });

  test('CspRequestError on MainModule is the class constructor', () => {
    expectTypeOf<MainModule['CspRequestError']>().toEqualTypeOf<typeof CspRequestError>();
  });

  // General type checks for the class. These are less important than the above.

  test('CspRequestError extends Error', () => {
    expectTypeOf<CspRequestError>().toExtend<Error>();
  });

  test('name is the literal "CspRequestError", not the widened string type', () => {
    expectTypeOf<CspRequestError['name']>().toEqualTypeOf<'CspRequestError'>();
  });

  test('resultCode is of type EResultCode', () => {
    expectTypeOf<CspRequestError['resultCode']>().toEqualTypeOf<EResultCode>();
  });

  test('httpResultCode is of type EResponseCodes', () => {
    expectTypeOf<CspRequestError['httpResultCode']>().toEqualTypeOf<EResponseCodes>();
  });

  test('responseBody is of type string', () => {
    expectTypeOf<CspRequestError['responseBody']>().toEqualTypeOf<string>();
  });

  test('failureReason is of type ERequestFailureReason', () => {
    expectTypeOf<CspRequestError['failureReason']>().toEqualTypeOf<ERequestFailureReason>();
  });
});
