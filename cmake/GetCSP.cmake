include_guard(GLOBAL)

# Locate CSP for the requested configuration.
# If a CSP install is on CMAKE_PREFIX_PATH (the CMake variable or the environment variable), that is used.
# Otherwise the pinned CSP release is downloaded.

# Just pleasant for when you add the prefix path, clear the cache so it's actually picked up without having to delete everything.
# ... although probably delete everything anyway just to be safe.
unset(CSP_DIR CACHE)

# Only consult CMAKE_PREFIX_PATH, not the system PATH, package registries or system prefixes,
# so a CSP is only ever picked up because it was asked for.
# QUIET because we expect this to "fail", and "fallback" to the download.
find_package(CSP CONFIG QUIET
    NO_SYSTEM_ENVIRONMENT_PATH
    NO_CMAKE_PACKAGE_REGISTRY
    NO_CMAKE_SYSTEM_PATH
    NO_CMAKE_SYSTEM_PACKAGE_REGISTRY)

if(CSP_FOUND)
    message(STATUS "Using CSP from discovered path: ${CSP_DIR}")
else()
    # Specify the CSP version we want to use.
    set(CSP_TARGET_VERSION "7.0.0" CACHE STRING "The CSP release version to download")

    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(_CSP_BUILD_TYPE "debug")
    elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
        set(_CSP_BUILD_TYPE "relwithdebinfo")
    elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
        set(_CSP_BUILD_TYPE "release")
    elseif(CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
        set(_CSP_BUILD_TYPE "release")
    else()
        message(FATAL_ERROR "Unsupported build type '${CMAKE_BUILD_TYPE}'")
    endif()


    include(FetchContent)
    FetchContent_Declare(csp_package
        URL "https://github.com/magnopus-opensource/connected-spaces-platform/releases/download/${CSP_TARGET_VERSION}/wasm-${_CSP_BUILD_TYPE}.zip"
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE)

    message(STATUS "Using CSP release ${CSP_TARGET_VERSION}: wasm-${_CSP_BUILD_TYPE}.zip")

    # The archive is an install tree with no top-level CMakeLists.txt, so this only downloads and extracts.
    FetchContent_MakeAvailable(csp_package)

    # Only look in the fetched package. NO_CMAKE_FIND_ROOT_PATH stops the Emscripten toolchain re-rooting it under the sysroot.
    find_package(CSP CONFIG REQUIRED PATHS "${csp_package_SOURCE_DIR}" NO_DEFAULT_PATH NO_CMAKE_FIND_ROOT_PATH)
endif()
