import { describe, it, expect, beforeAll } from 'vitest';
import { loadCSP } from '../loadModule';
import type { MainModule } from 'connected-spaces-platform-bindings';

describe('CSPFoundation', () => {
  let csp: MainModule;

  beforeAll(async () => {
    csp = await loadCSP();
  });

/* 
 * Test some assumptions about emscripten and automatic memory management
 * Arguably we're testing the library here, but browsers are
 * mysterious things and this proves we can rely on this sort
 * of thing at least on the chromium target.
 * 
 * Quite important i'd say, we'll likely build user patterns around these assumptions.
 */

  it('Delete releases C++ memory', () => {
    const before = csp.BindingsTestType.aliveCount;

    const leaked = new csp.BindingsTestType();
    expect(csp.BindingsTestType.aliveCount).toBe(before + 1);

    leaked.delete();
    expect(csp.BindingsTestType.aliveCount).toBe(before);
  });

 it('Releases C++ memory at scope exit', () => {
    const before = csp.BindingsTestType.aliveCount;

    {
      using t = new csp.BindingsTestType(42, 'hello');
      expect(csp.BindingsTestType.aliveCount).toBe(before + 1);
    }

    expect(csp.BindingsTestType.aliveCount).toBe(before);
  });

  it('Releases all C++ memory at scope exit', () => {
    const before = csp.BindingsTestType.aliveCount;
    {
      using a = new csp.BindingsTestType();
      using b = new csp.BindingsTestType();
      using c = new csp.BindingsTestType();
      expect(csp.BindingsTestType.aliveCount).toBe(before + 3);
    }
    expect(csp.BindingsTestType.aliveCount).toBe(before);
  });

  it('Releases C++ memory when scope exits via exception', () => {
    const before = csp.BindingsTestType.aliveCount;

    expect(() => {
      using t = new csp.BindingsTestType();
      expect(csp.BindingsTestType.aliveCount).toBe(before + 1);
      throw new Error('Oh no!');
    }).toThrow('Oh no!');

    expect(csp.BindingsTestType.aliveCount).toBe(before);
  });

  it('Throws on double delete', () => {
    const t = new csp.BindingsTestType();
    t.delete();
    expect(() => t.delete()).toThrow("BindingsTestType instance already deleted");
  });

  it('Throws when accessing a deleted handle', () => {
    const t = new csp.BindingsTestType(1, 'x');
    t.delete();
    expect(() => t.value).toThrow("cannot call emscripten binding method BindingsTestType.value getter on deleted object");
  });

});

