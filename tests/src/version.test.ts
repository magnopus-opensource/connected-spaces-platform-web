import { describe, it, expect, beforeAll } from 'vitest';
import { loadCSP } from '../loadModule';
import type { MainModule } from 'connected-spaces-platform-bindings';


/*
 * This is only a temporary test to demonstrate that an actual CSP method can be bound.
 * It will be deleted when we begin wallpapering the API proper.
 */

describe('CSPFoundation', () => {
  let csp: MainModule;

  beforeAll(async () => {
    csp = await loadCSP();
  });

  it('exports GetVersion', () => {
    expect(typeof csp.GetVersion).toBe('function');
  });

  it('returns a non-empty version string', () => {
    const version = csp.GetVersion();
    expect(typeof version).toBe('string');
    expect(version.length).toBeGreaterThan(0);
  });
});
