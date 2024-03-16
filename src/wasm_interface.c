#include "cpu.h"
#include "emulation.h"
#include "memory.h"
#include <emscripten.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

EMSCRIPTEN_KEEPALIVE void wasm_init()
{
  emulation_init();
}

// No need to call this, just discard the module instance
EMSCRIPTEN_KEEPALIVE void wasm_cleanup()
{
  emulation_cleanup();
}

EMSCRIPTEN_KEEPALIVE void wasm_one_iter()
{
  one_iter();
}

EMSCRIPTEN_KEEPALIVE void wasm_memory_write(uint16_t address, uint8_t byte)
{
  memory_write(address, byte);
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