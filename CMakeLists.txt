cmake_minimum_required(VERSION 3.15...4.2)

project(TestBinding VERSION 1.0
    DESCRIPTION "Test library for build system"
    LANGUAGES CXX)

add_executable(test-emscripten-bindings Bindings.cpp)

# Project uses C++17
target_compile_features(test-emscripten-bindings PUBLIC cxx_std_17)

include(FetchContent)

FetchContent_Declare(
  test-lib
  GIT_REPOSITORY https://github.com/MAG-mv/test-build-system.git
  GIT_TAG        143c5d68342fdcc704e9c5fb46b498efbf46178a
  GIT_SHALLOW    TRUE
  GIT_PROGRESS   TRUE
)

FetchContent_MakeAvailable(test-lib)

target_link_libraries(test-emscripten-bindings
  test-lib
)

target_link_options(test-emscripten-bindings PRIVATE
  "-lembind"
  "-sEXPORT_NAME=TestModule"
  "-sALLOW_MEMORY_GROWTH=1"
  "-sEXIT_RUNTIME=1"
  "--no-entry"
  "--emrun"
  "-sASSERTIONS=0"
  "-sEXPORTED_FUNCTIONS=['_exit']"                          
)


# TODO add module target
# TODO: install into A package that can be consumed directly

include(CTest)
enable_testing()

set(TEST_WEB_DIR "${CMAKE_BINARY_DIR}/webtests")

# Ensure output name matches what runner.html loads
set_target_properties(test-emscripten-bindings PROPERTIES
  OUTPUT_NAME "test_module"
  RUNTIME_OUTPUT_DIRECTORY "${TEST_WEB_DIR}"
)

add_custom_command(TARGET test-emscripten-bindings POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
          "${CMAKE_SOURCE_DIR}/IntegrationTest/runner.js"
          "${TEST_WEB_DIR}/runner.js"
)

set(MODULE_JS "test_module.js")
configure_file(
  "${CMAKE_SOURCE_DIR}/IntegrationTest/runner.html.in"
  "${TEST_WEB_DIR}/runner.html"
  @ONLY
)

# --log-level=3 prevents browser console spam.

add_test(
  NAME wasm_browser_integration
  COMMAND emrun.bat
          --timeout 60
          --port 0
          --browser chrome
          --browser_args "--headless --disable-gpu --no-sandbox --log-level=3"
          "${TEST_WEB_DIR}/runner.html"
)

set_tests_properties(wasm_browser_integration PROPERTIES
  DEPENDS test-emscripten-bindings
  WORKING_DIRECTORY "${TEST_WEB_DIR}"
)