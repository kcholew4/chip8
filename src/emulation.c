#include "emulation.h"
#include "cpu.h"
#include <SDL.h>
#include <emscripten/emscripten.h>
#include <inttypes.h>
#include <stdbool.h>

bool load_program(char name[])
{
  FILE *file = fopen(name, "rb");
  if (file == NULL) {
    SDL_Log("Can't open file");
    return false;
  }

  fseek(file, 0, SEEK_END);
  size_t file_size = ftell(file);
  rewind(file);

  SDL_Log("File size: %zu bytes", file_size);

  uint8_t *buffer = malloc(file_size);

  size_t result = fread(buffer, 1, file_size, file);

  if (result != file_size) {
    SDL_Log("Error reading file");
    free(buffer);
    fclose(file);
    return false;
  }

  memory_write_bytes(0x200, buffer, file_size);

  free(buffer);
  SDL_Log("File read correctly");
  return true;
}

void handle_key(SDL_KeyboardEvent key)
{
  switch (key.keysym.sym) {
  case SDLK_1: cpu->key = 0x1; break;
  case SDLK_2: cpu->key = 0x2; break;
  case SDLK_3: cpu->key = 0x3; break;
  case SDLK_4: cpu->key = 0xC; break;
  case SDLK_q: cpu->key = 0x4; break;
  case SDLK_w: cpu->key = 0x5; break;
  case SDLK_e: cpu->key = 0x6; break;
  case SDLK_r: cpu->key = 0xD; break;
  case SDLK_a: cpu->key = 0x7; break;
  case SDLK_s: cpu->key = 0x8; break;
  case SDLK_d: cpu->key = 0x9; break;
  case SDLK_f: cpu->key = 0xE; break;
  case SDLK_z: cpu->key = 0xA; break;
  case SDLK_x: cpu->key = 0x0; break;
  case SDLK_c: cpu->key = 0xB; break;
  case SDLK_v: cpu->key = 0xF; break;
  }
}

void sync()
{
  SDL_Event e;
  while (SDL_PollEvent(&e) != 0) {
    switch (e.type) {
    case SDL_KEYDOWN: handle_key(e.key); break;
    case SDL_KEYUP: cpu->key = 0xFF;
    }
  }
}

double iterations = 0;
int current_cycles = 12;
uint64_t last_timestamp = 0;

int freqInRange(double freq)
{
  const double error = 100; // Allow for 100Hz error;
  const double target = 600;

  if (freq > target + error) { return 1; }
  if (freq < target - error) { return -1; }
  return 0;
}

void one_iter()
{
  uint64_t timestamp = get_timestamp();

  if (timestamp - last_timestamp > 1000) {
    last_timestamp = timestamp;
    iterations = 0;
  }

  double duration = timestamp - last_timestamp + 1;

  iterations++;

  if (iterations > 5) {
    double freq = iterations * current_cycles / duration * 1000;
    // printf("%f: %f, %d, %f\n", freq, iterations, current_cycles, duration);

    if (freqInRange(freq) > 0) { current_cycles--; }

    if (freqInRange(freq) < 0) { current_cycles++; }
  }

  sync();

  for (int cycles = 0; cycles < current_cycles && !display->pending_render;
       cycles++) {
    if (cpu->step_execution && !cpu->paused) {
      cpu_cycle();
      cpu->paused = true;
    }

    if (!cpu->step_execution) { cpu_cycle(); }
  }

  display_refresh();
}

void emulation_init()
{
  memory_init();
  SDL_Init(SDL_INIT_VIDEO);
  display_init();
  cpu_create();
  cpu->sync = sync;
}

void emulation_cleanup()
{
  memory_destroy();
  display_destroy();
  cpu_destroy();
  SDL_Quit();
}