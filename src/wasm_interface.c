#include "cpu.h"
#include "emulation.h"
#include "memory.h"
#include <emscripten.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

EMSCRIPTEN_KEEPALIVE void wasm_key_event(SDL_KeyCode key, bool up)
{
  SDL_Event e;
  e.type = up ? SDL_KEYUP : SDL_KEYDOWN;
  e.key.keysym.sym = key;
  SDL_PushEvent(&e);
}

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

EMSCRIPTEN_KEEPALIVE void wasm_normal_execution()
{
  cpu->step_execution = false;
}

EMSCRIPTEN_KEEPALIVE void wasm_step_execution()
{
  cpu->step_execution = true;
}

EMSCRIPTEN_KEEPALIVE void wasm_cpu_step()
{
  if (cpu->step_execution) { cpu->paused = false; }
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