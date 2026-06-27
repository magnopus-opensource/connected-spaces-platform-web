cmake_minimum_required(VERSION 3.28)

project(ConnectedSpacesPlatformWeb VERSION 1.0
    DESCRIPTION "Embind generated WASM bindings to the connected spaces platform"
    LANGUAGES CXX)

add_executable(connected-spaces-platform-bindings
    Bindings.cpp
    src/bindings/TestBindings.cpp
    src/bindings/utils/Equality.cpp
    src/bindings/utils/Memory.cpp
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
)

target_link_options(connected-spaces-platform-bindings PRIVATE
   $<$<PLATFORM_ID:Emscripten>:
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
    "-sEXPORTED_RUNTIME_METHODS=['ccall','setValue','getValue','addFunction','removeFunction','UTF8ToString','stringToUTF8','lengthBytesUTF8','HEAPU8']"
    "-sINCOMING_MODULE_JS_API=['buffer','fetchSettings','instantiateWasm','locateFile','mainScriptUrlOrBlob','wasmMemory']"
    --emit-tsd=connected-spaces-platform-bindings.d.ts
    --no-entry
  >
)

include(cmake/PackageInstall.cmake)