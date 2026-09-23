import { describe, it, expect, beforeAll } from 'vitest';
import type {
  AvatarSpaceComponent,
  CinematicCameraSpaceComponent,
  ExternalLinkSpaceComponent,
  ImageSpaceComponent,
  LightSpaceComponent,
  LogSystem,
  MainModule,
  MultiplayerConnection,
  NetworkEventBus,
  ScriptSystem,
  SpaceSystem,
  SpaceTransform,
  UserSystem
} from 'connected-spaces-platform-bindings';
import {
  createTestSpace,
  enterOnlineSpace,
  generatedTestAccountPassword,
  initCsp,
  makeTestUser,
  registerLogSystemCallback,
  until
} from '../testUtils';

describe('Space Entities', () => {
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
    const setAllowSelfMessagingFlagResult = await multiplayerConnection.setAllowSelfMessagingFlag(false);
    expect(setAllowSelfMessagingFlagResult).toBe(csp.ErrorCode.None);

    if (userSystem.getLoginState().loginStateValue === csp.ELoginState.LoggedIn) {
      using logoutResult = await userSystem.logout();

      expect(logoutResult.resultCode).toBe(csp.EResultCode.Success);
    }
  });

  afterAll(async () => {
    expect(csp.CSPFoundation.shutdown()).toBe(true);
  });

  //================================================================================================

  it('Create avatar', async () => {
    // Create a test user and log in
    using userProfile = await makeTestUser(userSystem);

    using loginResult = await userSystem.login(userProfile.email, generatedTestAccountPassword, true, true);
    expect(loginResult.resultCode).toBe(csp.EResultCode.Success);

    // Create a test space
    using space = await createTestSpace(csp, spaceSystem);

    //Enter the test space

    using realtimeEngine = await enterOnlineSpace(
      csp,
      spaceSystem,
      multiplayerConnection,
      logSystem,
      eventBus,
      scriptSystem,
      space
    );

    // ------ Create an avatar ------

    const testAvatarName = 'TestAvatar';
    const testAvatarId = 'TestAvatarId';

    const spaceTransform: SpaceTransform = {
      position: { x: 1, y: 2, z: 3 },
      rotation: { x: 1, y: 2, z: 3, w: 4 },
      scale: { x: 3, y: 2, z: 1 }
    };

    using avatarEntity = await realtimeEngine.createAvatar(
      testAvatarName,
      userProfile.userId,
      spaceTransform,
      true,
      csp.AvatarState.Idle,
      testAvatarId,
      csp.AvatarPlayMode.Default,
      csp.LocomotionModel.Grounded
    );

    expect(avatarEntity).not.toBeNullable();

    expect(avatarEntity?.id).not.toBe(0);
    expect(avatarEntity?.entityType).toBe(csp.SpaceEntityType.Avatar);
    expect(avatarEntity?.getName()).toBe(testAvatarName);
    expect(avatarEntity?.getPosition()).toEqual(spaceTransform.position);
    expect(avatarEntity?.getRotation()).toEqual(spaceTransform.rotation);
    expect(avatarEntity?.getScale()).toEqual(spaceTransform.scale);

    // ------ Retrieve the avatar component ------

    const componentMap = avatarEntity?.getComponents();

    expect(componentMap).not.toBeNullable();
    expect(componentMap?.size).toBeGreaterThan(0);

    const avatarComponentBase = Array.from(componentMap?.values() || []).find(
      (component) => component?.componentType === csp.ComponentType.AvatarData
    );

    expect(avatarComponentBase).not.toBeNullable();

    const avatarComponent = avatarComponentBase as AvatarSpaceComponent;

    expect(avatarComponent?.avatarId).toBe(testAvatarId);
    expect(avatarComponent?.userId).toBe(userProfile.userId);
    expect(avatarComponent?.state).toBe(csp.AvatarState.Idle);
    expect(avatarComponent?.avatarPlayMode).toBe(csp.AvatarPlayMode.Default);
    expect(avatarComponent?.isVisible).toBe(true);

    // Clean up by exiting and deleting the created space
    using exitResult = await spaceSystem.exitSpace();
    expect(exitResult.resultCode).toBe(csp.EResultCode.Success);

    using spaceDeletionResult = await spaceSystem.deleteSpace(space.id);
    expect(spaceDeletionResult.resultCode).toBe(csp.EResultCode.Success);
  });

  it('Create an entity', async () => {
    // Create a test user and log in
    using userProfile = await makeTestUser(userSystem);

    using loginResult = await userSystem.login(userProfile.email, generatedTestAccountPassword, true, true);
    expect(loginResult.resultCode).toBe(csp.EResultCode.Success);

    // Create a test space
    using space = await createTestSpace(csp, spaceSystem);

    //Enter the test space

    using realtimeEngine = await enterOnlineSpace(
      csp,
      spaceSystem,
      multiplayerConnection,
      logSystem,
      eventBus,
      scriptSystem,
      space
    );

    // ------ Create an entity ------

    const testEntityName = 'TestEntity';

    const spaceTransform: SpaceTransform = {
      position: { x: 1, y: 2, z: 3 },
      rotation: { x: 1, y: 2, z: 3, w: 4 },
      scale: { x: 3, y: 2, z: 1 }
    };

    using testEntity = await realtimeEngine.createEntity(testEntityName, spaceTransform);

    expect(testEntity).not.toBeNullable();

    expect(testEntity?.id).not.toBe(0);
    expect(testEntity?.entityType).toBe(csp.SpaceEntityType.Object);
    expect(testEntity?.getName()).toBe(testEntityName);
    expect(testEntity?.getPosition()).toEqual(spaceTransform.position);
    expect(testEntity?.getRotation()).toEqual(spaceTransform.rotation);
    expect(testEntity?.getScale()).toEqual(spaceTransform.scale);

    expect(realtimeEngine.getAllEntities().length).toBe(1);

    const entityDeleted = await testEntity?.destroy();
    expect(entityDeleted).toBe(true);

    csp.CSPFoundation.tick();

    expect(realtimeEngine.getAllEntities().length).toBe(0);

    // Clean up by exiting and deleting the created space
    using exitResult = await spaceSystem.exitSpace();
    expect(exitResult.resultCode).toBe(csp.EResultCode.Success);

    using spaceDeletionResult = await spaceSystem.deleteSpace(space.id);
    expect(spaceDeletionResult.resultCode).toBe(csp.EResultCode.Success);
  });

  it('Create an entity with components', async () => {
    // Create a test user and log in
    using userProfile = await makeTestUser(userSystem);

    using loginResult = await userSystem.login(userProfile.email, generatedTestAccountPassword, true, true);
    expect(loginResult.resultCode).toBe(csp.EResultCode.Success);

    // Create a test space
    using space = await createTestSpace(csp, spaceSystem);

    //Enter the test space

    using realtimeEngine = await enterOnlineSpace(
      csp,
      spaceSystem,
      multiplayerConnection,
      logSystem,
      eventBus,
      scriptSystem,
      space
    );

    // ------ Create an entity with components ------

    using testEntity = await realtimeEngine.createEntity('TestEntity', {
      position: { x: 1, y: 2, z: 3 },
      rotation: { x: 1, y: 2, z: 3, w: 4 },
      scale: { x: 3, y: 2, z: 1 }
    });
    expect(testEntity).not.toBeNullable();

    if (!testEntity) {
      throw new Error('Failed to create the test entity');
    }

    const component1 = testEntity.addComponent(csp.ComponentType.Light);
    expect(component1).not.toBeNullable();

    if (!component1) {
      throw new Error('Failed to add Light component to the entity');
    }

    const lightComponent = component1 as LightSpaceComponent;
    lightComponent.componentName = 'TestLightComponent';
    lightComponent.lightType = csp.LightType.Point;
    lightComponent.color = { x: 1, y: 2, z: 3 };
    lightComponent.intensity = 1.0;
    lightComponent.range = 10.0;
    lightComponent.isVisible = false;

    const component2 = testEntity.addComponent(csp.ComponentType.Image);
    expect(component2).not.toBeNullable();

    if (!component2) {
      throw new Error('Failed to add Image component to the entity');
    }

    const imageComponent = component2 as ImageSpaceComponent;
    imageComponent.componentName = 'TestImageComponent';
    imageComponent.imageAssetId = 'test-image-asset-id';
    imageComponent.assetCollectionId = 'test-asset-collection-id';
    imageComponent.billboardMode = csp.BillboardMode.YawLockedBillboard;
    imageComponent.displayMode = csp.DisplayMode.DoubleSided;
    imageComponent.isEmissive = true;
    imageComponent.isVisible = false;

    testEntity.queueUpdate();
    csp.CSPFoundation.tick();

    expect(testEntity.getComponents().size).toBe(2);

    // ------ Retrieve the components from the entity ------

    const retrievedComponent1 = testEntity.getComponent(lightComponent.id);
    expect(retrievedComponent1).not.toBeNullable();

    if (!retrievedComponent1) {
      throw new Error('Failed to retrieve Light component from the entity');
    }

    const retrievedLightComponent = retrievedComponent1 as LightSpaceComponent;

    expect(retrievedLightComponent.id).toBe(lightComponent.id);
    expect(retrievedLightComponent.componentName).toBe('TestLightComponent');
    expect(retrievedLightComponent.lightType).toBe(csp.LightType.Point);
    expect(retrievedLightComponent.color).toEqual({ x: 1, y: 2, z: 3 });
    expect(retrievedLightComponent.intensity).toBe(1.0);
    expect(retrievedLightComponent.range).toBe(10.0);
    expect(retrievedLightComponent.isVisible).toBe(false);

    const retrievedComponent2 = testEntity.getComponent(imageComponent.id);
    expect(retrievedComponent2).not.toBeNullable();

    if (!retrievedComponent2) {
      throw new Error('Failed to retrieve Image component from the entity');
    }

    const retrievedImageComponent = retrievedComponent2 as ImageSpaceComponent;

    expect(retrievedImageComponent.id).toBe(imageComponent.id);
    expect(retrievedImageComponent.componentName).toBe('TestImageComponent');
    expect(retrievedImageComponent.imageAssetId).toBe('test-image-asset-id');
    expect(retrievedImageComponent.assetCollectionId).toBe('test-asset-collection-id');
    expect(retrievedImageComponent.billboardMode).toBe(csp.BillboardMode.YawLockedBillboard);
    expect(retrievedImageComponent.displayMode).toBe(csp.DisplayMode.DoubleSided);
    expect(retrievedImageComponent.isEmissive).toBe(true);
    expect(retrievedImageComponent.isVisible).toBe(false);

    const entityDeleted = await testEntity?.destroy();
    expect(entityDeleted).toBe(true);

    // Clean up by exiting and deleting the created space
    using exitResult = await spaceSystem.exitSpace();
    expect(exitResult.resultCode).toBe(csp.EResultCode.Success);

    using spaceDeletionResult = await spaceSystem.deleteSpace(space.id);
    expect(spaceDeletionResult.resultCode).toBe(csp.EResultCode.Success);
  });

  it('The entity fetch complete callback is called', async () => {
    // Create a test user and log in
    using userProfile = await makeTestUser(userSystem);

    using loginResult = await userSystem.login(userProfile.email, generatedTestAccountPassword, true, true);
    expect(loginResult.resultCode).toBe(csp.EResultCode.Success);

    // Create a test space and enter
    using space = await createTestSpace(csp, spaceSystem);

    using realtimeEngine = await enterOnlineSpace(
      csp,
      spaceSystem,
      multiplayerConnection,
      logSystem,
      eventBus,
      scriptSystem,
      space
    );

    // ------ Create some entities ------

    using _testEntity1 = await realtimeEngine.createEntity('TestEntity1', {
      position: { x: 0, y: 0, z: 0 },
      rotation: { x: 0, y: 0, z: 0, w: 1 },
      scale: { x: 1, y: 1, z: 1 }
    });

    using _testEntity2 = await realtimeEngine.createEntity('TestEntity2', {
      position: { x: 1, y: 0, z: 0 },
      rotation: { x: 0, y: 0, z: 0, w: 1 },
      scale: { x: 1, y: 1, z: 1 }
    });

    // ------ Exit the space ------

    using exitResult = await spaceSystem.exitSpace();
    expect(exitResult.resultCode).toBe(csp.EResultCode.Success);

    // ------ Setup the test entity fetch complete callback  and re-enter the space ------

    let entityFetchCompleteCalled = false;
    let fetchedEntityCount = 0;

    using spaceReentryRealtimeEngine = csp.OnlineRealtimeEngine.create(
      multiplayerConnection,
      logSystem,
      eventBus,
      scriptSystem
    );

    spaceReentryRealtimeEngine.setEntityFetchCompleteCallback((entityCount: number) => {
      entityFetchCompleteCalled = true;
      fetchedEntityCount = entityCount;
    });

    using reentryResult = await spaceSystem.enterSpace(space.id, spaceReentryRealtimeEngine);
    expect(reentryResult.resultCode).toBe(csp.EResultCode.Success);

    await until(() => entityFetchCompleteCalled);

    expect(entityFetchCompleteCalled).toBe(true);
    expect(fetchedEntityCount).toBe(2);

    using reentryExitResult = await spaceSystem.exitSpace();
    expect(reentryExitResult.resultCode).toBe(csp.EResultCode.Success);

    // Clean up by deleting the created space
    using spaceDeletionResult = await spaceSystem.deleteSpace(space.id);
    expect(spaceDeletionResult.resultCode).toBe(csp.EResultCode.Success);
  });

  it('The remote entity created callback is called', async () => {
    // Create a test user and log in
    using userProfile = await makeTestUser(userSystem);

    using loginResult = await userSystem.login(userProfile.email, generatedTestAccountPassword, true, true);
    expect(loginResult.resultCode).toBe(csp.EResultCode.Success);

    // Enable self-messaging to be able receive events
    const setAllowSelfMessagingFlagResult = await multiplayerConnection.setAllowSelfMessagingFlag(true);
    expect(setAllowSelfMessagingFlagResult).toBe(csp.ErrorCode.None);

    // Create a test space and enter
    using space = await createTestSpace(csp, spaceSystem);

    using realtimeEngine = await enterOnlineSpace(
      csp,
      spaceSystem,
      multiplayerConnection,
      logSystem,
      eventBus,
      scriptSystem,
      space
    );

    // ------ Set the remote entity created callback ------

    let remoteEntityCreatedCalled = false;
    let createdEntityName: string = 'TestEntity';

    realtimeEngine.setRemoteEntityCreatedCallback((entity) => {
      remoteEntityCreatedCalled = true;

      expect(entity).not.toBeNullable();

      expect(entity?.id).not.toBeNullable();
      expect(entity?.getName()).toBe(createdEntityName);
    });

    // ------ Create test entity ------

    using _testEntity = await realtimeEngine.createEntity(createdEntityName, {
      position: { x: 0, y: 0, z: 0 },
      rotation: { x: 0, y: 0, z: 0, w: 1 },
      scale: { x: 1, y: 1, z: 1 }
    });

    await until(() => remoteEntityCreatedCalled);

    expect(remoteEntityCreatedCalled).toBe(true);

    // Clean up by deleting the created space
    using exitResult = await spaceSystem.exitSpace();
    expect(exitResult.resultCode).toBe(csp.EResultCode.Success);

    using spaceDeletionResult = await spaceSystem.deleteSpace(space.id);
    expect(spaceDeletionResult.resultCode).toBe(csp.EResultCode.Success);
  });

  it('The entity update callback is called for entity updates', async () => {
    // Create a test user and log in
    using userProfile = await makeTestUser(userSystem);

    using loginResult = await userSystem.login(userProfile.email, generatedTestAccountPassword, true, true);
    expect(loginResult.resultCode).toBe(csp.EResultCode.Success);

    // Create a test space and enter
    using space = await createTestSpace(csp, spaceSystem);

    using realtimeEngine = await enterOnlineSpace(
      csp,
      spaceSystem,
      multiplayerConnection,
      logSystem,
      eventBus,
      scriptSystem,
      space
    );

    realtimeEngine.entityPatchRateLimitEnabled = false;

    // ------ Create test entity and set the update callback ------

    using testEntity = await realtimeEngine.createEntity('TestEntity', {
      position: { x: 0, y: 0, z: 0 },
      rotation: { x: 0, y: 0, z: 0, w: 1 },
      scale: { x: 1, y: 1, z: 1 }
    });
    expect(testEntity).not.toBeNullable();

    if (!testEntity) {
      throw new Error('Failed to create test entity');
    }

    let entityUpdatedCalled = false;

    testEntity.setUpdateCallback((entity, updateFlags) => {
      entityUpdatedCalled = true;

      expect(entity).not.toBeNullable();
      expect(entity?.getName()).toBe('TestEntity');

      expect(updateFlags & csp.SpaceEntityUpdateFlags.UPDATE_FLAGS_POSITION).toBeTruthy();

      expect(entity?.getPosition()).toEqual({ x: 4, y: 5, z: 6 });
    });

    testEntity.queueUpdate();
    realtimeEngine.processPendingEntityOperations();
    csp.CSPFoundation.tick();

    // ------ Update the entity ------

    testEntity.setPosition({ x: 4, y: 5, z: 6 });

    testEntity.queueUpdate();
    realtimeEngine.processPendingEntityOperations();

    expect(entityUpdatedCalled).toBe(true);

    // Clean up by deleting the created space
    using exitResult = await spaceSystem.exitSpace();
    expect(exitResult.resultCode).toBe(csp.EResultCode.Success);

    using spaceDeletionResult = await spaceSystem.deleteSpace(space.id);
    expect(spaceDeletionResult.resultCode).toBe(csp.EResultCode.Success);
  });

  it('The entity update callback is called for component updates', async () => {
    // Create a test user and log in
    using userProfile = await makeTestUser(userSystem);

    using loginResult = await userSystem.login(userProfile.email, generatedTestAccountPassword, true, true);
    expect(loginResult.resultCode).toBe(csp.EResultCode.Success);

    // Create a test space and enter
    using space = await createTestSpace(csp, spaceSystem);

    using realtimeEngine = await enterOnlineSpace(
      csp,
      spaceSystem,
      multiplayerConnection,
      logSystem,
      eventBus,
      scriptSystem,
      space
    );

    realtimeEngine.entityPatchRateLimitEnabled = false;

    // ------ Create test entity and component ------

    using testEntity = await realtimeEngine.createEntity('TestEntity', {
      position: { x: 0, y: 0, z: 0 },
      rotation: { x: 0, y: 0, z: 0, w: 1 },
      scale: { x: 1, y: 1, z: 1 }
    });
    expect(testEntity).not.toBeNullable();

    if (!testEntity) {
      throw new Error('Failed to create test entity');
    }

    const component = testEntity.addComponent(csp.ComponentType.CinematicCamera);
    expect(component).not.toBeNullable();

    if (!component) {
      throw new Error('Failed to create test component');
    }

    const cinematicCameraComponent = component as CinematicCameraSpaceComponent;
    cinematicCameraComponent.focalLength = 26;

    testEntity.queueUpdate();
    realtimeEngine.processPendingEntityOperations();
    csp.CSPFoundation.tick();

    // ------ Set up entity update callback ------

    let entityUpdatedCalled = false;

    testEntity.setUpdateCallback((entity, updateFlags, componentUpdateInfo) => {
      entityUpdatedCalled = true;

      expect(updateFlags & csp.SpaceEntityUpdateFlags.UPDATE_FLAGS_COMPONENTS).toBeTruthy();

      expect(componentUpdateInfo.length).toBe(1);
      expect(componentUpdateInfo[0]?.componentId).toBe(cinematicCameraComponent.id);
      expect(componentUpdateInfo[0]?.updateType).toBe(csp.ComponentUpdateType.Update);

      expect(cinematicCameraComponent.focalLength).toBe(32);
    });

    // ------ Update the component ------

    cinematicCameraComponent.focalLength = 32;

    testEntity.queueUpdate();
    realtimeEngine.processPendingEntityOperations();

    expect(entityUpdatedCalled).toBe(true);

    // Clean up by deleting the created space
    using exitResult = await spaceSystem.exitSpace();
    expect(exitResult.resultCode).toBe(csp.EResultCode.Success);

    using spaceDeletionResult = await spaceSystem.deleteSpace(space.id);
    expect(spaceDeletionResult.resultCode).toBe(csp.EResultCode.Success);
  });

  it('Lock an entity', async () => {
    // Create a test user and log in
    using userProfile = await makeTestUser(userSystem);

    using loginResult = await userSystem.login(userProfile.email, generatedTestAccountPassword, true, true);
    expect(loginResult.resultCode).toBe(csp.EResultCode.Success);

    // Create a test space and enter
    using space = await createTestSpace(csp, spaceSystem);

    using realtimeEngine = await enterOnlineSpace(
      csp,
      spaceSystem,
      multiplayerConnection,
      logSystem,
      eventBus,
      scriptSystem,
      space
    );

    realtimeEngine.entityPatchRateLimitEnabled = false;

    // ------ Create test entity ------

    using testEntity = await realtimeEngine.createEntity('TestEntity', {
      position: { x: 1, y: 2, z: 3 },
      rotation: { x: 0, y: 0, z: 0, w: 1 },
      scale: { x: 1, y: 1, z: 1 }
    });
    expect(testEntity).not.toBeNullable();

    if (!testEntity) {
      throw new Error('Failed to create test entity');
    }

    const component = testEntity.addComponent(csp.ComponentType.ExternalLink);
    expect(component).not.toBeNullable();

    if (!component) {
      throw new Error('Failed to create test component');
    }

    const externalLinkComponent = component as ExternalLinkSpaceComponent;
    externalLinkComponent.linkUrl = 'https://www.magnopus.com';

    testEntity.queueUpdate();
    realtimeEngine.processPendingEntityOperations();
    csp.CSPFoundation.tick();

    // ------ Lock the entity for modification ------

    testEntity.lock();

    testEntity.queueUpdate();
    realtimeEngine.processPendingEntityOperations();
    csp.CSPFoundation.tick();

    expect(testEntity.isLocked).toBe(true);

    // ------ Try to update the entity ------

    const setPositionResult = testEntity.setPosition({ x: 1, y: 1, z: 1 });

    expect(setPositionResult).toBe(false);
    expect(testEntity.getPosition()).toEqual({ x: 1, y: 2, z: 3 });

    externalLinkComponent.linkUrl = 'https://www.example.com';

    testEntity.queueUpdate();
    realtimeEngine.processPendingEntityOperations();
    csp.CSPFoundation.tick();

    // Ensure that the component update did not go through due to the entity being locked
    expect(externalLinkComponent.linkUrl).toBe('https://www.magnopus.com');

    // Clean up by deleting the created space
    using exitResult = await spaceSystem.exitSpace();
    expect(exitResult.resultCode).toBe(csp.EResultCode.Success);

    using spaceDeletionResult = await spaceSystem.deleteSpace(space.id);
    expect(spaceDeletionResult.resultCode).toBe(csp.EResultCode.Success);
  });

  it('Create an entity hierarchy', async () => {
    // Create a test user and log in
    using userProfile = await makeTestUser(userSystem);

    using loginResult = await userSystem.login(userProfile.email, generatedTestAccountPassword, true, true);
    expect(loginResult.resultCode).toBe(csp.EResultCode.Success);

    // Create a test space and enter
    using space = await createTestSpace(csp, spaceSystem);

    using realtimeEngine = await enterOnlineSpace(
      csp,
      spaceSystem,
      multiplayerConnection,
      logSystem,
      eventBus,
      scriptSystem,
      space
    );

    realtimeEngine.entityPatchRateLimitEnabled = false;

    // ------ Create test entity ------

    using parentEntity = await realtimeEngine.createEntity('ParentEntity', {
      position: { x: 1, y: 2, z: 3 },
      rotation: { x: 0, y: 0, z: 0, w: 1 },
      scale: { x: 1, y: 1, z: 1 }
    });
    expect(parentEntity).not.toBeNullable();

    if (!parentEntity) {
      throw new Error('Failed to create test entity');
    }

    // parentEntity.queueUpdate();
    // realtimeEngine.processPendingEntityOperations();
    // csp.CSPFoundation.tick();

    // ------ Create child entities ------

    using childEntity1 = await realtimeEngine.createEntity(
      'ChildEntity1',
      {
        position: { x: 4, y: 5, z: 6 },
        rotation: { x: 0, y: 0, z: 0, w: 1 },
        scale: { x: 1, y: 1, z: 1 }
      },
      parentEntity.id
    );
    expect(childEntity1).not.toBeNullable();

    if (!childEntity1) {
      throw new Error('Failed to create child entity 1');
    }

    using childEntity2 = await parentEntity.createChildEntity('ChildEntity2', {
      position: { x: -1, y: -2, z: -3 },
      rotation: { x: 0, y: 0, z: 0, w: 1 },
      scale: { x: 1, y: 1, z: 1 }
    });
    expect(childEntity2).not.toBeNullable();

    if (!childEntity2) {
      throw new Error('Failed to create child entity 2');
    }

    parentEntity.queueUpdate();
    realtimeEngine.processPendingEntityOperations();
    csp.CSPFoundation.tick();

    expect(childEntity1.getParentEntity()?.id).toBe(parentEntity.id);
    expect(childEntity2.getParentEntity()?.id).toBe(parentEntity.id);

    // ------ Get child entities ------

    const childEntities = parentEntity.getChildEntities();

    const retrievedChildEntity1 = childEntities.find((entity) => entity?.id === childEntity1.id);
    expect(retrievedChildEntity1).not.toBeNullable();

    const retrievedChildEntity2 = childEntities.find((entity) => entity?.id === childEntity2.id);
    expect(retrievedChildEntity2).not.toBeNullable();

    expect(childEntity1.globalPosition).toEqual({ x: 5, y: 7, z: 9 });
    expect(childEntity2.globalPosition).toEqual({ x: 0, y: 0, z: 0 });

    // Clean up by deleting the created space
    using exitResult = await spaceSystem.exitSpace();
    expect(exitResult.resultCode).toBe(csp.EResultCode.Success);

    using spaceDeletionResult = await spaceSystem.deleteSpace(space.id);
    expect(spaceDeletionResult.resultCode).toBe(csp.EResultCode.Success);
  });
});
