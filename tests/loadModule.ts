import createModule, { type MainModule } from 'connected-spaces-platform-bindings';

let cached: Promise<MainModule> | undefined;

// Module instantiation is expensive (wasm compile + pthread pool init),
// so share one instance across tests in the same browser page.
export function loadCSP(): Promise<MainModule> {
  cached ??= createModule();
  return cached;
}
