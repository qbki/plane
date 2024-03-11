/// <reference types="@types/emscripten" />

declare module 'plane' {
  // @types/emscripten doesn't contain all necessary types and embind throws
  // errors during compilation. Hence, I have to write types by hands.
  interface FixedEmscriptenModule extends EmscriptenModule {
    preRun: Array<(module: FixedEmscriptenModule) => void>;
    canvas: HTMLCanvasElement;
    getExceptionMessage: (error: number | WebAssembly.Exception) => [string, string];
    monitorRunDependencies: (left: number) => void;
  }
  declare const fn: EmscriptenModuleFactory<FixedEmscriptenModule>;
  export default fn;
}
