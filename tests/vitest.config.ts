import { defineConfig } from 'vitest/config';
import { playwright } from '@vitest/browser-playwright';
import { existsSync } from 'node:fs';
import { resolve } from 'node:path';

const testDir = import.meta.dirname;
const repoRoot = resolve(testDir, '..');
// Arbitrary install location, just what I decided to call it.
const installDir = resolve(repoRoot, 'CSP-WASM-Bindings');

// Shared config body. `debug` runs headed with the Chrome DWARF extension loaded
// for C++ source-level debugging; CSP_DWARF_EXT points at the unpacked DWARF
// debugger chrome extension:
// https://chromewebstore.google.com/detail/cc++-devtools-support-dwa/pdcpmagijalfljmkmjngeonclgbbannb
export function makeConfig({ debug = false } = {}) {
  if (!existsSync(installDir)) {
    throw new Error(
      `Bindings install directory not found: ${installDir}. Build and install the bindings first.`,
    );
  }

  const dwarfExt = process.env.CSP_DWARF_EXT;
  const loadDwarfExt = debug && !!dwarfExt;

  return {
    test: {
      globals: true,
      include: ['**/*.test.ts'],
      browser: {
        enabled: true,
        headless: !debug,
        screenshotFailures: false,
        provider: playwright(
          loadDwarfExt
            ? {
                // launchPersistentContext + headed is the only way Playwright loads extensions.
                persistentContext: true,
                launchOptions: {
                  args: [
                    `--disable-extensions-except=${dwarfExt}`,
                    `--load-extension=${dwarfExt}`,
                  ],
                },
              }
            : {},
        ),
        instances: [{ browser: 'chromium' as const }],
      },
    },
    server: {
      // Emscripten's ES6 loader resolves its sibling .wasm via import.meta.url,
      // so the dev server needs filesystem access to the install directory
      // (which lives outside this package's root).
      fs: { allow: [testDir, installDir] },
      // SharedArrayBuffer (required by -pthread) needs cross-origin isolation.
      headers: {
        'Cross-Origin-Opener-Policy': 'same-origin',
        'Cross-Origin-Embedder-Policy': 'require-corp',
      },
    },
    resolve: {
      alias: {
        'connected-spaces-platform-bindings': resolve(
          installDir,
          'connected-spaces-platform-bindings.js',
        ),
      },
    },
  };
}

export default defineConfig(() => makeConfig());
