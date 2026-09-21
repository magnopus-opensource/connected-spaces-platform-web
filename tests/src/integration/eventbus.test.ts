import { describe, it, expect, beforeAll, afterAll, afterEach } from 'vitest';
import { loadCSP } from '../../loadModule';
import type {
  MainModule,
  SystemsManager,
  UserSystem,
  NetworkEventBus,
  SpaceSystem,
  NetworkEventData,
  MultiplayerConnection,
  LogSystem,
  ScriptSystem,
  OnlineRealtimeEngine,
  ReplicatedValue,
  Space,
  Vector3
} from 'connected-spaces-platform-bindings';
import {
  createTestSpace,
  enterOnlineSpace,
  generatedTestAccountPassword,
  makeTestUser,
  registerLogSystemCallback,
  until
} from '../testUtils';

const ENDPOINT_ROOT_URI = 'https://ogs.magnopus-dev.cloud';
const TENANT = 'OKO_TESTS';

describe('CSP EventBus Integrations', () => {
  let csp: MainModule;
  let systemsManager: SystemsManager;
  let userSystem: UserSystem;
  let eventBus: NetworkEventBus;
  let spaceSystem: SpaceSystem;
  let multiplayerConnection: MultiplayerConnection;
  let logSystem: LogSystem;
  let scriptSystem: ScriptSystem;

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

    systemsManager = csp.SystemsManager.get();

    let userSystemOrNull = systemsManager.getUserSystem();
    if (userSystemOrNull === null) {
      throw new Error('Could not get UserSystem');
    } else {
      userSystem = userSystemOrNull;
    }

    let eventBusOrNull = systemsManager.getEventBus();
    if (eventBusOrNull === null) {
      throw new Error('Could not get EventBus');
    } else {
      eventBus = eventBusOrNull;
    }

    let spaceSystemOrNull = systemsManager.getSpaceSystem();
    if (spaceSystemOrNull === null) {
      throw new Error('Could not get SpaceSystem');
    } else {
      spaceSystem = spaceSystemOrNull;
    }

    let multiplayerConnectionOrNull = systemsManager.getMultiplayerConnection();
    if (multiplayerConnectionOrNull === null) {
      throw new Error('Could not get MultiplayerConnection');
    } else {
      multiplayerConnection = multiplayerConnectionOrNull;
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

    registerLogSystemCallback(csp);
  });

  afterAll(async () => {
    await multiplayerConnection.setAllowSelfMessagingFlag(false);
    expect(csp.CSPFoundation.shutdown()).toBe(true);
  });

  it('Add-Remove callback registrations', async () => {
    // We register to internal events (access control, asset detail chanced, async call completed, etc) as standard,
    // but we only want to measure the delta here.
    let initialEventCount = eventBus.allRegistrations().length;

    let receiverID1 = 'TestReceiverID1';
    let eventName1 = 'TestEventName1';
    let receiverID2 = 'TestReceiverID2';
    let eventName2 = 'TestEventName2';

    eventBus.listenCustomNetworkEvent(receiverID1, eventName1, (networkEventData: NetworkEventData) => {});
    eventBus.listenCustomNetworkEvent(receiverID1, eventName2, (networkEventData: NetworkEventData) => {});
    eventBus.listenCustomNetworkEvent(receiverID2, eventName1, (networkEventData: NetworkEventData) => {});
    expect(eventBus.allRegistrations().length).toBe(initialEventCount + 3);

    eventBus.stopListenCustomNetworkEvent(receiverID1, eventName1);
    expect(eventBus.allRegistrations().length).toBe(initialEventCount + 2);
    eventBus.stopListenCustomNetworkEvent(receiverID1, eventName2);
    expect(eventBus.allRegistrations().length).toBe(initialEventCount + 1);

    eventBus.stopListenAllNetworkEvents(receiverID2);
    expect(eventBus.allRegistrations().length).toBe(initialEventCount + 0);
  });

  it('Reject duplicate callback registration', async () => {
    // We register to internal events (access control, asset detail chanced, async call completed, etc) as standard,
    // but we only want to measure the delta here.
    let initialEventCount = eventBus.allRegistrations().length;

    let receiverID = 'TestReceiverID';
    let eventName = 'TestEventName';

    eventBus.listenCustomNetworkEvent(receiverID, eventName, (networkEventData: NetworkEventData) => {});
    expect(eventBus.allRegistrations().length).toBe(initialEventCount + 1);

    // Should have been rejected, won't be added to internal storage. There will be a log as well ... although let's not bother testing that.
    eventBus.listenCustomNetworkEvent(receiverID, eventName, (networkEventData: NetworkEventData) => {});
    expect(eventBus.allRegistrations().length).toBe(initialEventCount + 1);

    eventBus.stopListenAllNetworkEvents(receiverID);
    expect(eventBus.allRegistrations().length).toBe(initialEventCount + 0);
  });

  it('Send-Receive Custom Event', async () => {
    let profile = await makeTestUser(csp.SystemsManager.get().getUserSystem());

    using loginResult = await userSystem.login(profile.email, generatedTestAccountPassword, true, true);
    expect(loginResult.resultCode).toBe(csp.EResultCode.Success);
    expect(userSystem.getLoginState().loginStateValue).toBe(csp.ELoginState.LoggedIn);

    // Important if we want to get messages from ourself.
    expect(await multiplayerConnection.setAllowSelfMessagingFlag(true)).toBe(csp.ErrorCode.None);

    // Need to be in a scope to send and receive events. We could just enter the scope the space creates via spaceID (connection->SetScopes),
    // but I figure a full entry is probably best in terms of integrative coverage.
    // Pretty nonsensical part of the library tbh, or at least rather confused. How scopes, connections, spaces and events relate to eachother
    // needs to be clearer.
    using newSpace = await createTestSpace(csp, spaceSystem);
    using realtimeEngine = await enterOnlineSpace(
      csp,
      spaceSystem,
      multiplayerConnection,
      logSystem,
      eventBus,
      scriptSystem,
      newSpace
    );

    let receiverID = 'TestReceiverID';
    let eventName = 'TestEventName';
    let receivedVector3: Vector3 = { x: 0, y: 0, z: 0 };
    let callbackFired = false;

    eventBus.listenCustomNetworkEvent(receiverID, eventName, (networkEventData: NetworkEventData) => {
      //Sort of subtle, I think getEventValues is only disposable to support the potential map type that's in it, everything else is a value type. Interesting.
      using eventValues = networkEventData.getEventValues();
      const firstValue = eventValues[0];
      if (firstValue !== undefined) {
        expect(firstValue.replicatedValueType).toBe(csp.ReplicatedValueType.Vector3);
        receivedVector3 = firstValue.getVector3();
      }

      callbackFired = true;
    });

    // You know I tried this with Vector2 first, at at the time I got a "Unsupported, only Vector3 and Vector4 supported" message from the services. Odd and annoying.
    using vector3SentVal = csp.ReplicatedValue.createVector3({ x: 1, y: 2, z: 3 });
    let args: ReplicatedValue[] = [vector3SentVal];
    expect(await eventBus.sendNetworkEvent(eventName, args)).toBe(csp.ErrorCode.None);

    await until(() => callbackFired);
    expect(receivedVector3).toEqual(vector3SentVal.getVector3());

    //Cleanup
    eventBus.stopListenAllNetworkEvents(receiverID);
    using exitResult = await spaceSystem.exitSpace();
    expect(exitResult.resultCode).toBe(csp.EResultCode.Success);
    using deleteResult = await spaceSystem.deleteSpace(newSpace.id);
    expect(deleteResult.resultCode).toBe(csp.EResultCode.Success);
  });
});
