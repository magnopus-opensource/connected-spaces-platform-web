/**
 * Values passed from vitest.config.ts to the integration tests via Vitest's `provide`/`inject`
 * mechanism
 */
declare module 'vitest' {
  export interface ProvidedContext {
    endpoint: string;
    tenant: string;
  }
}

/*
 * Hack to make this file a module and ensure the above is added to Vitest's existing
 * ProvidedContext interface instead of declaring a new 'vitest' module.
 */
export {};
