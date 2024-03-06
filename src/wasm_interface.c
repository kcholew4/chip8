#include "emulation.h"
#include "memory.h"
#include <emscripten.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

EMSCRIPTEN_KEEPALIVE void wasm_init_devices()
{
  init_devices();
}

// Can't pass a pointer from javascript :(
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