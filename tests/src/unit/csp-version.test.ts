import { describe, it, expect } from 'vitest';
import { cspVersion } from 'connected-spaces-platform-bindings/version';
import packageJson from 'connected-spaces-platform-bindings/package.json';

describe('CSP version', () => {
  it('is a semantic version', () => {
    expect(cspVersion).toMatch(/^\d+\.\d+\.\d+/);
  });

  it('matches the version recorded in package.json', () => {
    expect(packageJson.cspVersion).toBe(cspVersion);
  });
});
