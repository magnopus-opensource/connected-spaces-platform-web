// The release and debug builds are generated from the same bindings, so the release build's
// declarations describe both.
import type { MainModule } from './connected-spaces-platform-bindings.js';

export interface LoadCspOptions {
  /** Load the debug build (with DWARF debug info) instead of the release build. Defaults to false. */
  debug?: boolean | undefined;
  /** Passed through to the Emscripten module factory, e.g. `locateFile`. */
  moduleOptions?: unknown;
}

/** Loads and instantiates the release or debug build of the bindings. */
export function loadCsp(options?: LoadCspOptions): Promise<MainModule>;
