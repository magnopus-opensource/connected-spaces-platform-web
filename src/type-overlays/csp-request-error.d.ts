/**
 * Type overlays for CspRequestError
 */

// @ts-expect-error Error with import as this is the path for the installed version of the generated
// bindings JS file.
import type { ERequestFailureReason, EResponseCodes, EResultCode } from '../connected-spaces-platform-bindings.js';

declare class CspRequestError extends Error {
  readonly name: 'CspRequestError';
  readonly resultCode: EResultCode;
  readonly httpResultCode: EResponseCodes;
  readonly responseBody: string;
  readonly failureReason: ERequestFailureReason;
}

export type { CspRequestError };

export interface CspRequestErrorOverrides {
  readonly CspRequestError: typeof CspRequestError;
}
