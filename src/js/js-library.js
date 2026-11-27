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
  constructor(resultCode, httpResultCode, responseBody, failureReason) {
    // Construct a descriptive error message so that error.message contains as much information
    // as possible when logged.

    const MAX_BODY_LENGTH = 500;
    const details = [];

    if (httpResultCode) {
      details.push(`HTTP ${httpResultCode}`);
    }

    if (failureReason > 0) {
      const reasons = Module['ERequestFailureReason'];

      details.push(
        Object.keys(reasons).find((name) => reasons[name] === failureReason) ?? `failure reason ${failureReason}`
      );
    }

    let message = details.length > 0 ? `Request failed (${details.join(', ')})` : 'Request failed';

    // Truncate the response body if too long
    if (responseBody) {
      message += `: ${responseBody.length > MAX_BODY_LENGTH ? `${responseBody.slice(0, MAX_BODY_LENGTH)}...` : responseBody}`;
    }

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

/*
 * As we add disposability to containers in both JS and C++, make a util function here that we can use to do it in both places.
 * Only Arrays, List and Maps need explicit disposal, and Arrays and Lists are the same, both use ArrayDispose.
 */
addToLibrary({
  $attachDisposerToContainer: (target) => {
    let disposerFunc = null;

    if (Array.isArray(target)) {
      disposerFunc = Module['disposeArray'];
    } else if (target instanceof Map) {
      disposerFunc = Module['disposeMap'];
    }

    if (!disposerFunc) {
      console.warn('Attempting to attach disposer to inappropriate or non-container.');
      return target;
    }

    Object.defineProperty(target, Symbol.dispose, {
      value: disposerFunc.bind(undefined, target),
      enumerable: false
    });

    return target;
  }
});
