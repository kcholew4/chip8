/// <reference types="emscripten"/>
// @ts-expect-error - shit's probably broken
import Module from './chip8_module.js';

export class Chip8 {
  public static async createInstance(canvas: HTMLCanvasElement) {
    // TODO: Error handling
    return new Chip8(
      await Module({
        canvas,
        locateFile: (path: string, prefix: string) => {
          const url = new URL(prefix);
          return url.origin + '/chip8/' + path;
        },
        print: (...args: string[]) => console.log(args.join(' '))
      })
    );
  }

  private module: { ccall: typeof ccall };

  constructor(module: { ccall: typeof ccall }) {
    this.module = module;
    this.init();
  }

  private init() {
    return this.module.ccall('wasm_init', null, [], []);
  }

  private cleanup() {
    return this.module.ccall('wasm_cleanup', null, [], []);
  }

  public oneIter() {
    return this.module.ccall('wasm_one_iter', null, [], []);
  }

  public memoryWriteByte(address: number, byte: number) {
    return this.module.ccall('wasm_memory_write', null, ['number', 'number'], [address, byte]);
  }

  public getV(index: number) {
    return this.module.ccall('wasm_get_v', 'number', ['number'], [index]);
  }

  public getI() {
    return this.module.ccall('wasm_get_i', 'number', [], []);
  }

  public getSP() {
    return this.module.ccall('wasm_get_sp', 'number', [], []);
  }

  public getPC() {
    return this.module.ccall('wasm_get_pc', 'number', [], []);
  }
}
