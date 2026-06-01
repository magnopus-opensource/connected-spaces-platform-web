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

    const leaked = csp.BindingsTestType.create(42, 'hello');
    expect(csp.BindingsTestType.aliveCount).toBe(before + 1);

    leaked.delete();
    expect(csp.BindingsTestType.aliveCount).toBe(before);
  });

 it('Releases C++ memory at scope exit', () => {
    const before = csp.BindingsTestType.aliveCount;

    {
      using t = csp.BindingsTestType.create(42, 'hello');
      expect(csp.BindingsTestType.aliveCount).toBe(before + 1);
    }

    expect(csp.BindingsTestType.aliveCount).toBe(before);
  });

  it('Releases all C++ memory at scope exit', () => {
    const before = csp.BindingsTestType.aliveCount;
    {
      using a = csp.BindingsTestType.create(42, 'hello');
      using b = csp.BindingsTestType.create(42, 'hello');
      using c = csp.BindingsTestType.create(42, 'hello');
      expect(csp.BindingsTestType.aliveCount).toBe(before + 3);
    }
    expect(csp.BindingsTestType.aliveCount).toBe(before);
  });

  it('Releases C++ memory when scope exits via exception', () => {
    const before = csp.BindingsTestType.aliveCount;

    expect(() => {
      using t = csp.BindingsTestType.create(42, 'hello');
      expect(csp.BindingsTestType.aliveCount).toBe(before + 1);
      throw new Error('Oh no!');
    }).toThrow('Oh no!');

    expect(csp.BindingsTestType.aliveCount).toBe(before);
  });

  it('Throws on double delete', () => {
    const t = csp.BindingsTestType.create(42, 'hello');
    t.delete();
    expect(() => t.delete()).toThrow("BindingsTestType instance already deleted");
  });

  it('Throws when accessing a deleted handle', () => {
    const t = csp.BindingsTestType.create(42, 'hello');
    t.delete();
    expect(() => t.value).toThrow("cannot call emscripten binding method BindingsTestType.value getter on deleted object");
  });

  /*
   * Array<T> memory tests.
   * We've gone to some effort to allow Array<T>'s (and other containers) that come out of CSP
   * to be declared with `using`, and get automatically deallocated at scope exit. 
   */

  it('Using on a returned array of handles releases every element at scope exit', () => {
    using bindingsArrayHelper = csp.BindingsMechanismsTestType.create();
    using elem1 = csp.BindingsTestType.create(1, 'one');
    using elem2 = csp.BindingsTestType.create(2, 'two');
    bindingsArrayHelper.setArrayFullTypeByValue([elem1, elem2]);

    const beforeGet = csp.BindingsTestType.aliveCount;
    {
      using arr = bindingsArrayHelper.getArrayFullTypeByValue();
      expect(csp.BindingsTestType.aliveCount).toBe(beforeGet + 2);
      expect(arr.length).toBe(2);
    }
    // Scope exit, destructors should run.
    expect(csp.BindingsTestType.aliveCount).toBe(beforeGet);
  });

  it('Using on a const-ref returned array of handles releases every element at scope exit', () => {
    /* Consider if this is a smell. We're testing const ref here just because it is a potential
     * shape of the API, but const-ref or not, we always copy every C++ object over the boundary,
     * and thus this deletion is necessary. But should we always be copying? That is the question.
     */

    using bindingsArrayHelper = csp.BindingsMechanismsTestType.create();
    using elem1 = csp.BindingsTestType.create(1, 'one');
    bindingsArrayHelper.setArrayFullTypeByValue([elem1]);

    const beforeGet = csp.BindingsTestType.aliveCount;
    {
      using arr = bindingsArrayHelper.getArrayFullTypeByConstRef();
      expect(csp.BindingsTestType.aliveCount).toBe(beforeGet + 1);
      expect(arr.length).toBe(1);
    }
    // Scope exit, destructors should run.
    expect(csp.BindingsTestType.aliveCount).toBe(beforeGet);
  });

  it('Using on an array of basic types is a tolerated no-op on disposal', () => {
    using bindingsArrayHelper = csp.BindingsMechanismsTestType.create();
    bindingsArrayHelper.setArrayBasicTypeByValue([1, 2, 3]);
    expect(() => {
      using arr = bindingsArrayHelper.getArrayBasicTypeByValue();
      expect(arr.length).toBe(3);
    }).not.toThrow();
  });

  it('Releases array handles when the scope exits via exception', () => {
    using bindingsArrayHelper = csp.BindingsMechanismsTestType.create();
    using elem1 = csp.BindingsTestType.create(1, 'one');
    using elem2 = csp.BindingsTestType.create(2, 'two');
    bindingsArrayHelper.setArrayFullTypeByValue([elem1, elem2]);

    const beforeGet = csp.BindingsTestType.aliveCount;
    expect(() => {
      using arr = bindingsArrayHelper.getArrayFullTypeByValue();
      expect(csp.BindingsTestType.aliveCount).toBe(beforeGet + 2);
      throw new Error('Oh no!');
    }).toThrow('Oh no!');
    expect(csp.BindingsTestType.aliveCount).toBe(beforeGet);
  });

  it('Disposing an empty returned array is a no-op', () => {
    using bindingsArrayHelper = csp.BindingsMechanismsTestType.create();
    bindingsArrayHelper.setArrayFullTypeByValue([]);
    const beforeGet = csp.BindingsTestType.aliveCount;
    {
      using arr = bindingsArrayHelper.getArrayFullTypeByValue();
      expect(csp.BindingsTestType.aliveCount).toBe(beforeGet); //No types in empty array, no alive count increment
      expect(arr.length).toBe(0);
    }
    expect(csp.BindingsTestType.aliveCount).toBe(beforeGet);
  });

  it('Array getter result without `using` leaks until manually disposed', () => {
    using bindingsArrayHelper = csp.BindingsMechanismsTestType.create();
    using elem1 = csp.BindingsTestType.create(1, 'one');
    bindingsArrayHelper.setArrayFullTypeByValue([elem1]);

    const beforeGet = csp.BindingsTestType.aliveCount;
    const leaked = bindingsArrayHelper.getArrayFullTypeByValue();
    expect(csp.BindingsTestType.aliveCount).toBe(beforeGet + 1);
    // Symbol.dispose is reachable and cleans up.
    leaked[Symbol.dispose]();
    expect(csp.BindingsTestType.aliveCount).toBe(beforeGet);
  });

  it('Disposing a returned array does not affect the underlying C++ storage', () => {
    using bindingsArrayHelper = csp.BindingsMechanismsTestType.create();
    using elem1 = csp.BindingsTestType.create(1, 'one');
    bindingsArrayHelper.setArrayFullTypeByValue([elem1]);

    const beforeGet = csp.BindingsTestType.aliveCount;
    { using arr = bindingsArrayHelper.getArrayFullTypeByValue(); }
    expect(csp.BindingsTestType.aliveCount).toBe(beforeGet);
    // Storage survives despite the scope exit — we can fetch again.
    using arr2 = bindingsArrayHelper.getArrayFullTypeByValue();
    expect(arr2[0]!.value).toBe(1);
  });

  it('Pre-deleting an element in a returned array does not break dispose', () => {
    using bindingsArrayHelper = csp.BindingsMechanismsTestType.create();
    using elem1 = csp.BindingsTestType.create(1, 'one');
    using elem2 = csp.BindingsTestType.create(2, 'two');
    bindingsArrayHelper.setArrayFullTypeByValue([elem1, elem2]);

    const beforeGet = csp.BindingsTestType.aliveCount;
    expect(() => {
      using arr = bindingsArrayHelper.getArrayFullTypeByValue();
      arr[0]!.delete();
    }).not.toThrow();
    expect(csp.BindingsTestType.aliveCount).toBe(beforeGet);

    // The returned array is a copy, so explicitly deleting an element must not
    // touch the underlying C++ storage — re-fetching still yields both elements.
    using arr2 = bindingsArrayHelper.getArrayFullTypeByValue();
    expect(arr2.length).toBe(2);
    expect(arr2[0]!.value).toBe(1);
    expect(arr2[1]!.value).toBe(2);
  });

  it('disposeArray is directly callable as a manual alternative to using', () => {
    using bindingsArrayHelper = csp.BindingsMechanismsTestType.create();
    using elem1 = csp.BindingsTestType.create(1, 'one');
    bindingsArrayHelper.setArrayFullTypeByValue([elem1]);

    const beforeGet = csp.BindingsTestType.aliveCount;
    const arr = bindingsArrayHelper.getArrayFullTypeByValue();
    expect(csp.BindingsTestType.aliveCount).toBe(beforeGet + 1);
    csp.disposeArray(arr);
    expect(csp.BindingsTestType.aliveCount).toBe(beforeGet);
  });

  it('disposeArray on a plain JS array of primitives is tolerated', () => {
    expect(() => csp.disposeArray([1, 2, 3])).not.toThrow();
    expect(() => csp.disposeArray([])).not.toThrow();
  });

  it('disposeArray recurses into nested arrays of handles', () => {
    const a = csp.BindingsTestType.create(1, 'one');
    const b = csp.BindingsTestType.create(2, 'two');
    const before = csp.BindingsTestType.aliveCount;
    csp.disposeArray([[a, b]]);
    expect(csp.BindingsTestType.aliveCount).toBe(before - 2);
  });

  it('disposeArray throws on non-array input', () => {
    expect(() => csp.disposeArray(42)).toThrow();
    expect(() => csp.disposeArray('nope')).toThrow();
    expect(() => csp.disposeArray({})).toThrow();
  });

});

