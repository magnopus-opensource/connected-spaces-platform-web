/**
 * Type overlay for auto-generated emscripten bindings.
 *
 * The `--emit-tsd` flag produces loosely-typed signatures for functions with `emscripten::val`
 * parameters.
 * This file combines custom per-source-file type overlays and re-exports the generated declarations
 * with the improved types included.
 *
 * To add new overlays for a new C++ file:
 * - Create a new .d.ts in this folder with the same name as the C++ source file
 * - Export an interface with the improved signatures,
 * - Import the interface here and add it to `TypeOverrides` below.
 */

// Note: there is currently an error for the import, as this is the path for the installed version
// of the generated bindings JS file.
import type { MainModule as _GeneratedMainModule } from "../connected-spaces-platform-bindings.js";

//==================================================================================================

// ADD NEW INTERFACES HERE

import type { EqualityOverrides } from "./equality.d.ts";

// Union of all type overlay interfaces.
type TypeOverrides = EqualityOverrides;

//==================================================================================================

// MainModule with hand-written type overrides applied.
export type MainModule = Omit<_GeneratedMainModule, keyof TypeOverrides> &
  TypeOverrides;

// Re-export all other generated types.
// The generated MainModule will be skipped as the local MainModule shadows the generated one.
export * from "../connected-spaces-platform-bindings.js";

// Re-export the factory function with the narrowed MainModule return type
declare function MainModuleFactory(options?: unknown): Promise<MainModule>;
export default MainModuleFactory;
