import { describe, it, expect, beforeAll } from 'vitest';
import type {
  LogSystem,
  MainModule,
  MultiplayerConnection,
  NetworkEventBus,
  ScriptSystem,
  SpaceSystem,
  UserSystem
} from 'connected-spaces-platform-bindings';
import {
  generatedTestAccountPassword,
  generateTestSpaceName,
  initCsp,
  makeTestUser,
  registerLogSystemCallback
} from '../testUtils';

describe('Space', () => {
  let csp: MainModule;

  let spaceSystem: SpaceSystem;
  let userSystem: UserSystem;
  let logSystem: LogSystem;
  let scriptSystem: ScriptSystem;
  let multiplayerConnection: MultiplayerConnection;
  let eventBus: NetworkEventBus;

  beforeAll(async () => {
    csp = await initCsp();

    const systemsManager = csp.SystemsManager.get();

    let userSystemOrNull = systemsManager.getUserSystem();
    if (userSystemOrNull === null) {
      throw new Error('Could not get UserSystem');
    } else {
      userSystem = userSystemOrNull;
    }

    let spaceSystemOrNull = systemsManager.getSpaceSystem();
    if (spaceSystemOrNull === null) {
      throw new Error('Could not get SpaceSystem');
    } else {
      spaceSystem = spaceSystemOrNull;
    }

    let logSystemOrNull = systemsManager.getLogSystem();
    if (logSystemOrNull === null) {
      throw new Error('Could not get LogSystem');
    } else {
      logSystem = logSystemOrNull;
    }

    let scriptSystemOrNull = systemsManager.getScriptSystem();
    if (scriptSystemOrNull === null) {
      throw new Error('Could not get ScriptSystem');
    } else {
      scriptSystem = scriptSystemOrNull;
    }

    let multiplayerConnectionOrNull = systemsManager.getMultiplayerConnection();
    if (multiplayerConnectionOrNull === null) {
      throw new Error('Could not get MultiplayerConnection');
    } else {
      multiplayerConnection = multiplayerConnectionOrNull;
    }

    let eventBusOrNull = systemsManager.getEventBus();
    if (eventBusOrNull === null) {
      throw new Error('Could not get EventBus');
    } else {
      eventBus = eventBusOrNull;
    }

    registerLogSystemCallback(csp);
  });

  afterEach(async () => {
    if (userSystem.getLoginState().loginStateValue === csp.ELoginState.LoggedIn) {
      using logoutResult = await userSystem.logout();

      expect(logoutResult.resultCode).toBe(csp.EResultCode.Success);
    }
  });

  afterAll(() => {
    expect(csp.CSPFoundation.shutdown()).toBe(true);
  });

  //================================================================================================

  it('Create a space', async ({ task }) => {
    // Create a test user and log in
    using userProfile = await makeTestUser(userSystem);

    using loginResult = await userSystem.login(userProfile.email, generatedTestAccountPassword, true, true);
    expect(loginResult.resultCode).toBe(csp.EResultCode.Success);

    // ------ Create a test space ------

    const spaceName = generateTestSpaceName();
    const spaceDescription = `WASM Test - ${task.name}`;

    using spaceResult = await spaceSystem.createSpace(
      spaceName,
      spaceDescription,
      csp.SpaceAttributes.Public,
      undefined,
      new Map<string, string>()
    );

    expect(spaceResult.resultCode).toBe(csp.EResultCode.Success);

    using space = spaceResult.getSpace();

    expect(space.id).not.toBe('');
    expect(space.name).toBe(spaceName);
    expect(space.attributes).toBe(csp.SpaceAttributes.Public);
    expect(space.ownerId).toBe(userProfile.userId);
    expect(space.description).toBe(spaceDescription);

    // Clean up by deleting the created space

    using spaceDeletionResult = await spaceSystem.deleteSpace(space.id);
    expect(spaceDeletionResult.resultCode).toBe(csp.EResultCode.Success);
  });

  it('Update space metadata', async ({ task }) => {
    // Create a test user and log in
    using userProfile = await makeTestUser(userSystem);

    using loginResult = await userSystem.login(userProfile.email, generatedTestAccountPassword, true, true);
    expect(loginResult.resultCode).toBe(csp.EResultCode.Success);

    // Create a test space
    const spaceName = generateTestSpaceName();
    const spaceDescription = `WASM Test - ${task.name}`;

    using spaceResult = await spaceSystem.createSpace(
      spaceName,
      spaceDescription,
      csp.SpaceAttributes.Public,
      undefined,
      new Map<string, string>()
    );

    expect(spaceResult.resultCode).toBe(csp.EResultCode.Success);

    using space = spaceResult.getSpace();

    // ------ Update the space metadata ------

    const newSpaceMetadata = new Map<string, string>([
      ['testKey1', 'value1'],
      ['testKey2', 'value2']
    ]);

    using spaceUpdateResult = await spaceSystem.updateSpaceMetadata(space.id, newSpaceMetadata);
    expect(spaceUpdateResult.resultCode).toBe(csp.EResultCode.Success);

    using spaceMetadataResult = await spaceSystem.getSpaceMetadata(space.id);
    expect(spaceMetadataResult.resultCode).toBe(csp.EResultCode.Success);

    const updatedSpaceMetadata = spaceMetadataResult.metadata;
    expect(csp.mapEquals(updatedSpaceMetadata, newSpaceMetadata)).toEqual(true);

    // Clean up by deleting the created space
    using spaceDeletionResult = await spaceSystem.deleteSpace(space.id);
    expect(spaceDeletionResult.resultCode).toBe(csp.EResultCode.Success);
  });

  it('Update space geo-location', async ({ task }) => {
    // Create a test user and log in
    using userProfile = await makeTestUser(userSystem);

    using loginResult = await userSystem.login(userProfile.email, generatedTestAccountPassword, true, true);
    expect(loginResult.resultCode).toBe(csp.EResultCode.Success);

    // Create a test space
    const spaceName = generateTestSpaceName();
    const spaceDescription = `WASM Test - ${task.name}`;

    using spaceResult = await spaceSystem.createSpace(
      spaceName,
      spaceDescription,
      csp.SpaceAttributes.Public,
      undefined,
      new Map<string, string>()
    );

    expect(spaceResult.resultCode).toBe(csp.EResultCode.Success);

    using space = spaceResult.getSpace();

    // ------ Update the space geo-location ------

    using spaceGeoLocation = csp.GeoLocation.create();
    spaceGeoLocation.latitude = 55.86126016;
    spaceGeoLocation.longitude = -4.25066902;

    using spaceUpdateGeoLocationResult = await spaceSystem.updateSpaceGeoLocation(space.id, spaceGeoLocation, 180);
    expect(spaceUpdateGeoLocationResult.resultCode).toBe(csp.EResultCode.Success);

    using spaceGeoLocationResult = await spaceSystem.getSpaceGeoLocation(space.id);
    expect(spaceGeoLocationResult.resultCode).toBe(csp.EResultCode.Success);

    using updatedSpaceGeoLocation = spaceGeoLocationResult.getSpaceGeoLocation();
    using updateSpaceLocation = updatedSpaceGeoLocation.getLocation();
    expect(updateSpaceLocation.latitude).toBeCloseTo(spaceGeoLocation.latitude);
    expect(updateSpaceLocation.longitude).toBeCloseTo(spaceGeoLocation.longitude);
    expect(updatedSpaceGeoLocation.orientation).toBeCloseTo(180);

    // Clean up by deleting the created space
    using spaceDeletionResult = await spaceSystem.deleteSpace(space.id);
    expect(spaceDeletionResult.resultCode).toBe(csp.EResultCode.Success);
  });

  it('Enter and exit a space', async ({ task }) => {
    // Create a test user and log in
    using userProfile = await makeTestUser(userSystem);

    using loginResult = await userSystem.login(userProfile.email, generatedTestAccountPassword, true, true);
    expect(loginResult.resultCode).toBe(csp.EResultCode.Success);

    // Create a test space
    const spaceName = generateTestSpaceName();
    const spaceDescription = `WASM Test - ${task.name}`;

    using spaceResult = await spaceSystem.createSpace(
      spaceName,
      spaceDescription,
      csp.SpaceAttributes.Public,
      undefined,
      new Map<string, string>()
    );
    expect(spaceResult.resultCode).toBe(csp.EResultCode.Success);

    using space = spaceResult.getSpace();

    // ------ Enter the space ------

    using realtimeEngine = csp.OnlineRealtimeEngine.create(multiplayerConnection, logSystem, eventBus, scriptSystem);

    realtimeEngine.setEntityFetchCompleteCallback((entityCount: number) => {
      // Do nothing for now
    });

    using enterResult = await spaceSystem.enterSpace(space.id, realtimeEngine);
    expect(enterResult.resultCode).toBe(csp.EResultCode.Success);

    // ------ Exit the space ------

    using exitResult = await spaceSystem.exitSpace();
    expect(exitResult.resultCode).toBe(csp.EResultCode.Success);

    // Clean up by deleting the created space
    using spaceDeletionResult = await spaceSystem.deleteSpace(space.id);
    expect(spaceDeletionResult.resultCode).toBe(csp.EResultCode.Success);
  });
});
