import { describe, it, expect, beforeAll } from 'vitest';
import { server } from 'vitest/browser';
import { loadCSP } from '../../loadModule';
import { type MainModule } from 'connected-spaces-platform-bindings';

describe.skipIf(server.browser !== 'webkit')('DisposableStack', () => {
  let csp: MainModule;

  beforeAll(async () => {
    csp = await loadCSP();
  });

  /*
   * Tests for the DisposableStack polyfill.
   * The polyfill is required as Safari/WebKit does not support explicit resource management.
   * The tests run for WebKit only.
   *
   * Remove these tests when the polyfill is no longer required.
   */

  it('DisposableStack is available', () => {
    expect(typeof DisposableStack).toBe('function');
    expect(Object.prototype.toString.call(new DisposableStack())).toBe('[object DisposableStack]');
  });

  it('DisposableStack releases every used handle at scope exit', () => {
    const before = csp.BindingsTestType.aliveCount();
    {
      using stack = new DisposableStack();
      stack.use(csp.BindingsTestType.create(1, 'one'));
      stack.use(csp.BindingsTestType.create(2, 'two'));
      stack.use(csp.BindingsTestType.create(3, 'three'));
      expect(csp.BindingsTestType.aliveCount()).toBe(before + 3);
    }
    expect(csp.BindingsTestType.aliveCount()).toBe(before);
  });

  it('DisposableStack.use returns the value it was given', () => {
    using stack = new DisposableStack();
    const t = stack.use(csp.BindingsTestType.create(42, 'hello'));
    expect(t.value).toBe(42);
  });

  it('DisposableStack.use tolerates null and undefined', () => {
    using stack = new DisposableStack();
    expect(stack.use(null)).toBeNull();
    expect(stack.use(undefined)).toBeUndefined();
  });

  it('DisposableStack.use throws on a non-disposable value', () => {
    using stack = new DisposableStack();
    expect(() => stack.use({} as Disposable)).toThrow(TypeError);
  });

  it('DisposableStack disposes in reverse order', () => {
    const order: number[] = [];
    {
      using stack = new DisposableStack();
      stack.defer(() => order.push(1));
      stack.defer(() => order.push(2));
      stack.defer(() => order.push(3));
    }
    expect(order).toEqual([3, 2, 1]);
  });

  it('DisposableStack.adopt passes the value to the disposer', () => {
    const before = csp.BindingsTestType.aliveCount();
    {
      using stack = new DisposableStack();
      const t = stack.adopt(csp.BindingsTestType.create(1, 'one'), (v) => v.delete());
      expect(t.isDeleted()).toBe(false);
      expect(csp.BindingsTestType.aliveCount()).toBe(before + 1);
    }
    expect(csp.BindingsTestType.aliveCount()).toBe(before);
  });

  it('DisposableStack releases handles when the scope exits via exception', () => {
    const before = csp.BindingsTestType.aliveCount();
    expect(() => {
      using stack = new DisposableStack();
      stack.use(csp.BindingsTestType.create(1, 'one'));
      stack.use(csp.BindingsTestType.create(2, 'two'));
      expect(csp.BindingsTestType.aliveCount()).toBe(before + 2);
      throw new Error('Oh no!');
    }).toThrow('Oh no!');
    expect(csp.BindingsTestType.aliveCount()).toBe(before);
  });

  it('DisposableStack.dispose is idempotent and sets disposed', () => {
    const before = csp.BindingsTestType.aliveCount();
    const stack = new DisposableStack();
    stack.use(csp.BindingsTestType.create(1, 'one'));
    expect(stack.disposed).toBe(false);

    stack.dispose();
    expect(stack.disposed).toBe(true);
    expect(csp.BindingsTestType.aliveCount()).toBe(before);

    expect(() => stack.dispose()).not.toThrow();
    expect(csp.BindingsTestType.aliveCount()).toBe(before);
  });

  it('DisposableStack throws ReferenceError when used after disposal', () => {
    const stack = new DisposableStack();
    stack.dispose();
    expect(() => stack.use(csp.BindingsTestType.create(1, 'one'))).toThrow(ReferenceError);
    expect(() => stack.defer(() => {})).toThrow(ReferenceError);
    expect(() => stack.adopt(1, () => {})).toThrow(ReferenceError);
    expect(() => stack.move()).toThrow(ReferenceError);
  });

  it('DisposableStack.move transfers ownership to a new stack', () => {
    const before = csp.BindingsTestType.aliveCount();
    const original = new DisposableStack();
    original.use(csp.BindingsTestType.create(1, 'one'));
    original.use(csp.BindingsTestType.create(2, 'two'));

    const moved = original.move();
    expect(original.disposed).toBe(true);
    expect(moved.disposed).toBe(false);

    // Disposing the original must not release anything - the handles now belong to `moved`.
    original.dispose();
    expect(csp.BindingsTestType.aliveCount()).toBe(before + 2);

    moved.dispose();
    expect(csp.BindingsTestType.aliveCount()).toBe(before);
  });

  it('DisposableStack keeps disposing after a disposer throws, then rethrows', () => {
    const before = csp.BindingsTestType.aliveCount();
    const stack = new DisposableStack();
    stack.use(csp.BindingsTestType.create(1, 'one'));
    stack.defer(() => {
      throw new Error('disposer failed');
    });
    stack.use(csp.BindingsTestType.create(2, 'two'));

    expect(() => stack.dispose()).toThrow('disposer failed');
    // Handles registered either side of the throwing disposer are still released.
    expect(csp.BindingsTestType.aliveCount()).toBe(before);
  });

  it('DisposableStack wraps multiple disposer errors in a SuppressedError', () => {
    const stack = new DisposableStack();
    stack.defer(() => {
      throw new Error('first registered');
    });
    stack.defer(() => {
      throw new Error('second registered');
    });

    let caught: unknown;
    try {
      stack.dispose();
    } catch (e) {
      caught = e;
    }

    // Reverse order: 'second registered' runs first and is then suppressed by 'first registered'.
    expect(caught).toBeInstanceOf(SuppressedError);
    const suppressedError = caught as SuppressedError;
    expect((suppressedError.error as Error).message).toBe('first registered');
    expect((suppressedError.suppressed as Error).message).toBe('second registered');
  });
});
