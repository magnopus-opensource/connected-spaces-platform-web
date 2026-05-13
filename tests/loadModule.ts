import type { CSPModule } from './csp';
import createModule from 'connected-spaces-platform-bindings';

let cached: Promise<CSPModule> | undefined;

// Module instantiation is expensive (wasm compile + pthread pool init),
// so share one instance across tests in the same browser page.
export function loadCSP(): Promise<CSPModule> {
  cached ??= createModule();
  return cached;
}
