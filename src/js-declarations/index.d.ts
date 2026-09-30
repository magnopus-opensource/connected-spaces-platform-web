/**
 * Manifest of declarations for the parts of the public API that come from JavaScript rather than
 * from C++.
 *
 * Symbols attached to the module via `addToLibrary` or `__postset` in src/js are invisible to
 * Embind, so will not appear in the generated TypeScript declarations file. This file provides the
 * necessary type declarations which will be merged into the generated declarations file.
 *
 * To add declarations for a new JS file:
 * - Create a new .d.ts in this folder with the same name as the JS source file
 * - Export an interface with the members that file attaches to the module
 * - Import the interface here and add it to `JsDeclarations` below
 *
 * Every member must not already exist in the Embind-generated declarations, or the build will fail.
 */

// ADD NEW INTERFACES HERE

import type { CspRequestErrorDeclarations } from './js-library.d.ts';

// Intersection of all declaration interfaces. Add new interfaces to the intersection below.
export type JsDeclarations = CspRequestErrorDeclarations;
