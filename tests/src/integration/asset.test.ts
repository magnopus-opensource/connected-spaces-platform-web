import { describe, it, expect, beforeAll, afterAll, afterEach } from 'vitest';
import type {
  MainModule,
  SystemsManager,
  UserSystem,
  AssetSystem,
  SpaceSystem
} from 'connected-spaces-platform-bindings';
import {
  createTestSpace,
  enterOnlineSpace,
  generatedTestAccountPassword,
  initCsp,
  makeTestUser,
  registerLogSystemCallback
} from '../testUtils';

const ENDPOINT_ROOT_URI = 'https://ogs.magnopus-dev.cloud';
const TENANT = 'OKO_TESTS';

describe('CSP Asset Integrations', () => {
  let csp: MainModule;
  let systemsManager: SystemsManager;
  let userSystem: UserSystem;
  let assetSystem: AssetSystem;
  let spaceSystem: SpaceSystem;

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

    let spaceSystemOrNull = systemsManager.getSpaceSystem();
    if (spaceSystemOrNull === null) {
      throw new Error('Could not get SpaceSystem');
    } else {
      spaceSystem = spaceSystemOrNull;
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

  it('Asset Collection Equality', async () => {
    using assetCollection1 = csp.AssetCollection.create();
    using assetCollection2 = csp.AssetCollection.create();

    expect(assetCollection1.equals(assetCollection2)).toBeTruthy();

    assetCollection1.id = 'AssetCollection1';
    assetCollection2.id = 'AssetCollection2';

    expect(assetCollection1.equals(assetCollection2)).toBeFalsy();

    assetCollection1.id = 'AssetCollection';
    assetCollection2.id = 'AssetCollection';

    expect(assetCollection1.equals(assetCollection2)).toBeTruthy();

    assetCollection1.type = csp.EAssetCollectionType.COMMENT;
    assetCollection2.type = csp.EAssetCollectionType.SPACE_THUMBNAIL;

    expect(assetCollection1.equals(assetCollection2)).toBeFalsy();

    assetCollection2.type = csp.EAssetCollectionType.COMMENT;

    expect(assetCollection1.equals(assetCollection2)).toBeTruthy();
  });

  it('Asset Collection In Space', async () => {
    using profile = await makeTestUser(userSystem);
    using loginResult = await userSystem.login(profile.email, generatedTestAccountPassword, true, true);
    expect(loginResult.resultCode).toBe(csp.EResultCode.Success);

    using newSpace = await createTestSpace(csp, spaceSystem);

    let metadata = new Map<string, string>();
    metadata.set('testKey', 'testValue');

    let assetCollectionName = 'assetCollectionName-{}'.replace('{}', crypto.randomUUID());

    // Make an asset collection
    using makeAssetCollectionResult = await assetSystem.createAssetCollection(
      newSpace.id,
      undefined,
      assetCollectionName,
      metadata,
      csp.EAssetCollectionType.DEFAULT,
      []
    );
    expect(makeAssetCollectionResult.resultCode).toBe(csp.EResultCode.Success);

    // Get the asset collection
    using getAssetCollectionResult = await assetSystem.getAssetCollectionByName(assetCollectionName);
    expect(getAssetCollectionResult.resultCode).toBe(csp.EResultCode.Success);
    expect(getAssetCollectionResult.getAssetCollection().name).toEqual(assetCollectionName);
    expect(getAssetCollectionResult.getAssetCollection().metadata).toEqual(metadata);

    // Delete the asset collection
    using deleteAssetcollectionResult = await assetSystem.deleteAssetCollection(
      getAssetCollectionResult.getAssetCollection()
    );
    expect(deleteAssetcollectionResult.resultCode).toBe(csp.EResultCode.Success);

    // Cleanup
    using exitResult = await spaceSystem.exitSpace();
    expect(exitResult.resultCode).toBe(csp.EResultCode.Success);
    using deleteResult = await spaceSystem.deleteSpace(newSpace.id);
    expect(deleteResult.resultCode).toBe(csp.EResultCode.Success);
  });

  it('Upload Asset', async () => {
    using profile = await makeTestUser(userSystem);
    using loginResult = await userSystem.login(profile.email, generatedTestAccountPassword, true, true);
    expect(loginResult.resultCode).toBe(csp.EResultCode.Success);

    using newSpace = await createTestSpace(csp, spaceSystem);

    let metadata = new Map<string, string>();
    metadata.set('testKey', 'testValue');

    let assetCollectionName = 'assetCollectionName-{}'.replace('{}', crypto.randomUUID());

    // Make an asset collection
    using makeAssetCollectionResult = await assetSystem.createAssetCollection(
      newSpace.id,
      undefined,
      assetCollectionName,
      metadata,
      csp.EAssetCollectionType.DEFAULT,
      []
    );
    expect(makeAssetCollectionResult.resultCode).toBe(csp.EResultCode.Success);

    let assetName = 'assetName';

    // Create asset
    using createAssetResult = await assetSystem.createAsset(
      makeAssetCollectionResult.getAssetCollection(),
      assetName,
      undefined,
      undefined,
      csp.EAssetType.TEXT
    );
    expect(createAssetResult.resultCode).toBe(csp.EResultCode.Success);

    // Upload text to asset
    using asset = createAssetResult.getAsset();
    asset.fileName = 'asset.txt';

    using buffer = csp.NativeBuffer.create(4);
    buffer.getView().set(new TextEncoder().encode('TEST'));

    using bufferAssetDataSource = csp.BufferAssetDataSource.create();
    bufferAssetDataSource.setBuffer(buffer);
    bufferAssetDataSource.mimeType = 'text/plain';

    using uploadDataResult = await assetSystem.uploadAssetData(
      makeAssetCollectionResult.getAssetCollection(),
      asset,
      bufferAssetDataSource
    );
    expect(uploadDataResult.resultCode).toBe(csp.EResultCode.Success);

    // Download the uploaded text
    let response = await fetch(uploadDataResult.uri);
    expect(response.ok).toBeTruthy();

    let fetchedText = await response.text();
    console.log(fetchedText);
    expect(fetchedText).toEqual('TEST');

    // Delete the asset collection
    using deleteAssetcollectionResult = await assetSystem.deleteAssetCollection(
      makeAssetCollectionResult.getAssetCollection()
    );
    expect(deleteAssetcollectionResult.resultCode).toBe(csp.EResultCode.Success);

    // Cleanup
    using exitResult = await spaceSystem.exitSpace();
    expect(exitResult.resultCode).toBe(csp.EResultCode.Success);
    using deleteResult = await spaceSystem.deleteSpace(newSpace.id);
    expect(deleteResult.resultCode).toBe(csp.EResultCode.Success);
  });
});
