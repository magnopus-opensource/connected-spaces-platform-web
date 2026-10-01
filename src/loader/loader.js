// Package entry point that picks the release or debug build of the bindings at runtime.
// Both builds expose the same API, so the choice only affects which build is downloaded.
// Each build is a separate dynamic import, so bundlers emit them as separate chunks and only the
// selected build's .js and .wasm are fetched.
export async function loadCsp({ debug = false, moduleOptions } = {}) {
  const { default: createModule } = debug
    ? await import('./debug/connected-spaces-platform-bindings.js')
    : await import('./connected-spaces-platform-bindings.js');

  return createModule(moduleOptions);
}
