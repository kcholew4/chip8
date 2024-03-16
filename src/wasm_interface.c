#include "cpu.h"
#include "emulation.h"
#include "memory.h"
#include <emscripten.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

// TODO: Do all the init when module instance is created

EMSCRIPTEN_KEEPALIVE void wasm_init_devices()
{
  init_devices();
}

EMSCRIPTEN_KEEPALIVE void wasm_memory_write(uint16_t address, uint8_t byte)
{
  memory_write(address, byte);
}

EMSCRIPTEN_KEEPALIVE void wasm_emulation_start()
{
  emulation_start();
}

EMSCRIPTEN_KEEPALIVE void wasm_emulation_end()
{
  emulation_end();
}

EMSCRIPTEN_KEEPALIVE bool wasm_is_running()
{
  return isRunning;
}

EMSCRIPTEN_KEEPALIVE uint8_t wasm_get_v(int index)
{
  return cpu->V[index];
}

EMSCRIPTEN_KEEPALIVE uint16_t wasm_get_i()
{
  return cpu->I;
}

EMSCRIPTEN_KEEPALIVE uint16_t wasm_get_sp()
{
  return cpu->SP;
}

EMSCRIPTEN_KEEPALIVE uint16_t wasm_get_pc()
{
  return cpu->PC;
}