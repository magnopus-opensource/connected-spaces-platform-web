import { MainModule } from 'connected-spaces-platform-bindings';

/* Timer to let us busy-wait on callbacks finishing. */
export async function until(predicate: () => boolean, timeoutMs = 6000): Promise<void> {
  const deadline = performance.now() + timeoutMs;
  while (!predicate()) {
    if (performance.now() >= deadline) {
      throw new Error(`Until timed out after ${timeoutMs}ms`);
    }
    await new Promise((resolve) => setTimeout(resolve, 0));
  }
}

/*
 * We want to wait until all the callbacks are finished fully (including teardown) before we do
 * any lifetime assertions.
 * There is an internal atomic counter in the test backend that counts the number of off-thread
 * callbacks in flight. We combine this with a manually managed "callbackFired" boolean, to
 * get the two state questions we need, "Has the callback started" followed by "Has the callback finished".
 * It does not matter where `callbackFired` is set in the callback itself, so long as it's set somewhere.
 *
 * For awaitable callbacks used with async/await, use the `await untilCallbacksSettled(csp)` single-argument
 * form after awaiting the call under test ('callbackFired' in this case is implicit).
 */
export async function untilCallbacksSettled(csp: MainModule, callbackFired: () => boolean = () => true): Promise<void> {
  await until(callbackFired);
  await until(() => csp.CallbacksBindingMechanismsTestType.offThreadCallbacksInFlight() === 0);
}

export async function untilOffThreadCallbacksFinished(): Promise<void> {}

/* Forces the heap to grow, which lets us be sure we've escaped the pre-allocated
 * memory and are now into standard geometric growth. Used for testing heap sizes
 * and leaks and whatnot.
 * Pass a fresh module instance so the heap starts at exactly INITIAL_MEMORY.
 * This guarantees the sensitivity analysis below holds regardless of what other
 * tests have already allocated in the shared csp instance.
 */
export function forceHeapGrowth(freshCSPModule: any) {
  // The following is based on the Emscripten compiler settings at the time of writing
  // (see CMakeLists.txt).
  // If those ever change this test may need to be updated to remain valid:
  // - INITIAL_MEMORY=32MB
  // - ALLOW_MEMORY_GROWTH=1 (enabled)
  // - MEMORY_GROWTH_GEOMETRIC_STEP unset (defaults to 0.20)
  // - MEMORY_GROWTH_LINEAR_STEP unset (growth is geometric by default)
  // - MALLOC unset (defaults to dlmalloc)
  // (Documentation: https://emscripten.org/docs/tools_reference/settings_reference.html)

  using bindingsArrayHelper = freshCSPModule.ContainerBindingMechanismsTestType.create();
  using anchor = freshCSPModule.BindingsTestType.create(1, 'one');
  bindingsArrayHelper.setArrayOfPointersByValue([anchor]);

  // Leak large value-copied allocations until the heap is forced to grow, then take the
  // baseline. This bounds how much free space the pointer loop has to absorb in order to detect
  // any leaks.
  //
  // Using an initial memory size of 32MB and a geometric growth step of 0.20, the first growth
  // event will expand the heap by 32MB * 0.20 = 6.4MB, rounded up to the next 64KB WASM page
  // boundary so ~6.44MB.
  // With 500k iterations we can detect leaks of ~6.44MB / 500k = ~13.5 bytes per call.
  // dlmalloc's (Emscripten's default malloc) minimum chunk size is ~24 bytes, so this catches any
  // non-trivial WASM heap allocation that isn't freed - including the transient Array<T*> backing
  // buffer (~20 bytes) if embind ever failed to destroy it.

  // 512 KB string
  const bigString = 'x'.repeat(512 * 1024);
  using filler = freshCSPModule.BindingsTestType.create(0, bigString);
  bindingsArrayHelper.setArrayFullTypeByValue([filler]);

  // Leak memory until the heap grows
  let sentinel = (freshCSPModule as unknown as { HEAPU8: Uint8Array }).HEAPU8.byteLength;
  while ((freshCSPModule as unknown as { HEAPU8: Uint8Array }).HEAPU8.byteLength === sentinel) {
    const arr = bindingsArrayHelper.getArrayFullTypeByValue();
    void arr[0]?.value;
    // Intentional leak - we want these to accumulate and force a heap growth.
  }
}
