/**
 * Manifest of the hand-written type overlays for the auto-generated Emscripten bindings.
 *
 * The Embind `--emit-tsd` flag produces loosely-typed signatures for functions with
 * `emscripten::val` parameters which appear as `any` in TypeScript. Each file in this folder
 * replaces those signatures for one C++ source file.
 *
 * The type overlays included here will be merged into the generated declarations file.
 *
 * To add overlays for a new C++ file:
 * - Create a new .d.ts in this folder with the same name as the C++ source file
 * - Export an interface with the improved signatures
 * - Import the interface here and add it to `TypeOverrides` below
 *
 * Every member of those interfaces must match a member Embind generated, or the build fails.
 * For the public API defined in JavaScript rather than C++, see src/js-declarations instead.
 */

// ADD NEW INTERFACES HERE

import type { EqualityOverrides } from './equality.d.ts';
import type { DisposalOverrides } from './disposal.d.ts';
import type { CloneOverrides } from './clone.d.ts';

// Intersection of all type overlay interfaces. Add new interfaces to the intersection below.
export type TypeOverrides = EqualityOverrides & DisposalOverrides & CloneOverrides;
