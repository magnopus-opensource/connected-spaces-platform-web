import { describe, it, expect, beforeAll, afterAll, afterEach, createExpect } from 'vitest';
import type {
  MainModule,
  SystemsManager,
  UserSystem,
  ScriptSystem,
  SpaceSystem,
  NetworkEventBus,
  MultiplayerConnection,
  LogSystem,
  ScriptSpaceComponent,
  AnimatedModelSpaceComponent,
  SpaceEntity,
  ComponentUpdateInfo
} from 'connected-spaces-platform-bindings';
import {
  createTestSpace,
  enterOnlineSpace,
  initCsp,
  INTEGRATION_TEST_TIMEOUT_MS,
  loginTestUser,
  makeTestUser,
  registerLogSystemCallback,
  until
} from '../testUtils';

describe(
  'CSP Script Integrations',
  () => {
    let csp: MainModule;
    let systemsManager: SystemsManager;
    let userSystem: UserSystem;
    let scriptSystem: ScriptSystem;
    let spaceSystem: SpaceSystem;
    let eventBus: NetworkEventBus;
    let multiplayerConnection: MultiplayerConnection;
    let logSystem: LogSystem;

    beforeAll(async () => {
      csp = await initCsp();

      systemsManager = csp.SystemsManager.get();

      let userSystemOrNull = systemsManager.getUserSystem();
      if (userSystemOrNull === null) {
        throw new Error('Could not get UserSystem');
      } else {
        userSystem = userSystemOrNull;
      }

      let scriptSystemOrNull = systemsManager.getScriptSystem();
      if (scriptSystemOrNull === null) {
        throw new Error('Could not get ScriptSystem');
      } else {
        scriptSystem = scriptSystemOrNull;
      }

      let spaceSystemOrNull = systemsManager.getSpaceSystem();
      if (spaceSystemOrNull === null) {
        throw new Error('Could not get SpaceSystem');
      } else {
        spaceSystem = spaceSystemOrNull;
      }

      let eventBusOrNull = systemsManager.getEventBus();
      if (eventBusOrNull === null) {
        throw new Error('Could not get EventBus');
      } else {
        eventBus = eventBusOrNull;
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

      registerLogSystemCallback(csp);
    });

    /*
     * You have to logout before you login again, make sure we do that after each test
     */
    afterEach(async () => {
      using loginState = userSystem.getLoginState();

      if (loginState.loginStateValue === csp.ELoginState.LoggedIn) {
        using logoutResult = await userSystem.logout();

        expect(logoutResult.resultCode).toBe(csp.EResultCode.Success);
      }
    });

    afterAll(() => {
      expect(csp.CSPFoundation.shutdown()).toBe(true);
    });

    it('Run Script', async () => {
      using profile = await makeTestUser(userSystem);

      await loginTestUser(csp, userSystem, multiplayerConnection, profile.email);

      let scriptSystemReady: boolean = false;

      using newSpace = await createTestSpace(csp, spaceSystem);
      using realtimeEngine = await enterOnlineSpace(
        csp,
        spaceSystem,
        multiplayerConnection,
        logSystem,
        eventBus,
        scriptSystem,
        newSpace,
        (success) => {
          scriptSystemReady = success;
        }
      );

      // Wait for the script system to be ready after we've entered the space
      await until(() => scriptSystemReady);

      // Script we'll use. We'll create an animated model component and have the script change its position.
      let scriptText = `
        var entities = TheEntitySystem.getEntities();
        var entityIndex = TheEntitySystem.getIndexOfEntity(ThisEntity.id);

        globalThis.onTick = () => {
          var model = entities[entityIndex].getAnimatedModelComponents()[0];
          model.position = [10, 10, 10];
        }

        ThisEntity.subscribeToMessage("entityTick", "onTick");
      `;

      let createdEntity = await realtimeEngine.createEntity(
        'EntityName',
        {
          position: { x: 0, y: 0, z: 0 },
          rotation: { x: 0, y: 0, z: 0, w: 1 },
          scale: { x: 1, y: 1, z: 1 }
        },
        undefined
      );

      expect(createdEntity).not.toBe(null);

      let animatedModelComponent = createdEntity?.addComponent(csp.ComponentType.AnimatedModel);
      let scriptComponent = createdEntity?.addComponent(csp.ComponentType.ScriptData); // Also run the script on this object
      expect(scriptComponent).not.toBe(null);

      createdEntity?.queueUpdate();
      realtimeEngine.processPendingEntityOperations();

      (scriptComponent as ScriptSpaceComponent).scriptSource = scriptText;
      createdEntity?.getScript().invoke();
      csp.CSPFoundation.tick();

      expect(createdEntity?.getScript().hasError()).toBe(false);

      expect((animatedModelComponent as AnimatedModelSpaceComponent).position).toStrictEqual({ x: 10, y: 10, z: 10 });

      // Cleanup
      using exitResult = await spaceSystem.exitSpace();
      expect(exitResult.resultCode).toBe(csp.EResultCode.Success);
      using deleteResult = await spaceSystem.deleteSpace(newSpace.id);
      expect(deleteResult.resultCode).toBe(csp.EResultCode.Success);
    });

    it('Scripts Can Log', async () => {
      using profile = await makeTestUser(userSystem);

      await loginTestUser(csp, userSystem, multiplayerConnection, profile.email);

      let scriptSystemReady: boolean = false;

      using newSpace = await createTestSpace(csp, spaceSystem);
      using realtimeEngine = await enterOnlineSpace(
        csp,
        spaceSystem,
        multiplayerConnection,
        logSystem,
        eventBus,
        scriptSystem,
        newSpace,
        (success) => {
          scriptSystemReady = success;
        }
      );

      // Wait for the script system to be ready after we've entered the space
      await until(() => scriptSystemReady);

      // Script we'll use. Log something
      let scriptText = `
        var entities = TheEntitySystem.getEntities();
        var entityIndex = TheEntitySystem.getIndexOfEntity(ThisEntity.id);

        globalThis.onTick = () => {
          OKO.Log('onTick Called');
        }

        ThisEntity.subscribeToMessage("entityTick", "onTick");
      `;

      let createdEntity = await realtimeEngine.createEntity(
        'EntityName',
        {
          position: { x: 0, y: 0, z: 0 },
          rotation: { x: 0, y: 0, z: 0, w: 1 },
          scale: { x: 1, y: 1, z: 1 }
        },
        undefined
      );

      expect(createdEntity).not.toBe(null);

      let scriptComponent = createdEntity?.addComponent(csp.ComponentType.ScriptData); // Run the script on this object
      expect(scriptComponent).not.toBe(null);

      createdEntity?.queueUpdate();
      realtimeEngine.processPendingEntityOperations();

      // Capture what CSP logs so we can assert on the script's OKO.Log call.
      let capturedLogs: string[] = [];
      logSystem.setLogCallback((level, message) => {
        if (level === csp.LogLevel.Log) {
          capturedLogs.push(message);
        }
      });

      try {
        (scriptComponent as ScriptSpaceComponent).scriptSource = scriptText;
        createdEntity?.getScript().invoke();
        csp.CSPFoundation.tick();

        expect(capturedLogs.some((message) => message.trim() === 'onTick Called')).toBe(true);
      } finally {
        // Put the console-routing callback back for the rest of the suite.
        registerLogSystemCallback(csp);
      }

      // Cleanup
      using exitResult = await spaceSystem.exitSpace();
      expect(exitResult.resultCode).toBe(csp.EResultCode.Success);
      using deleteResult = await spaceSystem.deleteSpace(newSpace.id);
      expect(deleteResult.resultCode).toBe(csp.EResultCode.Success);
    });

    it('Script Changes Invoke Entity Update Callbacks', async () => {
      using profile = await makeTestUser(userSystem);

      await loginTestUser(csp, userSystem, multiplayerConnection, profile.email);

      let scriptSystemReady: boolean = false;

      using newSpace = await createTestSpace(csp, spaceSystem);
      using realtimeEngine = await enterOnlineSpace(
        csp,
        spaceSystem,
        multiplayerConnection,
        logSystem,
        eventBus,
        scriptSystem,
        newSpace,
        (success) => {
          scriptSystemReady = success;
        }
      );

      // We make a bunch of quick ticks, normally these things happen on a frame-timer.
      realtimeEngine.entityPatchRateLimitEnabled = false;

      // Wait for the script system to be ready after we've entered the space
      await until(() => scriptSystemReady);

      // Script we'll use. We'll create an animated model component and have the script change its position.
      let scriptText = `
        var entities = TheEntitySystem.getEntities();
        var entityIndex = TheEntitySystem.getIndexOfEntity(ThisEntity.id);

        globalThis.onTick = () => {
          var model = entities[entityIndex].getAnimatedModelComponents()[0];
          model.position = [10, 10, 10];
        }

        ThisEntity.subscribeToMessage("entityTick", "onTick");
      `;

      let createdEntity = await realtimeEngine.createEntity(
        'EntityName',
        {
          position: { x: 0, y: 0, z: 0 },
          rotation: { x: 0, y: 0, z: 0, w: 1 },
          scale: { x: 1, y: 1, z: 1 }
        },
        undefined
      );

      expect(createdEntity).not.toBe(null);

      let animatedModelComponent = createdEntity?.addComponent(csp.ComponentType.AnimatedModel);
      let scriptComponent = createdEntity?.addComponent(csp.ComponentType.ScriptData); // Also run the script on this object
      expect(scriptComponent).not.toBe(null);

      createdEntity?.queueUpdate();
      realtimeEngine.processPendingEntityOperations();

      (scriptComponent as ScriptSpaceComponent).scriptSource = scriptText;
      createdEntity?.queueUpdate();
      realtimeEngine.processPendingEntityOperations();

      let entityUpdates: {
        entity: SpaceEntity | null;
        updateFlags: number;
        componentUpdates: ComponentUpdateInfo[];
      }[] = [];

      createdEntity?.setUpdateCallback(
        (entity: SpaceEntity | null, updateFlags: number, componentUpdates: ComponentUpdateInfo[]) => {
          entityUpdates.push({
            entity: entity,
            updateFlags: updateFlags,
            componentUpdates: componentUpdates
          });
        }
      );

      createdEntity?.getScript().invoke();
      csp.CSPFoundation.tick(); // Runs the script, which dirties the property
      csp.CSPFoundation.tick(); // Dirty property callback is emitted next tick .... not sure I like this design, no way around the latency.

      let [update] = entityUpdates;
      if (!update) {
        throw new Error('Entity update callback never fired');
      }

      expect(entityUpdates.length).toBe(1);
      expect(update.entity).not.toBeNull();
      expect(update.entity?.id).toBe(createdEntity?.id);
      expect(update.updateFlags & csp.SpaceEntityUpdateFlags.UPDATE_FLAGS_COMPONENTS).toBeTruthy();
      expect(update.componentUpdates.length).toBe(1);
      expect(update.componentUpdates[0]?.updateType).toBe(csp.ComponentUpdateType.Update);
      expect(update.componentUpdates[0]?.componentId).toBe(animatedModelComponent?.id);

      // Cleanup
      using exitResult = await spaceSystem.exitSpace();
      expect(exitResult.resultCode).toBe(csp.EResultCode.Success);
      using deleteResult = await spaceSystem.deleteSpace(newSpace.id);
      expect(deleteResult.resultCode).toBe(csp.EResultCode.Success);
    });
  },
  INTEGRATION_TEST_TIMEOUT_MS
);
