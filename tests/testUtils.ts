/* Timer to let us busy-wait on callbacks finishing. */
export async function until(predicate: () => boolean, timeoutMs = 2000): Promise<void> {
  const deadline = performance.now() + timeoutMs;
  while (!predicate()) {
    if (performance.now() >= deadline) {
      throw new Error(`Until timed out after ${timeoutMs}ms`);
    }
    await new Promise((resolve) => setTimeout(resolve, 0));
  }
}
