/**
 * Type overlays for Memory.cpp
 */

// Note: there is currently an error for the import, as this is the path for the installed version
// of the generated bindings JS file.
import type { ClassHandle } from "../connected-spaces-platform-bindings.js";

export interface MemoryOverrides {
  disposeElement<T extends ClassHandle>(handle: T): void;
  disposeArray<T>(arr: T[]): void;
  disposeMap<K, V>(map: Map<K, V>): void;
}
