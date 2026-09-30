/**
 * Type overlays for Clone.cpp
 */

export interface CloneOverrides {
  cloneArray<T>(arr: T[]): T[];
  cloneMap<K, V>(map: Map<K, V>): Map<K, V>;
}
