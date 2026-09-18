import { describe, it, expect, beforeAll } from 'vitest';
import { loadCSP } from '../../loadModule';
import type { MainModule } from 'connected-spaces-platform-bindings';

/*
 * Tests for NativeBuffer bindings, a JS-owned block of memory used to transfer binary data to CSP.
 *
 * Includes some basic tests for BufferAssetDataSource which is the only use of NativeBuffer
 * currently.
 */

const heapSize = (csp: MainModule) => (csp as unknown as { HEAPU8: Uint8Array }).HEAPU8.byteLength;

describe('NativeBuffer', () => {
  let csp: MainModule;

  beforeAll(async () => {
    csp = await loadCSP();
  });

  it('creates the a buffer of the requested length', () => {
    using buffer = csp.NativeBuffer.create(16);

    expect(buffer.length).toBe(16);
    expect(buffer.getView().byteLength).toBe(16);
  });

  it('rejects a zero length allocation', () => {
    expect(() => csp.NativeBuffer.create(0)).toThrow();
  });

  it('writes through a view into the underlying memory', () => {
    using buffer = csp.NativeBuffer.create(4);

    buffer.getView().set([1, 2, 3, 4]);

    expect(Array.from(buffer.getView())).toEqual([1, 2, 3, 4]);
  });

  it('returns a live alias from view, not a copy', () => {
    using buffer = csp.NativeBuffer.create(4);
    const view = buffer.getView();

    buffer.getView().set([9, 9, 9, 9]);

    expect(Array.from(view)).toEqual([9, 9, 9, 9]);
  });

  it('toByteArray returns a detached copy of the buffer', () => {
    using buffer = csp.NativeBuffer.create(4);
    buffer.getView().set([1, 2, 3, 4]);

    const copied = buffer.toByteArray();
    buffer.getView().set([5, 6, 7, 8]);

    expect(Array.from(copied)).toEqual([1, 2, 3, 4]);
    expect(Array.from(buffer.getView())).toEqual([5, 6, 7, 8]);
  });

  it('round trips content through createFromByteArray', () => {
    using buffer = csp.NativeBuffer.createFromByteArray(new Uint8Array([10, 20, 30]));

    expect(buffer.length).toBe(3);
    expect(Array.from(buffer.toByteArray())).toEqual([10, 20, 30]);
  });

  it('copies in createFromByteArray, leaving the source independent', () => {
    const source = new Uint8Array([1, 2, 3]);
    using buffer = csp.NativeBuffer.createFromByteArray(source);

    source.set([7, 7, 7]);

    expect(Array.from(buffer.toByteArray())).toEqual([1, 2, 3]);
  });

  it('releases its memory at scope exit', () => {
    const before = heapSize(csp);

    for (let i = 0; i < 64; i++) {
      using buffer = csp.NativeBuffer.create(1024 * 1024);

      buffer.getView().set([1]);
    }

    // 64MB of leaked buffer allocations (instead of 64 1MB allocations released each iteration)
    // would have forced the heap to grow past its 32MB start.
    expect(heapSize(csp)).toBe(before);
  });

  it('throws on double delete', () => {
    const buffer = csp.NativeBuffer.create(8);
    buffer.delete();

    expect(() => buffer.delete()).toThrow('NativeBuffer instance already deleted');
  });
});

describe('BufferAssetDataSource', () => {
  let csp: MainModule;

  beforeAll(async () => {
    csp = await loadCSP();
  });

  it('takes its length from the buffer passed to setBuffer', () => {
    using buffer = csp.NativeBuffer.createFromByteArray(new Uint8Array([1, 2, 3, 4]));
    using bufferAssetDataSource = csp.BufferAssetDataSource.create();

    bufferAssetDataSource.setBuffer(buffer);

    expect(bufferAssetDataSource.bufferLength).toBe(4);
    expect(Array.from(bufferAssetDataSource.getBufferView())).toEqual([1, 2, 3, 4]);
  });

  it('aliases the buffer rather than copying it', () => {
    using buffer = csp.NativeBuffer.create(4);
    using bufferAssetDataSource = csp.BufferAssetDataSource.create();

    bufferAssetDataSource.setBuffer(buffer);
    buffer.getView().set([5, 6, 7, 8]);

    expect(Array.from(bufferAssetDataSource.getBufferView())).toEqual([5, 6, 7, 8]);
  });
});
