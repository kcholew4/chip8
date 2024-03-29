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
          // TODO: Manual module instantiation
          const url = new URL(prefix);
          return url.origin + import.meta.env.BASE_URL + '/' + path; // <-- Every time createInstance() is called this path is requested, not good :(
        },
        print: (...args: string[]) => console.log(args.join(' '))
      })
    );
  }

  private module: { ccall: typeof ccall };
  public stepExecutionMode = false;

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

  public normalExecution() {
    this.stepExecutionMode = false;
    return this.module.ccall('wasm_normal_execution', null, [], []);
  }

  public stepExecution() {
    this.stepExecutionMode = true;
    return this.module.ccall('wasm_step_execution', null, [], []);
  }

  public cpuStep() {
    return this.module.ccall('wasm_cpu_step', null, [], []);
  }

  public keyEvent(key: number, type: 'up' | 'down') {
    const up = type === 'up';
    return this.module.ccall('wasm_key_event', null, ['number', 'boolean'], [key, up]);
  }

  public async loadProgram(program: File) {
    const bytes = new Uint8Array(await program.arrayBuffer());
    for (let i = 0; i < bytes.length; i++) {
      this.memoryWriteByte(0x200 + i, bytes[i]);
    }
  }

  public createMainLoop() {
    let requestId: ReturnType<typeof requestAnimationFrame>;

    const iteration = () => {
      this.oneIter();
      requestId = requestAnimationFrame(iteration);
    };

    iteration();

    return () => {
      cancelAnimationFrame(requestId);
    };
  }

  public dumpRegisters() {
    return {
      V: new Array(0x10).map((_, index) => this.getV(index)),
      I: this.getI(),
      SP: this.getSP(),
      PC: this.getPC()
    };
  }
}
