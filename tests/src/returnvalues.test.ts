import { describe, it, expect, beforeAll } from 'vitest';
import { loadCSP } from '../loadModule';
import type { MainModule } from 'connected-spaces-platform-bindings';

/*
 * We focused on complicated types first, callbacks, containers, etc.
 * Hence, we never really bothered to test basic returns.
 * Just add them here, plug the gap in the testing.
 */

describe('equality', () => {
  let csp: MainModule;

  beforeAll(async () => {
    csp = await loadCSP();
  });

  it('Value returns are disposable', () => {
    using helper = csp.ContainerBindingMechanismsTestType.create();
    const before = csp.BindingsTestType.aliveCount;

    {
      using singleValue = helper.getSingleFullTypeByValue();
      expect(singleValue.name).toBe('One');
      expect(singleValue.value).toBe(1);
      expect(csp.BindingsTestType.aliveCount).toBe(before + 1);
      expect(singleValue.value).toBe(1);
    }

    expect(csp.BindingsTestType.aliveCount).toBe(before);
  });

  it('Const ref returns are disposable', () => {
    using helper = csp.ContainerBindingMechanismsTestType.create();
    const before = csp.BindingsTestType.aliveCount;

    {
      using singleValueConstRef = helper.getSingleFullTypeAsConstRef();
      expect(singleValueConstRef.name).toBe('One');
      expect(singleValueConstRef.value).toBe(1);
      expect(csp.BindingsTestType.aliveCount).toBe(before + 1);
      expect(singleValueConstRef.value).toBe(1);
    }

    expect(csp.BindingsTestType.aliveCount).toBe(before);
  });

  it('Pointer returns are non owning', () => {
    using helper = csp.ContainerBindingMechanismsTestType.create();
    const before = csp.BindingsTestType.aliveCount;

    const singlePointer = helper.getSingleFullTypeAsPointer();
    expect(singlePointer?.name).toBe('One');
    expect(singlePointer?.value).toBe(1);

    expect(() => singlePointer?.delete()).toThrow();
    expect(() => singlePointer?.deleteLater()).toThrow();
    expect(() => singlePointer?.clone()).toThrow();
    expect(() => singlePointer?.[Symbol.dispose]()).toThrow();

    expect(singlePointer?.value).toBe(1);
    expect(csp.BindingsTestType.aliveCount).toBe(before);
  });
});
