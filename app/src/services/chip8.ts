/// <reference types="emscripten"/>
// @ts-expect-error - shit's probably broken
import Module from './chip8_module.js';
import { useVMStore } from '@/stores/vm';

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
  const store = useVMStore();
  store.ready = true;
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
    isRunning: moduleInstance.cwrap('wasm_is_running', 'boolean', []),
    getV: moduleInstance.cwrap('wasm_get_v', 'number', ['number']),
    getI: moduleInstance.cwrap('wasm_get_i', 'number', []),
    getSP: moduleInstance.cwrap('wasm_get_sp', 'number', []),
    getPC: moduleInstance.cwrap('wasm_get_pc', 'number', [])
  };
};
