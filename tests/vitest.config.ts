import { defineConfig } from 'vitest/config';
import { playwright } from '@vitest/browser-playwright';
import { existsSync } from 'node:fs';
import { resolve } from 'node:path';

const BUILD_DIRS: Record<string, string> = {
  debug: 'build/Debug',
  release: 'build/Release',
};

const testDir = import.meta.dirname;
const repoRoot = resolve(testDir, '..');

export default defineConfig(() => {
  const variant = process.env.CSP_BUILD;
  const relative = variant ? BUILD_DIRS[variant] : undefined;
  if (!relative) {
    throw new Error(
      `CSP_BUILD must be one of: ${Object.keys(BUILD_DIRS).join(', ')} (got "${variant ?? ''}"). Use npm run test:debug or test:release.`,
    );
  }

  const buildDir = resolve(repoRoot, relative);
  if (!existsSync(buildDir)) {
    throw new Error(
      `Build directory not found: ${buildDir}. Build the bindings first (cmake --build --preset ${mode}).`,
    );
  }

  return {
    test: {
      globals: true,
      include: ['**/*.test.ts'],
      browser: {
        enabled: true,
        headless: true,
        provider: playwright(),
        instances: [{ browser: 'chromium' }],
      },
    },
    server: {
      // Emscripten's ES6 loader resolves its sibling .wasm via import.meta.url,
      // so the dev server needs filesystem access to the CMake build directory
      // (which lives outside this package's root).
      fs: { allow: [testDir, buildDir] },
      // SharedArrayBuffer (required by -pthread) needs cross-origin isolation.
      headers: {
        'Cross-Origin-Opener-Policy': 'same-origin',
        'Cross-Origin-Embedder-Policy': 'require-corp',
      },
    },
    resolve: {
      alias: {
        '@bindings': buildDir,
      },
    },
  };
});
