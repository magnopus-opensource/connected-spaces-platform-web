# Produce an npm-package-shaped install tree so downstream consumers (and our
# own tests) can import the bindings by package name rather than build path.

# npm script values and config commonly contain ${...} so @ONLY prevents us substituting that form.
# we are doing substitution here at time of writing, to expand project version and description, although
# even so the @ONLY is merely defensive
configure_file(package.json.in package.json @ONLY)

# Note that this isn't a package install. As far as cmake is concerned, the target only produces a .js, we'd still have to install additional files.
# Might as well just do it all as a FILES install.
# Could theoretically use BYPRODUCT to tie them together in the target, but I don't think that buys us much here, would still have to do this. Might be a better way I'm no expert.
install(FILES
      "$<TARGET_FILE:connected-spaces-platform-bindings>" # This is the .js file that is actually our target output
      "$<TARGET_FILE_DIR:connected-spaces-platform-bindings>/$<TARGET_FILE_BASE_NAME:connected-spaces-platform-bindings>.wasm" # The wasm, not a target output, always adjacent
      "$<TARGET_FILE_DIR:connected-spaces-platform-bindings>/$<TARGET_FILE_BASE_NAME:connected-spaces-platform-bindings>.d.ts" # Embind-generated TS declarations via --emit-tsd, always adjacent
      "${CMAKE_CURRENT_BINARY_DIR}/package.json" # Configured above, makes this an npm package
      DESTINATION .
  )
