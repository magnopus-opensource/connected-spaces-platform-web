import { it, expect, beforeAll } from 'vitest';
import { loadCSP } from '../loadModule';
import type { MainModule, StringResultTestType } from 'connected-spaces-platform-bindings';
import { describeOnAndOffThread, untilCallbacksSettled } from './testUtils';

/*
 * Test ResultBase bindings integration with callbacks.
 */

describeOnAndOffThread('ResultBase Callbacks', (offThread, modelLabel) => {
  let csp: MainModule;

  beforeAll(async () => {
    csp = await loadCSP();
  });

  beforeEach(async () => {
    await untilCallbacksSettled(csp);
  });

  // ----------
  // Standard callbacks

  it(`Callback with ResultBase succeeded (${modelLabel})`, async () => {
    using helper = csp.CallbacksBindingMechanismsTestType.create(offThread);

    let callbackCalled = false;

    helper.callbackFunctionStringResultSucceeded((result) => {
      callbackCalled = true;

      expect(result.resultCode).toBe(csp.EResultCode.Success);
      expect(result.httpResultCode).toBe(csp.EResponseCodes.ResponseOK);
      expect(result.value).toBe('Success');
    });

    await untilCallbacksSettled(csp, () => callbackCalled);

    expect(callbackCalled).toBe(true);
  });

  it(`Callback with ResultBase failed (${modelLabel})`, async () => {
    using helper = csp.CallbacksBindingMechanismsTestType.create(offThread);

    let callbackCalled = false;

    helper.callbackFunctionStringResultFailed((result) => {
      callbackCalled = true;

      expect(result.resultCode).toBe(csp.EResultCode.Failed);
      expect(result.httpResultCode).toBe(csp.EResponseCodes.ResponseUnauthorized);
      expect(result.value).toBe('Failure');
    });

    await untilCallbacksSettled(csp, () => callbackCalled);

    expect(callbackCalled).toBe(true);
  });

  // ----------
  // Async callbacks

  it(`Await ResultBase succeeded result (${modelLabel})`, async () => {
    using helper = csp.CallbacksBindingMechanismsTestType.create(offThread);

    using result = await helper.callbackFunctionStringResultSucceededAsync();

    expect(result.resultCode).toBe(csp.EResultCode.Success);
    expect(result.httpResultCode).toBe(csp.EResponseCodes.ResponseOK);
    expect(result.value).toBe('Success');
  });

  it(`Await ResultBase failed result (${modelLabel})`, async () => {
    using helper = csp.CallbacksBindingMechanismsTestType.create(offThread);

    let callbackThrew = false;

    try {
      using result = await helper.callbackFunctionStringResultFailedAsync();
    } catch (error) {
      callbackThrew = true;

      // @ts-expect-error CspRequestError type not defined in bindings so it is unknown
      expect(error.failureReason).toBe(csp.ERequestFailureReason.UserMissingPassword);
      // @ts-expect-error Same as above
      expect(error.resultCode).toBe(csp.EResultCode.Failed);
      // @ts-expect-error Same as above
      expect(error.httpResultCode).toBe(csp.EResponseCodes.ResponseUnauthorized);
    }

    expect(callbackThrew).toBe(true);
  });

  // Async results with progress

  it(`Await ResultBase with progress (${modelLabel})`, async () => {
    using helper = csp.CallbacksBindingMechanismsTestType.create(offThread);

    let progressReportCount = 0;

    using result = await helper.callbackFunctionStringResultProgressAsync((progress) => {
      expect(progress).toBeCloseTo(progressReportCount / 4, 2);

      ++progressReportCount;
    });

    await untilCallbacksSettled(csp);

    expect(result.resultCode).toBe(csp.EResultCode.Success);
    expect(result.httpResultCode).toBe(csp.EResponseCodes.ResponseOK);
    expect(result.value).toBe('Success');

    expect(progressReportCount).toBe(4);
  });
});
