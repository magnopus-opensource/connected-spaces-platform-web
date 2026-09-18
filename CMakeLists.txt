cmake_minimum_required(VERSION 3.28)

project(ConnectedSpacesPlatformWeb VERSION 1.0
    DESCRIPTION "Embind generated WASM bindings to the connected spaces platform"
    LANGUAGES CXX)

add_executable(connected-spaces-platform-bindings
    src/bindings/csp/AssetHash.cpp
    src/bindings/csp/CSPFoundation.cpp
    src/bindings/csp/ContainerDeclarations.cpp
    src/bindings/csp/CallbackDeclarations.cpp
    src/bindings/csp/PromiseDeclarations.cpp
    src/bindings/csp/PointerDeclarations.cpp
    src/bindings/csp/Common/SharedEnums.cpp
    src/bindings/csp/Systems/WebService.cpp
    src/bindings/csp/Common/Vector.cpp
    src/bindings/csp/Common/ReplicatedValue.cpp
    src/bindings/csp/Common/Settings.cpp
    src/bindings/csp/Common/LoginState.cpp
    src/bindings/csp/Common/CancellationToken.cpp
    src/bindings/csp/Common/NetworkEventData.cpp
    src/bindings/csp/Common/Interfaces/IJSScriptRunner.cpp
    src/bindings/csp/Common/Interfaces/IRealtimeEngine.cpp
    src/bindings/csp/Common/Systems/Log/LogLevels.cpp
    src/bindings/csp/Common/Systems/Log/LogSystem.cpp
    src/bindings/csp/Multiplayer/PatchTypes.cpp
    src/bindings/csp/Multiplayer/ComponentBase.cpp
    src/bindings/csp/Multiplayer/SpaceEntity.cpp
    src/bindings/csp/Multiplayer/SpaceTransform.cpp
    src/bindings/csp/Multiplayer/CSPSceneDescription.cpp
    src/bindings/csp/Multiplayer/MultiPlayerConnection.cpp
    src/bindings/csp/Multiplayer/NetworkEventBus.cpp
    src/bindings/csp/Multiplayer/OnlineRealtimeEngine.cpp
    src/bindings/csp/Multiplayer/OfflineRealtimeEngine.cpp
    src/bindings/csp/Multiplayer/Script/EntityScript.cpp
    src/bindings/csp/Multiplayer/Conversation/Conversation.cpp
    src/bindings/csp/Multiplayer/Components/BillBoardModeEnum.cpp
    src/bindings/csp/Multiplayer/Components/AIChatbotComponent.cpp
    src/bindings/csp/Multiplayer/Components/AnimatedModelSpaceComponent.cpp
    src/bindings/csp/Multiplayer/Components/AudioSpaceComponent.cpp
    src/bindings/csp/Multiplayer/Components/AvatarSpaceComponent.cpp
    src/bindings/csp/Multiplayer/Components/ButtonSpaceComponent.cpp
    src/bindings/csp/Multiplayer/Components/CinematicCameraSpaceComponent.cpp
    src/bindings/csp/Multiplayer/Components/CollisionSpaceComponent.cpp
    src/bindings/csp/Multiplayer/Components/ConversationSpaceComponent.cpp
    src/bindings/csp/Multiplayer/Components/CustomSpaceComponent.cpp
    src/bindings/csp/Multiplayer/Components/ECommerceSpaceComponent.cpp
    src/bindings/csp/Multiplayer/Components/ExternalLinkSpaceComponent.cpp
    src/bindings/csp/Multiplayer/Components/FiducialMarkerSpaceComponent.cpp
    src/bindings/csp/Multiplayer/Components/FogSpaceComponent.cpp
    src/bindings/csp/Multiplayer/Components/GaussianSplatSpaceComponent.cpp
    src/bindings/csp/Multiplayer/Components/HotspotSpaceComponent.cpp
    src/bindings/csp/Multiplayer/Components/ImageSpaceComponent.cpp
    src/bindings/csp/Multiplayer/Components/LightSpaceComponent.cpp
    src/bindings/csp/Multiplayer/Components/PortalSpaceComponent.cpp
    src/bindings/csp/Multiplayer/Components/ReflectionSpaceComponent.cpp
    src/bindings/csp/Multiplayer/Components/ScreenSharingSpaceComponent.cpp
    src/bindings/csp/Multiplayer/Components/ScriptSpaceComponent.cpp
    src/bindings/csp/Multiplayer/Components/SplineSpaceComponent.cpp
    src/bindings/csp/Multiplayer/Components/StaticModelSpaceComponent.cpp
    src/bindings/csp/Multiplayer/Components/TextSpaceComponent.cpp
    src/bindings/csp/Multiplayer/Components/VideoPlayerSpaceComponent.cpp
    src/bindings/csp/Systems/SystemBase.cpp
    src/bindings/csp/Systems/SystemsManager.cpp
    src/bindings/csp/Systems/SystemsResult.cpp
    src/bindings/csp/Systems/ServiceStatus.cpp
    src/bindings/csp/Systems/CSPSceneData.cpp
    src/bindings/csp/Systems/Assets/Asset.cpp
    src/bindings/csp/Systems/Assets/AssetCollection.cpp
    src/bindings/csp/Systems/Assets/TextureInfo.cpp
    src/bindings/csp/Systems/Assets/Material.cpp
    src/bindings/csp/Systems/Assets/GLTFMaterial.cpp
    src/bindings/csp/Systems/Assets/AlphaVideoMaterial.cpp
    src/bindings/csp/Systems/Assets/LOD.cpp
    src/bindings/csp/Systems/Assets/AssetSystem.cpp
    src/bindings/csp/Systems/Analytics/AnalyticsSystem.cpp
    src/bindings/csp/Systems/ECommerce/ECommerce.cpp
    src/bindings/csp/Systems/ECommerce/ECommerceSystem.cpp
    src/bindings/csp/Systems/EventTicketing/EventTicketing.cpp
    src/bindings/csp/Systems/EventTicketing/EventTicketingSystem.cpp
    src/bindings/csp/Systems/ExternalServices/ExternalServiceProxySystem.cpp
    src/bindings/csp/Systems/GraphQL/GraphQL.cpp
    src/bindings/csp/Systems/GraphQL/GraphQLSystem.cpp
    src/bindings/csp/Systems/HotspotSequence/HotspotGroup.cpp
    src/bindings/csp/Systems/HotspotSequence/HotspotSequenceSystem.cpp
    src/bindings/csp/Systems/Maintenance/Maintenance.cpp
    src/bindings/csp/Systems/Maintenance/MaintenanceSystem.cpp
    src/bindings/csp/Systems/Multiplayer/Scope.cpp
    src/bindings/csp/Systems/Multiplayer/ScopeLeader.cpp
    src/bindings/csp/Systems/Quota/Quota.cpp
    src/bindings/csp/Systems/Quota/QuotaSystem.cpp
    src/bindings/csp/Systems/Script/ScriptSystem.cpp
    src/bindings/csp/Systems/Sequence/Sequence.cpp
    src/bindings/csp/Systems/Sequence/SequenceSystem.cpp
    src/bindings/csp/Systems/Settings/ApplicationSettings.cpp
    src/bindings/csp/Systems/Settings/ApplicationSettingsSystem.cpp
    src/bindings/csp/Systems/Settings/SettingsCollection.cpp
    src/bindings/csp/Systems/Settings/SettingsSystem.cpp
    src/bindings/csp/Systems/Spaces/Space.cpp
    src/bindings/csp/Systems/Spaces/SpaceSystem.cpp
    src/bindings/csp/Systems/Spaces/Site.cpp
    src/bindings/csp/Systems/Spaces/UserRoles.cpp
    src/bindings/csp/Systems/Spatial/SpatialDataTypes.cpp
    src/bindings/csp/Systems/Spatial/Anchor.cpp
    src/bindings/csp/Systems/Spatial/AnchorSystem.cpp
    src/bindings/csp/Systems/Spatial/PointOfInterest.cpp
    src/bindings/csp/Systems/Spatial/PointOfInterestSystem.cpp
    src/bindings/csp/Systems/Users/Profile.cpp
    src/bindings/csp/Systems/Users/Authentication.cpp
    src/bindings/csp/Systems/Users/ThirdPartyAuthentication.cpp
    src/bindings/csp/Systems/Users/UserSystem.cpp
    src/bindings/csp/Systems/Voip/VoipSystem.cpp
    src/bindings/testtypes/BindingsTestType.cpp
    src/bindings/testtypes/TestContainerBindings.cpp
    src/bindings/testtypes/TestCallbackBindings.cpp
    src/bindings/testtypes/TestResultBindings.cpp
    src/bindings/containers/Equality.cpp
    src/bindings/containers/Disposal.cpp
    src/bindings/containers/Clone.cpp
    src/bindings/containers/NativeBuffer.cpp
    src/bindings/async/CatchingCallback.cpp
    src/bindings/async/Promises.cpp
)
set_target_properties(connected-spaces-platform-bindings PROPERTIES SUFFIX ".js")

# Project uses C++17
target_compile_features(connected-spaces-platform-bindings PUBLIC cxx_std_17)

# Configure with CMAKE_PREFIX_PATH to be an install directory
find_package(CSP CONFIG REQUIRED)

target_link_libraries(connected-spaces-platform-bindings
  PRIVATE
    CSP::csp-lib
)

target_compile_options(connected-spaces-platform-bindings PRIVATE
  -Wall
  -Wextra
  -Werror
  $<$<PLATFORM_ID:Emscripten>:-pthread>
  $<$<PLATFORM_ID:Emscripten>:-fwasm-exceptions>
  $<$<CONFIG:Debug>:-fstandalone-debug> # By default, clang limits debug info to keep binary size down, normally fine, but WASM is a special case. Turn that off.
)

# We configure this to set a different maximum memory for tests, since we start a lot of runtimes and don't want to reserve so much memory.
set(CSP_WASM_MAXIMUM_MEMORY "1073741824" CACHE STRING "Maximum wasm heap size, in bytes")

target_link_options(connected-spaces-platform-bindings PRIVATE
   $<$<PLATFORM_ID:Emscripten>:
    # Emit DWARF in the final .wasm for Debug builds only.
    $<$<CONFIG:Debug>:-g>
    -pthread
    -fwasm-exceptions
    -lembind
    -lwebsocket.js
    -sFETCH
    -sPTHREAD_POOL_SIZE_STRICT=0
    "-sEXPORTED_FUNCTIONS=['_malloc','_free','_exit']"
    -sEXPORT_ES6=1
    -sMODULARIZE=1
    -sEXPORT_NAME=createModule
    -sALLOW_TABLE_GROWTH=1
    -sWASM_BIGINT
    -sALLOW_MEMORY_GROWTH=1
    -sINITIAL_MEMORY=33554432
    -sMAXIMUM_MEMORY=${CSP_WASM_MAXIMUM_MEMORY}
    "-sEXPORTED_RUNTIME_METHODS=['ccall','setValue','getValue','addFunction','removeFunction','UTF8ToString','stringToUTF8', 'stringToNewUTF8', 'lengthBytesUTF8','HEAPU8']"
    "-sINCOMING_MODULE_JS_API=['buffer','fetchSettings','instantiateWasm','locateFile','mainScriptUrlOrBlob','wasmMemory', 'noExitRuntime']"
    --emit-tsd=connected-spaces-platform-bindings.d.ts
    --no-entry
    --js-library=${CMAKE_SOURCE_DIR}/src/js/js-library.js
    --extern-pre-js=${CMAKE_SOURCE_DIR}/src/polyfills/polyfills.js
  >
)

include(cmake/PackageInstall.cmake)
