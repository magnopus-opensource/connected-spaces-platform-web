import { describe, it, expect, beforeAll } from 'vitest';
import { loadCSP } from '../../loadModule';
import type { MainModule } from 'connected-spaces-platform-bindings';
import { generatedTestAccountDisplayName, makeTestUser, registerLogSystemCallback } from '../testUtils';

const ENDPOINT_ROOT_URI = 'https://ogs.magnopus-dev.cloud';
const TENANT = 'OKO_TESTS';

describe('CSPFoundation User Integrations', () => {
  let csp: MainModule;

  beforeAll(async () => {
    csp = await loadCSP();

    using userAgent = csp.ClientUserAgent.create();
    userAgent.cspVersion = csp.CSPFoundation.getVersion();
    userAgent.clientOs = 'WASMTestsOS';
    userAgent.clientSku = 'connected-spaces-platform-web-tests';
    userAgent.clientVersion = csp.CSPFoundation.getVersion();
    userAgent.clientEnvironment = 'ODev';
    userAgent.chsEnvironment = 'oDev';

    csp.CSPFoundation.initialise(ENDPOINT_ROOT_URI, TENANT, userAgent);

    registerLogSystemCallback(csp);
  });

  afterAll(() => {
    expect(csp.CSPFoundation.shutdown()).toBe(true);
  });

  it('Create Test User', async () => {
    expect(csp.CSPFoundation.getIsInitialised()).toBe(true);
    using profile = await makeTestUser(csp.SystemsManager.get().getUserSystem());
    expect(profile.displayName).toBe(generatedTestAccountDisplayName);
  });
});
