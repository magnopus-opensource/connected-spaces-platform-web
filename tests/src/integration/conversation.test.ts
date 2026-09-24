import { describe, it, expect, beforeAll } from 'vitest';
import type {
  ConversationEventType,
  ConversationSpaceComponent,
  LogSystem,
  MainModule,
  MultiplayerConnection,
  NetworkEventBus,
  ScriptSystem,
  SpaceSystem,
  UserSystem
} from 'connected-spaces-platform-bindings';
import {
  createTestSpace,
  enterOnlineSpace,
  initCsp,
  loginTestUser,
  makeTestUser,
  registerLogSystemCallback,
  until
} from '../testUtils';

// Smallest possible PNG image (1x1 pixel transparent image)
// prettier-ignore
const pngTestData = new Uint8Array([
  0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a, // PNG Signature
  0x00, 0x00, 0x00, 0x0d, 0x49, 0x48, 0x44, 0x52,
  0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01,
  0x01, 0x00, 0x00, 0x00, 0x00, 0x37, 0x6e, 0xf9,
  0x24, 0x00, 0x00, 0x00, 0x0a, 0x49, 0x44, 0x41,
  0x54, 0x78, 0x01, 0x63, 0x60, 0x00, 0x00, 0x00,
  0x02, 0x00, 0x01, 0x73, 0x75, 0x01, 0x18, 0x00,
  0x00, 0x00, 0x00, 0x49, 0x45, 0x4e, 0x44, 0xae,
  0x42, 0x60, 0x82
]);

interface ConversationUpdateTestEvent {
  messageType: ConversationEventType;
  message: string;
}

describe('CSP Conversation Integration Tests', () => {
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
    using loginState = userSystem.getLoginState();

    if (loginState.loginStateValue === csp.ELoginState.LoggedIn) {
      using logoutResult = await userSystem.logout();

      expect(logoutResult.resultCode).toBe(csp.EResultCode.Success);
    }
  });

  afterAll(async () => {
    await multiplayerConnection.setAllowSelfMessagingFlag(false);

    expect(csp.CSPFoundation.shutdown()).toBe(true);
  });

  //================================================================================================

  it('Create a conversation', async () => {
    // Create a test user and log in
    using userProfile = await makeTestUser(userSystem);

    await loginTestUser(csp, userSystem, multiplayerConnection, userProfile.email);

    // Create a test space
    using space = await createTestSpace(csp, spaceSystem);

    // Enter the test space

    using realtimeEngine = await enterOnlineSpace(
      csp,
      spaceSystem,
      multiplayerConnection,
      logSystem,
      eventBus,
      scriptSystem,
      space
    );

    // ------ Create a conversation component ------

    using testEntity = await realtimeEngine.createEntity('TestEntity', {
      position: { x: 1, y: 2, z: 3 },
      rotation: { x: 1, y: 2, z: 3, w: 4 },
      scale: { x: 3, y: 2, z: 1 }
    });

    expect(testEntity).not.toBeNullable();

    if (!testEntity) {
      throw new Error('Failed to create the test entity');
    }

    const component = testEntity.addComponent(csp.ComponentType.Conversation);
    expect(component).not.toBeNullable();

    if (!component) {
      throw new Error('Failed to add Conversation component to the entity');
    }

    const conversationComponent = component as ConversationSpaceComponent;

    // ------ Create a conversation message ------

    const conversationMessage = 'Hello, testing conversations.';

    using createConversationResult = await conversationComponent.createConversation(conversationMessage);

    expect(createConversationResult.resultCode).toBe(csp.EResultCode.Success);
    // Value is the conversation id so make sure we get a non-empty result
    expect(createConversationResult.value).toBeTruthy();

    // Clean up by exiting and deleting the created space
    using exitResult = await spaceSystem.exitSpace();
    expect(exitResult.resultCode).toBe(csp.EResultCode.Success);

    using spaceDeletionResult = await spaceSystem.deleteSpace(space.id);
    expect(spaceDeletionResult.resultCode).toBe(csp.EResultCode.Success);
  });

  it('Update a conversation', async () => {
    // Create a test user and log in
    using userProfile = await makeTestUser(userSystem);

    await loginTestUser(csp, userSystem, multiplayerConnection, userProfile.email);

    // Create a test space
    using space = await createTestSpace(csp, spaceSystem);

    // Enter the test space

    using realtimeEngine = await enterOnlineSpace(
      csp,
      spaceSystem,
      multiplayerConnection,
      logSystem,
      eventBus,
      scriptSystem,
      space
    );

    // ------ Create a conversation component and a conversation ------

    using testEntity = await realtimeEngine.createEntity('TestEntity', {
      position: { x: 1, y: 2, z: 3 },
      rotation: { x: 1, y: 2, z: 3, w: 4 },
      scale: { x: 3, y: 2, z: 1 }
    });

    expect(testEntity).not.toBeNullable();

    if (!testEntity) {
      throw new Error('Failed to create the test entity');
    }

    const component = testEntity.addComponent(csp.ComponentType.Conversation);
    expect(component).not.toBeNullable();

    if (!component) {
      throw new Error('Failed to add Conversation component to the entity');
    }

    const conversationComponent = component as ConversationSpaceComponent;

    using createConversationResult = await conversationComponent.createConversation('Hello, testing conversations.');
    expect(createConversationResult.resultCode).toBe(csp.EResultCode.Success);

    // ------ Update the conversation message ------

    const updatedConversationMessage = 'Updated conversation message.';
    using messageUpdate = csp.MessageUpdateParams.create(updatedConversationMessage);

    using updateResult = await conversationComponent.updateConversation(messageUpdate);
    expect(updateResult.resultCode).toBe(csp.EResultCode.Success);

    using conversationResult = await conversationComponent.getConversationInfo();
    expect(conversationResult.resultCode).toBe(csp.EResultCode.Success);

    using messageInfo = conversationResult.getConversationInfo();

    expect(messageInfo.message).toBe(updatedConversationMessage);

    // ------ Delete the conversation ------

    using deleteResult = await conversationComponent.deleteConversation();

    expect(deleteResult.resultCode).toBe(csp.EResultCode.Success);

    // Clean up by exiting and deleting the created space
    using exitResult = await spaceSystem.exitSpace();
    expect(exitResult.resultCode).toBe(csp.EResultCode.Success);

    using spaceDeletionResult = await spaceSystem.deleteSpace(space.id);
    expect(spaceDeletionResult.resultCode).toBe(csp.EResultCode.Success);
  });

  it('The update conversation callback is called', async () => {
    // Create a test user and log in
    using userProfile = await makeTestUser(userSystem);

    await loginTestUser(csp, userSystem, multiplayerConnection, userProfile.email);

    // Enable self-messaging to be able receive events
    const setAllowSelfMessagingFlagResult = await multiplayerConnection.setAllowSelfMessagingFlag(true);
    expect(setAllowSelfMessagingFlagResult).toBe(csp.ErrorCode.None);

    // Create a test space
    using space = await createTestSpace(csp, spaceSystem);

    // Enter the test space

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

    // ------ Create a conversation component ------

    using testEntity = await realtimeEngine.createEntity('TestEntity', {
      position: { x: 1, y: 2, z: 3 },
      rotation: { x: 1, y: 2, z: 3, w: 4 },
      scale: { x: 3, y: 2, z: 1 }
    });

    expect(testEntity).not.toBeNullable();

    if (!testEntity) {
      throw new Error('Failed to create the test entity');
    }

    const component = testEntity.addComponent(csp.ComponentType.Conversation);
    expect(component).not.toBeNullable();

    if (!component) {
      throw new Error('Failed to add Conversation component to the entity');
    }

    const conversationComponent = component as ConversationSpaceComponent;

    // ------ Set the conversation update callback and store received events ------

    const conversationUpdateEvents: ConversationUpdateTestEvent[] = [];

    conversationComponent.setConversationUpdateCallback((conversationNetworkEventData) => {
      using messageInfo = conversationNetworkEventData.getMessageInfo();

      conversationUpdateEvents.push({
        messageType: conversationNetworkEventData.messageType,
        message: messageInfo.message
      });
    });

    // ------ Create the conversation ------

    const initialConversationMessage = 'Hello, testing conversations.';

    using createConversationResult = await conversationComponent.createConversation(initialConversationMessage);
    expect(createConversationResult.resultCode).toBe(csp.EResultCode.Success);

    testEntity.queueUpdate();
    realtimeEngine.processPendingEntityOperations();

    await until(
      () => {
        csp.CSPFoundation.tick();

        return conversationUpdateEvents.length > 0;
      },
      { intervalMs: 10 }
    );

    expect(conversationUpdateEvents.length).toBeGreaterThan(0);

    expect(conversationUpdateEvents[0]?.messageType).toBe(csp.ConversationEventType.NewConversation);
    expect(conversationUpdateEvents[0]?.message).toBe(initialConversationMessage);

    // ------ Update the conversation message ------

    // Clear the received event array
    conversationUpdateEvents.length = 0;

    const updatedConversationMessage = 'Updated conversation message.';

    using messageUpdate = csp.MessageUpdateParams.create(updatedConversationMessage);

    using updateResult = await conversationComponent.updateConversation(messageUpdate);
    expect(updateResult.resultCode).toBe(csp.EResultCode.Success);

    testEntity.queueUpdate();
    realtimeEngine.processPendingEntityOperations();

    await until(
      () => {
        csp.CSPFoundation.tick();

        return conversationUpdateEvents.length > 0;
      },
      { intervalMs: 10 }
    );

    expect(conversationUpdateEvents.length).toBeGreaterThan(0);

    expect(conversationUpdateEvents[0]?.messageType).toBe(csp.ConversationEventType.ConversationInformation);
    expect(conversationUpdateEvents[0]?.message).toBe(updatedConversationMessage);

    // Clean up by exiting and deleting the created space
    using exitResult = await spaceSystem.exitSpace();
    expect(exitResult.resultCode).toBe(csp.EResultCode.Success);

    using spaceDeletionResult = await spaceSystem.deleteSpace(space.id);
    expect(spaceDeletionResult.resultCode).toBe(csp.EResultCode.Success);
  });

  it('Add messages (replies) to a conversation', async () => {
    // Create a test user and log in
    using userProfile = await makeTestUser(userSystem);

    await loginTestUser(csp, userSystem, multiplayerConnection, userProfile.email);

    // Create a test space
    using space = await createTestSpace(csp, spaceSystem);

    // Enter the test space

    using realtimeEngine = await enterOnlineSpace(
      csp,
      spaceSystem,
      multiplayerConnection,
      logSystem,
      eventBus,
      scriptSystem,
      space
    );

    // ------ Create a conversation component and a conversation ------

    using testEntity = await realtimeEngine.createEntity('TestEntity', {
      position: { x: 1, y: 2, z: 3 },
      rotation: { x: 1, y: 2, z: 3, w: 4 },
      scale: { x: 3, y: 2, z: 1 }
    });

    expect(testEntity).not.toBeNullable();

    if (!testEntity) {
      throw new Error('Failed to create the test entity');
    }

    const component = testEntity.addComponent(csp.ComponentType.Conversation);
    expect(component).not.toBeNullable();

    if (!component) {
      throw new Error('Failed to add Conversation component to the entity');
    }

    const conversationComponent = component as ConversationSpaceComponent;

    using createConversationResult = await conversationComponent.createConversation('Hello, testing conversations.');
    expect(createConversationResult.resultCode).toBe(csp.EResultCode.Success);

    // ------ Add messages (replies) to the conversation ------

    const message1 = 'Reply 1';
    const message2 = 'Reply 2';

    using addMessageResult1 = await conversationComponent.addMessage(message1);
    expect(addMessageResult1.resultCode).toBe(csp.EResultCode.Success);

    using addMessageResult2 = await conversationComponent.addMessage(message2);
    expect(addMessageResult2.resultCode).toBe(csp.EResultCode.Success);

    // ------ Retrieve messages from the conversation ------

    using numberOfRepliesResult = await conversationComponent.getNumberOfReplies();
    expect(numberOfRepliesResult.resultCode).toBe(csp.EResultCode.Success);
    expect(numberOfRepliesResult.count).toBe(2n);

    using messagesResult = await conversationComponent.getMessagesFromConversation();
    expect(messagesResult.resultCode).toBe(csp.EResultCode.Success);

    using messages = messagesResult.getMessages();
    expect(messages.length).toBe(2);
    // Messages are in reverse order (most recent first)
    expect(messages[0]?.message).toBe(message2);
    expect(messages[1]?.message).toBe(message1);

    using message1Info = addMessageResult1.getMessageInfo();
    expect(message1Info.messageId).toBeTruthy();
    // Verify that the message belongs to the correct conversation
    expect(message1Info.conversationId).toBe(createConversationResult.value);
    expect(message1Info.message).toBe(message1);

    // ------ Delete a message ------

    using deleteMessageResult = await conversationComponent.deleteMessage(message1Info.messageId);
    expect(deleteMessageResult.resultCode).toBe(csp.EResultCode.Success);

    using numberOfRepliesAfterDeleteResult = await conversationComponent.getNumberOfReplies();
    expect(numberOfRepliesAfterDeleteResult.resultCode).toBe(csp.EResultCode.Success);
    expect(numberOfRepliesAfterDeleteResult.count).toBe(1n);

    // Clean up by exiting and deleting the created space
    using exitResult = await spaceSystem.exitSpace();
    expect(exitResult.resultCode).toBe(csp.EResultCode.Success);

    using spaceDeletionResult = await spaceSystem.deleteSpace(space.id);
    expect(spaceDeletionResult.resultCode).toBe(csp.EResultCode.Success);
  });

  it('Add an annotation to a conversation', async () => {
    // Create a test user and log in
    using userProfile = await makeTestUser(userSystem);

    await loginTestUser(csp, userSystem, multiplayerConnection, userProfile.email);

    // Create a test space
    using space = await createTestSpace(csp, spaceSystem);

    // Enter the test space

    using realtimeEngine = await enterOnlineSpace(
      csp,
      spaceSystem,
      multiplayerConnection,
      logSystem,
      eventBus,
      scriptSystem,
      space
    );

    // ------ Create a conversation component and a conversation ------

    using testEntity = await realtimeEngine.createEntity('TestEntity', {
      position: { x: 1, y: 2, z: 3 },
      rotation: { x: 1, y: 2, z: 3, w: 4 },
      scale: { x: 3, y: 2, z: 1 }
    });

    expect(testEntity).not.toBeNullable();

    if (!testEntity) {
      throw new Error('Failed to create the test entity');
    }

    const component = testEntity.addComponent(csp.ComponentType.Conversation);
    expect(component).not.toBeNullable();

    if (!component) {
      throw new Error('Failed to add Conversation component to the entity');
    }

    const conversationComponent = component as ConversationSpaceComponent;

    using createConversationResult = await conversationComponent.createConversation('Hello, testing conversations.');
    expect(createConversationResult.resultCode).toBe(csp.EResultCode.Success);

    // ------ Create annotation and thumbnail data ------

    using annotationBuffer = csp.NativeBuffer.create(pngTestData.length);
    annotationBuffer.getView().set(pngTestData);

    using annotationBufferAssetDataSource = csp.BufferAssetDataSource.create();
    annotationBufferAssetDataSource.setBuffer(annotationBuffer);
    annotationBufferAssetDataSource.mimeType = 'image/png';

    using thumbnailBuffer = csp.NativeBuffer.create(pngTestData.length);
    thumbnailBuffer.getView().set(pngTestData);

    using thumbnailBufferAssetDataSource = csp.BufferAssetDataSource.create();
    thumbnailBufferAssetDataSource.setBuffer(thumbnailBuffer);
    thumbnailBufferAssetDataSource.mimeType = 'image/png';

    // ------ Add the annotation on the conversation ------

    using addAnnotationResult = await conversationComponent.setConversationAnnotation(
      {
        verticalFov: 50,
        authorCameraPosition: { x: 1, y: 2, z: 3 },
        authorCameraRotation: { x: 1, y: 2, z: 3, w: 4 }
      },
      annotationBufferAssetDataSource,
      thumbnailBufferAssetDataSource
    );

    expect(addAnnotationResult.resultCode).toBe(csp.EResultCode.Success);

    using annotationAsset = addAnnotationResult.getAnnotationAsset();
    expect(annotationAsset.id).toBeTruthy();

    using annotationThumbnailAsset = addAnnotationResult.getAnnotationThumbnailAsset();
    expect(annotationThumbnailAsset.id).toBeTruthy();

    // ------ Retrieve the annotation ------

    using getAnnotationResult = await conversationComponent.getConversationAnnotation();
    expect(getAnnotationResult.resultCode).toBe(csp.EResultCode.Success);

    using annotationData = getAnnotationResult.getAnnotationData();

    expect(annotationData.verticalFov).toBe(50);
    expect(annotationData.authorCameraPosition).toEqual({ x: 1, y: 2, z: 3 });
    expect(annotationData.authorCameraRotation).toEqual({ x: 1, y: 2, z: 3, w: 4 });

    // ------ Delete the annotation ------

    using deleteAnnotationResult = await conversationComponent.deleteConversationAnnotation();
    expect(deleteAnnotationResult.resultCode).toBe(csp.EResultCode.Success);

    // Clean up by exiting and deleting the created space
    using exitResult = await spaceSystem.exitSpace();
    expect(exitResult.resultCode).toBe(csp.EResultCode.Success);

    using spaceDeletionResult = await spaceSystem.deleteSpace(space.id);
    expect(spaceDeletionResult.resultCode).toBe(csp.EResultCode.Success);
  });
});
