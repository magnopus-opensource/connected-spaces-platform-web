// Hand-written ambient types for the embind module surface.
// Eventually this will be generated/authored alongside the bindings;
// for now it tracks Bindings.cpp by hand.

export interface CSPModule {
  GetVersion(): string;

  // Emscripten runtime helpers exposed via -sEXPORTED_RUNTIME_METHODS.
  _exit(code: number): void;
}

export type CSPModuleFactory = (overrides?: Partial<CSPModule>) => Promise<CSPModule>;

declare module 'connected-spaces-platform-bindings' {
  const createModule: CSPModuleFactory;
  export default createModule;
}
