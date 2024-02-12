#include "emulation.h"
#include "controllers.h"
#include "cpu/cpu.h"
#include "display.h"
#include "memory.h"
#include <SDL.h>
#include <stdbool.h>

bool load_program(char name[], MemoryController *memory)
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

  for (int i = 0; i < file_size; i++) { memory->write(0x200 + i, buffer[i]); }

  free(buffer);
  return true;
}

void emulation_init(char executable[])
{
  SDL_Init(SDL_INIT_VIDEO);

  CPU *cpu = cpu_create();
  MemoryController *memory = memory_create_controller();
  DisplayController *display = display_create_controller();

  cpu_set_memory_controller(cpu, memory);
  cpu_set_display_controller(cpu, display);

  display_init();

  cpu->PC = 0x200;

  bool quit = false;
  SDL_Event e;

  if (!load_program(executable, memory)) { quit = true; }

  SDL_Log("File read correctly");

  uint64_t frames = 0;
  uint64_t instructions = 0;

  uint64_t runtime_start = SDL_GetTicks64();

  while (!quit) {
    uint64_t frame_start = SDL_GetTicks64();

    while (SDL_PollEvent(&e) != 0) {
      switch (e.type) {
      case SDL_QUIT: quit = true; break;
      }
    }

    while (!display_pending_render) {
      uint16_t opcode = cpu_fetch(cpu);
      cpu_execute(cpu, opcode);
      instructions++;
    }

    display_render();
    frames++;

    uint64_t frame_time = frame_start - SDL_GetTicks64();
    // There are better ways
    if ((1000 / 60) > frame_time) { SDL_Delay((1000 / 60) - frame_time); }
  };

  double runtime = (double)(SDL_GetTicks64() - runtime_start) / 1000;

  SDL_Log("Frames: %llu", frames);
  SDL_Log("Instructions: %llu", instructions);
  SDL_Log("Run time: %fs", runtime);
  SDL_Log("Average FPS: %f", frames / runtime);
  SDL_Log("Average instructions/s: %f", instructions / runtime);

  SDL_Quit();
}