# Merges the hand-written TypeScript type overlays and declarations into the output of Embind's
# --emit-tsd, so the package ships a single, unified declaration file.
# - Type overlays are in src/type-overlays.
# - Type declarations are in src/js-declarations.
#
# This module provides:
#
#   merge_dts(TARGET <target> OUTPUT_VARIABLE <var>)
#
# which registers the install step that produces the merged file, and sets <var> to its path for
# the caller to install.

include_guard(GLOBAL)

find_program(NODE_EXECUTABLE NAMES node REQUIRED
  DOC "Node.js, used by merge-dts script")

# Check for Node's built-in TypeScript support (i.e. type stripping) to ensure we can run the merge
# script.
execute_process(
  COMMAND "${NODE_EXECUTABLE}" -p "process.versions.node + ' ' + process.features.typescript"
  OUTPUT_VARIABLE NODE_PROBE
  OUTPUT_STRIP_TRAILING_WHITESPACE)
string(REGEX MATCH "^[^ ]*" NODE_VERSION "${NODE_PROBE}")
if(NOT NODE_PROBE MATCHES " (strip|transform)$")
  message(FATAL_ERROR
    "Node ${NODE_VERSION} found at ${NODE_EXECUTABLE} cannot run TypeScript directly, which "
    "merge-dts.ts needs. Use node >= 22.18 (or >= 23.6 for 23.x), or point NODE_EXECUTABLE "
    "at a newer install.")
endif()

# merge_dts(TARGET <target> OUTPUT_VARIABLE <var>)
#
# <target> is the name of executable linked with the Embind --emit-tsd.
# <var> is set to the path of the merged .d.ts file.
#
# Call merge_dts() before the install(FILES).
function(merge_dts)
  cmake_parse_arguments(PARSE_ARGV 0 ARG "" "TARGET;OUTPUT_VARIABLE" "")
  if(ARG_UNPARSED_ARGUMENTS)
    message(FATAL_ERROR "merge_dts: unexpected arguments: ${ARG_UNPARSED_ARGUMENTS}")
  endif()
  if(NOT ARG_TARGET OR NOT ARG_OUTPUT_VARIABLE)
    message(FATAL_ERROR "merge_dts: TARGET and OUTPUT_VARIABLE are both required.")
  endif()
  if(NOT TARGET ${ARG_TARGET})
    message(FATAL_ERROR "merge_dts: \"${ARG_TARGET}\" is not a target.")
  endif()

  set(generated "$<TARGET_FILE_DIR:${ARG_TARGET}>/$<TARGET_FILE_BASE_NAME:${ARG_TARGET}>.d.ts")

  # Output the merged file into a subdirectory to not overwrite the original generated .d.ts file
  # during the merge.
  set(merged "${CMAKE_CURRENT_BINARY_DIR}/merged/$<TARGET_FILE_BASE_NAME:${ARG_TARGET}>.d.ts")

  install(CODE "
    execute_process(
      COMMAND \"${NODE_EXECUTABLE}\" \"${PROJECT_SOURCE_DIR}/tools/merge-dts/merge-dts.ts\"
        --generated    \"${generated}\"
        --overlays     \"${PROJECT_SOURCE_DIR}/src/type-overlays\"
        --declarations \"${PROJECT_SOURCE_DIR}/src/js-declarations\"
        --out          \"${merged}\"
      RESULT_VARIABLE MERGE_RESULT
    )
    if(NOT MERGE_RESULT EQUAL 0)
      message(FATAL_ERROR \"Failed to merge the hand-written type declarations with the generated ones.\")
    endif()
  ")

  set(${ARG_OUTPUT_VARIABLE} "${merged}" PARENT_SCOPE)
endfunction()
