import { afterAll, afterEach, beforeAll, describe, expect, it } from 'vitest';
import type {
  AssetSystem,
  GLTFMaterial,
  LogSystem,
  MainModule,
  MultiplayerConnection,
  NetworkEventBus,
  ScriptSystem,
  SpaceSystem,
  SystemsManager,
  UserSystem
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

const createTestMaterialName = () => `WASM-INTEROP-TESTMATERIAL-${crypto.randomUUID()}`;

describe(
  'CSP Custom Material Integration Tests',
  () => {
    let csp: MainModule;

    let systemsManager: SystemsManager;
    let assetSystem: AssetSystem;
    let userSystem: UserSystem;
    let eventBus: NetworkEventBus;
    let spaceSystem: SpaceSystem;
    let multiplayerConnection: MultiplayerConnection;
    let logSystem: LogSystem;
    let scriptSystem: ScriptSystem;

    beforeAll(async () => {
      csp = await initCsp();

      systemsManager = csp.SystemsManager.get();

      let userSystemOrNull = systemsManager.getUserSystem();
      if (userSystemOrNull === null) {
        throw new Error('Could not get UserSystem');
      } else {
        userSystem = userSystemOrNull;
      }

      let assetSystemOrNull = systemsManager.getAssetSystem();
      if (assetSystemOrNull === null) {
        throw new Error('Could not get AssetSystem');
      } else {
        assetSystem = assetSystemOrNull;
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

    afterEach(async () => {
      if (userSystem.getLoginState().loginStateValue === csp.ELoginState.LoggedIn) {
        using logoutResult = await userSystem.logout();

        expect(logoutResult.resultCode).toBe(csp.EResultCode.Success);
      }
    });

    afterAll(async () => {
      await multiplayerConnection.setAllowSelfMessagingFlag(false);

      expect(csp.CSPFoundation.shutdown()).toBe(true);
    });

    it('Create and delete custom material', async () => {
      // Create a test user and log in
      using userProfile = await makeTestUser(userSystem);

      await loginTestUser(csp, userSystem, multiplayerConnection, userProfile.email);

      // Create a test space
      using space = await createTestSpace(csp, spaceSystem);

      // ------ Create the test material ------

      const materialName = createTestMaterialName();

      using createMaterialResult = await assetSystem.createMaterial(
        materialName,
        csp.EShaderType.Standard,
        space.id,
        new Map<string, string>(),
        ['material-test-tag']
      );

      expect(createMaterialResult.resultCode).toBe(csp.EResultCode.Success);

      // Materials are client owning pointers, so we need to dispose them and also check for null
      // before use.
      using material = createMaterialResult.getMaterial();

      expect(material).not.toBeNullable();

      if (!material) {
        throw new Error('Material is null');
      }

      expect(material.name).toBe(materialName);
      expect(material.shaderType).toBe(csp.EShaderType.Standard);

      // ------ Delete the test material ------

      const deleteMaterialResult = await assetSystem.deleteMaterial(material);

      expect(deleteMaterialResult.resultCode).toBe(csp.EResultCode.Success);

      // Clean up by deleting the test space
      const deleteSpaceResult = await spaceSystem.deleteSpace(space.id);
      expect(deleteSpaceResult.resultCode).toBe(csp.EResultCode.Success);
    });

    it('Update a custom material', async () => {
      // Create a test user and log in
      using userProfile = await makeTestUser(userSystem);

      await loginTestUser(csp, userSystem, multiplayerConnection, userProfile.email);

      // Create a test space
      using space = await createTestSpace(csp, spaceSystem);

      // Create a custom material

      const materialName = createTestMaterialName();

      using createMaterialResult = await assetSystem.createMaterial(
        materialName,
        csp.EShaderType.Standard,
        space.id,
        new Map<string, string>(),
        ['material-test-tag']
      );
      expect(createMaterialResult.resultCode).toBe(csp.EResultCode.Success);

      // Materials are client owning pointers, so we need to dispose them and also check for null
      // before use
      using material = createMaterialResult.getMaterial();
      expect(material).not.toBeNullable();

      if (!material) {
        throw new Error('Material is null');
      }

      // ------ Update the test material ------

      // We're down-casting the material here so no `using`, otherwise we'd double delete
      const gltfMaterial = material as GLTFMaterial;

      gltfMaterial.alphaMode = csp.EAlphaMode.Blend;
      gltfMaterial.alphaCutoff = 0.75;
      gltfMaterial.doubleSided = true;
      gltfMaterial.baseColorFactor = { x: 1, y: 0.5, z: 0, w: 0.5 };
      gltfMaterial.metallicFactor = 0.5;
      gltfMaterial.roughnessFactor = 0.5;
      gltfMaterial.emissiveFactor = { x: 0, y: 0.5, z: 1 };
      gltfMaterial.emissiveStrength = 1.5;

      const testTextureAssetCollectionId = 'test-asset-collection-id';
      const testTextureAssetId = 'test-asset-id';

      using textureInfo = csp.TextureInfo.create();
      textureInfo.setCollectionAndAssetId(testTextureAssetCollectionId, testTextureAssetId);
      textureInfo.uvOffset = { x: 0.5, y: 0.25 };
      textureInfo.uvRotation = 45;
      textureInfo.uvScale = { x: 2, y: 2 };
      textureInfo.texCoord = 1;
      textureInfo.isStereoFlipped = true;
      textureInfo.stereoVideoType = csp.StereoVideoType.TopBottom;

      gltfMaterial.setBaseColorTexture(textureInfo);

      using updateMaterialResult = await assetSystem.updateMaterial(material);

      expect(updateMaterialResult.resultCode).toBe(csp.EResultCode.Success);

      // ------ Update the test material and re-retrieve it ------

      using getMaterialResult = await assetSystem.getMaterial(material.materialCollectionId, material.materialId);

      expect(getMaterialResult.resultCode).toBe(csp.EResultCode.Success);

      using retrievedMaterial = getMaterialResult.getMaterial();
      expect(retrievedMaterial).not.toBeNullable();

      if (!retrievedMaterial) {
        throw new Error('Retrieved material is null');
      }

      const retrievedGltfMaterial = retrievedMaterial as GLTFMaterial;

      expect(retrievedGltfMaterial.alphaMode).toBe(csp.EAlphaMode.Blend);
      expect(retrievedGltfMaterial.alphaCutoff).toBe(0.75);
      expect(retrievedGltfMaterial.doubleSided).toBe(true);
      expect(retrievedGltfMaterial.baseColorFactor).toEqual({ x: 1, y: 0.5, z: 0, w: 0.5 });
      expect(retrievedGltfMaterial.metallicFactor).toBe(0.5);
      expect(retrievedGltfMaterial.roughnessFactor).toBe(0.5);
      expect(retrievedGltfMaterial.emissiveFactor).toEqual({ x: 0, y: 0.5, z: 1 });
      expect(retrievedGltfMaterial.emissiveStrength).toBe(1.5);

      using baseColorTextureInfo = retrievedGltfMaterial.getBaseColorTexture();
      expect(baseColorTextureInfo).not.toBeNullable();

      if (!baseColorTextureInfo) {
        throw new Error('Base color texture info is null');
      }
      expect(baseColorTextureInfo.assetCollectionId).toBe(testTextureAssetCollectionId);
      expect(baseColorTextureInfo.assetId).toBe(testTextureAssetId);
      expect(baseColorTextureInfo.uvOffset).toEqual({ x: 0.5, y: 0.25 });
      expect(baseColorTextureInfo.uvRotation).toBe(45);
      expect(baseColorTextureInfo.uvScale).toEqual({ x: 2, y: 2 });
      expect(baseColorTextureInfo.texCoord).toBe(1);
      expect(baseColorTextureInfo.isStereoFlipped).toBe(true);
      expect(baseColorTextureInfo.stereoVideoType).toBe(csp.StereoVideoType.TopBottom);

      // Delete the test material
      const deleteMaterialResult = await assetSystem.deleteMaterial(material);
      expect(deleteMaterialResult.resultCode).toBe(csp.EResultCode.Success);

      // Clean up by deleting the test space
      const deleteSpaceResult = await spaceSystem.deleteSpace(space.id);
      expect(deleteSpaceResult.resultCode).toBe(csp.EResultCode.Success);
    });

    it('Material update events are received', async () => {
      // Create a test user and log in
      using userProfile = await makeTestUser(userSystem);

      await loginTestUser(csp, userSystem, multiplayerConnection, userProfile.email);

      // Enable self-messaging to be able receive events
      const setAllowSelfMessagingFlagResult = await multiplayerConnection.setAllowSelfMessagingFlag(true);
      expect(setAllowSelfMessagingFlagResult).toBe(csp.ErrorCode.None);

      // Create and enter a test space
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

      // ------ Listen for material update events ------

      let callbackCalled = false;
      let materialCreatedEventReceived = false;
      let materialUpdatedEventReceived = false;
      let materialDeletedEventReceived = false;

      assetSystem.setMaterialChangedCallback((materialChangedParams) => {
        callbackCalled = true;

        if (materialChangedParams.changeType === csp.EAssetChangeType.Created) {
          materialCreatedEventReceived = true;
        } else if (materialChangedParams.changeType === csp.EAssetChangeType.Updated) {
          materialUpdatedEventReceived = true;
        } else if (materialChangedParams.changeType === csp.EAssetChangeType.Deleted) {
          materialDeletedEventReceived = true;
        }
      });

      // Create a custom material
      const materialName = createTestMaterialName();

      using createMaterialResult = await assetSystem.createMaterial(
        materialName,
        csp.EShaderType.Standard,
        space.id,
        new Map<string, string>(),
        ['material-test-tag']
      );
      expect(createMaterialResult.resultCode).toBe(csp.EResultCode.Success);

      await until(() => callbackCalled);
      callbackCalled = false;

      expect(materialCreatedEventReceived).toBe(true);

      // Materials are client owning pointers, so we need to dispose them and also check for null
      // before use
      using material = createMaterialResult.getMaterial();
      expect(material).not.toBeNullable();

      if (!material) {
        throw new Error('Material is null');
      }

      // Update the test material

      // We're down-casting the material here so no `using`, otherwise we'd double delete
      const gltfMaterial = material as GLTFMaterial;
      gltfMaterial.metallicFactor = 0.5;

      using updateMaterialResult = await assetSystem.updateMaterial(material);
      expect(updateMaterialResult.resultCode).toBe(csp.EResultCode.Success);

      await until(() => callbackCalled);
      callbackCalled = false;

      expect(materialUpdatedEventReceived).toBe(true);

      // Delete the test material

      const deleteMaterialResult = await assetSystem.deleteMaterial(material);
      expect(deleteMaterialResult.resultCode).toBe(csp.EResultCode.Success);

      await until(() => callbackCalled);
      callbackCalled = false;

      expect(materialDeletedEventReceived).toBe(true);

      // Clean up by exiting and deleting the test space
      using exitSpaceResult = await spaceSystem.exitSpace();
      expect(exitSpaceResult.resultCode).toBe(csp.EResultCode.Success);

      const deleteSpaceResult = await spaceSystem.deleteSpace(space.id);
      expect(deleteSpaceResult.resultCode).toBe(csp.EResultCode.Success);
    });
  },
  INTEGRATION_TEST_TIMEOUT_MS
);
