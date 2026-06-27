import { defineConfig } from 'vitest/config';
import { makeConfig } from './vitest.config';

// Headed run with the Chrome DWARF extension for C++ source-level debugging.
export default defineConfig(() => makeConfig({ debug: true }));
