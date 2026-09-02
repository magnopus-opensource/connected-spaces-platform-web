/*
 * JavaScript code that is used either inside the library itself or exposed to as part of the public
 * interface. Code can be defined once here and referenced by EM_JS functions for example.
 *
 * Remember to use the EM_JS_DEPS macro at the place of use to ensure referenced JS symbols are
 * included in the build output, as nothing will be included by default.
 *   EM_JS_DEPS(unique_name_for_deps, "$JsSymbol1,$JsSymbol2");
 *
 * Symbols can be added to the public interface by using `__postset`, for example:
 *   addToLibrary({
 *     $JsSymbol1__postset: "Module['JsSymbol1'] = JsSymbol1;",
 *     $JsSymbol1: ...
 *   });
 */

/*
 * Custom error class for failed CSP requests.
 * Designed to be used in conjunction with ResultBase-derived classes that represent failed CSP
 * operations.
 */
class CspRequestError extends Error {
  constructor(message, resultCode, httpResultCode, responseBody, failureReason) {
    super(message);

    if (Error.captureStackTrace) {
      Error.captureStackTrace(this, CspRequestError);
    }

    this.name = 'CspRequestError';

    this.resultCode = resultCode;
    this.httpResultCode = httpResultCode;
    this.responseBody = responseBody;
    this.failureReason = failureReason;
  }
}

addToLibrary({
  // Expose the CspRequestError class to the global module scope so users can use it in expressions
  // such as `if (error instanceof CspRequestError)`.
  $CspRequestError__postset: "Module['CspRequestError'] = CspRequestError;",
  $CspRequestError: CspRequestError
});
