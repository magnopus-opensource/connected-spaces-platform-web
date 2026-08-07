cmake_minimum_required(VERSION 3.28)

project(ConnectedSpacesPlatformWeb VERSION 1.0
    DESCRIPTION "Embind generated WASM bindings to the connected spaces platform"
    LANGUAGES CXX)

add_executable(connected-spaces-platform-bindings
    Bindings.cpp
    src/bindings/testtypes/BindingsTestType.cpp
    src/bindings/testtypes/TestContainerBindings.cpp
    src/bindings/testtypes/TestCallbackBindings.cpp
    src/bindings/containers/Equality.cpp
    src/bindings/containers/Disposal.cpp
    src/bindings/containers/Clone.cpp
    src/bindings/async/CatchingCallback.cpp
    src/bindings/usd/UsdDemo.cpp
)

# Virtual filesystem location of USD's plugin resources. CSP's exported target is
# what actually embeds them there (via --embed-file), and USD's own embed rules
# fix the location as /usd, so this only has to name the same path.
target_compile_definitions(connected-spaces-platform-bindings
  PRIVATE
    CSP_USD_PLUGIN_PATH="/usd"
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

# OpenUSD headers, for the USD bindings in src/bindings/usd.
#
# CSP already brings USD's archives to the link, but it exports them as
# $<LINK_ONLY:...> - they are link-only precisely because USD does not appear in
# CSP's public headers, and LINK_ONLY suppresses usage requirements such as
# include directories. Naming the libraries this translation unit actually uses
# brings their include paths and PXR_STATIC in without disturbing the link.
target_link_libraries(connected-spaces-platform-bindings
  PRIVATE
    plug
    ar
    sdf
    usd
    usdGeom
)

target_compile_options(connected-spaces-platform-bindings PRIVATE
  -Wall
  -Wextra
  -Werror
  $<$<PLATFORM_ID:Emscripten>:-pthread>
  $<$<PLATFORM_ID:Emscripten>:-fwasm-exceptions>
)

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
    -sMAXIMUM_MEMORY=1073741824
    # Emscripten's default stack is 64KB and, per its own docs, overflowing it
    # "will fail silently" unless assertions are on. OpenUSD recurses deeply
    # while composing stages and parsing paths, so it needs considerably more
    # than that. Costs address space rather than committed memory.
    -sSTACK_SIZE=4194304
    "-sEXPORTED_RUNTIME_METHODS=['ccall','setValue','getValue','addFunction','removeFunction','UTF8ToString','stringToUTF8', 'stringToNewUTF8', 'lengthBytesUTF8','HEAPU8']"
    "-sINCOMING_MODULE_JS_API=['buffer','fetchSettings','instantiateWasm','locateFile','mainScriptUrlOrBlob','wasmMemory', 'noExitRuntime']"
    --emit-tsd=connected-spaces-platform-bindings.d.ts
    --no-entry
    --extern-pre-js=${CMAKE_SOURCE_DIR}/src/polyfills/polyfills.js
  >
)

include(cmake/PackageInstall.cmake)
