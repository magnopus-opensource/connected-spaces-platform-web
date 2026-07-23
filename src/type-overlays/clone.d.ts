/**
 * Type overlays for Clone.cpp
 */

// Note: there is currently an error for the import, as this is the path for the installed version
// of the generated bindings JS file.
import type { ClassHandle } from "../connected-spaces-platform-bindings.js";

export interface CloneOverrides {
  cloneElement<T extends ClassHandle>(handle: T): T;
  cloneArray<T>(arr: T[]): T[];
  cloneMap<K, V>(map: Map<K, V>): Map<K, V>;
}
