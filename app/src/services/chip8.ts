/// <reference types="emscripten"/>
// @ts-expect-error - shit's probably broken
import Module from './chip8_module.js';

let moduleInstance: {
  cwrap: typeof cwrap;
} | null = null;

export const createModuleInstance = async (canvas: HTMLCanvasElement) => {
  moduleInstance = await Module({
    canvas,
    locateFile: (path: string, prefix: string) => {
      const url = new URL(prefix);
      return url.origin + '/chip8/' + path;
    },
    print: (...args: string[]) => console.log(args.join(' '))
  });
};

export const getExportedFunctions = () => {
  if (!moduleInstance) {
    throw new Error("wasm module instance doesn't exist");
  }

  return {
    initDevices: moduleInstance.cwrap('wasm_init_devices', null, []),
    memoryWriteByte: moduleInstance.cwrap('wasm_memory_write', null, ['number', 'number']),
    emulationStart: moduleInstance.cwrap('wasm_emulation_start', null, []),
    emulationEnd: moduleInstance.cwrap('wasm_emulation_end', null, []),
    isRunning: moduleInstance.cwrap('wasm_is_running', null, [])
  };
};
