#include "emulation.h"
#include "cpu.h"
#include <SDL.h>
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

  for (int i = 0; i < file_size; i++) { memory_write(0x200 + i, buffer[i]); }

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
  case SDLK_k: cpu->step_execution = !cpu->step_execution; break;
  case SDLK_l: cpu->paused = false; break;
  }
}

bool quit = false;

void sync()
{
  SDL_Event e;
  while (SDL_PollEvent(&e) != 0) {
    switch (e.type) {
    case SDL_QUIT: quit = true; break;
    case SDL_KEYDOWN: handle_key(e.key); break;
    case SDL_KEYUP: cpu->key = 0xFF;
    }
  }
}

void emulation_init(char executable[])
{
  memory_init();
  display_init();
  SDL_Init(SDL_INIT_VIDEO);
  cpu_create();
  cpu->sync = sync;

  if (!load_program(executable)) { quit = true; }

  while (!quit) {
    uint64_t frame_start = SDL_GetTicks64();

    sync();

    for (int cycles = 0; cycles < 1000 && !display->pending_render; cycles++) {
      if (cpu->step_execution && !cpu->paused) {
        cpu_cycle();
        cpu->paused = true;
      }

      if (!cpu->step_execution) { cpu_cycle(); }
    }

    display_refresh();

    uint64_t frame_time = SDL_GetTicks64() - frame_start;
    if ((1000 / 60) > frame_time) { SDL_Delay((1000 / 60) - frame_time); }
  };

  SDL_Quit();
}