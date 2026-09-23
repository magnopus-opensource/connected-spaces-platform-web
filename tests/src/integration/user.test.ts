import { describe, it, expect, beforeAll, afterAll, afterEach } from 'vitest';
import type { MainModule, SystemsManager, UserSystem } from 'connected-spaces-platform-bindings';
import {
  generatedTestAccountDisplayName,
  generatedTestAccountPassword,
  initCsp,
  INTEGRATION_TEST_TIMEOUT_MS,
  makeTestUser,
  registerLogSystemCallback
} from '../testUtils';

describe(
  'CSP User Integrations',
  () => {
    let csp: MainModule;
    let systemsManager: SystemsManager;
    let userSystem: UserSystem;

    beforeAll(async () => {
      csp = await initCsp();

      systemsManager = csp.SystemsManager.get();

      let userSystemOrNull = systemsManager.getUserSystem();
      if (userSystemOrNull === null) {
        throw new Error('Could not get UserSystem');
      } else {
        userSystem = userSystemOrNull;
      }

      registerLogSystemCallback(csp);
    });

    /*
     * You have to logout before you login again, make sure we do that after each test
     */
    afterEach(async () => {
      if (userSystem.getLoginState().loginStateValue === csp.ELoginState.LoggedIn) {
        using logoutResult = await userSystem.logout();
        expect(logoutResult.resultCode).toBe(csp.EResultCode.Success);
      }
    });

    afterAll(() => {
      expect(csp.CSPFoundation.shutdown()).toBe(true);
    });

    it('Create Test User', async () => {
      expect(csp.CSPFoundation.getIsInitialised()).toBe(true);
      using profile = await makeTestUser(csp.SystemsManager.get().getUserSystem());
      expect(profile.displayName).toBe(generatedTestAccountDisplayName);
    });

    it('Login as Guest', async () => {
      using loginResult = await userSystem.loginAsGuest(false, true);
      expect(loginResult.resultCode).toBe(csp.EResultCode.Success);
    });

    it('Login as User', async () => {
      using profile = await makeTestUser(userSystem);
      using loginResult = await userSystem.login(profile.email, generatedTestAccountPassword, false, true);
      expect(loginResult.resultCode).toBe(csp.EResultCode.Success);
    });

    it('Login as User And Create Multiplayer Connection', async () => {
      using profile = await makeTestUser(userSystem);
      using loginResult = await userSystem.login(profile.email, generatedTestAccountPassword, true, true);
      expect(loginResult.resultCode).toBe(csp.EResultCode.Success);
      expect(systemsManager.getMultiplayerConnection()?.connectionState).toBe(csp.ConnectionState.Connected);
    });

    it('Re-Login Same Account', async () => {
      using profile = await makeTestUser(userSystem);
      using loginResult = await userSystem.login(profile.email, generatedTestAccountPassword, false, true);
      expect(loginResult.resultCode).toBe(csp.EResultCode.Success);
      expect(userSystem.getLoginState().loginStateValue).toBe(csp.ELoginState.LoggedIn);

      using logoutResult = await userSystem.logout();
      expect(logoutResult.resultCode).toBe(csp.EResultCode.Success);
      expect(userSystem.getLoginState().loginStateValue).toBe(csp.ELoginState.LoggedOut);

      using relogResult = await userSystem.login(profile.email, generatedTestAccountPassword, false, true);
      expect(relogResult.resultCode).toBe(csp.EResultCode.Success);
      expect(userSystem.getLoginState().loginStateValue).toBe(csp.ELoginState.LoggedIn);
    });

    it('Fail to login with bad password', async () => {
      using profile = await makeTestUser(userSystem);

      /* A Failed result rejects the promise rather than resolving it, so there is no result handle to dispose. */
      await expect(userSystem.login(profile.email, 'NotTheCorrectPassword', false, true)).rejects.toMatchObject({
        name: 'CspRequestError',
        resultCode: csp.EResultCode.Failed,
        httpResultCode: 403
      });
    });
  },
  INTEGRATION_TEST_TIMEOUT_MS
);
