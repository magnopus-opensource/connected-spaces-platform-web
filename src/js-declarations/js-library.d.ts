/**
 * Declarations for the public API that src/js/js-library.js injects into the module.
 */

// @ts-expect-error Error with import as this is the path for the installed version of the generated
// bindings JS file.
import type { ERequestFailureReason, EResponseCodes, EResultCode } from '../connected-spaces-platform-bindings.js';

/**
 * Error thrown for failed CSP requests. Used in conjunction with ResultBase-derived classes that
 * represent failed CSP operations, and exposed on the module so it can be used in expressions such
 * as `error instanceof csp.CspRequestError`.
 */
declare class CspRequestError extends Error {
  readonly name: 'CspRequestError';
  readonly resultCode: EResultCode;
  readonly httpResultCode: EResponseCodes;
  readonly responseBody: string;
  readonly failureReason: ERequestFailureReason;
}

export type { CspRequestError };

export interface CspRequestErrorDeclarations {
  readonly CspRequestError: typeof CspRequestError;
}
