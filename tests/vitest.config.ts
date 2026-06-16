import { defineConfig } from 'vitest/config';
import { playwright } from '@vitest/browser-playwright';
import { existsSync } from 'node:fs';
import { resolve } from 'node:path';

const testDir = import.meta.dirname;
const repoRoot = resolve(testDir, '..');
// Arbitrary install location, just what I decided to call it.
const installDir = resolve(repoRoot, 'CSP-WASM-Bindings');

export default defineConfig(() => {
  if (!existsSync(installDir)) {
    throw new Error(
      `Bindings install directory not found: ${installDir}. Build and install the bindings first.`,
    );
  }

  return {
    test: {
      globals: true,
      include: ['**/*.test.ts'],
      browser: {
        enabled: true,
        headless: true,
        screenshotFailures: false,
        provider: playwright(),
        instances: [{ browser: 'chromium' }],
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
});
