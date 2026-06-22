/**
 * Type overlays for Equality.cpp
 */

export interface EqualityOverrides {
  arrayEquals<T>(a: T[], b: T[]): boolean;
  elementEquals<T>(a: T, b: T): boolean;
  mapEquals<K, V>(a: Map<K, V>, b: Map<K, V>): boolean;
}
