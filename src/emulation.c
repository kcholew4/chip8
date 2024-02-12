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

  while (!quit) {
    while (SDL_PollEvent(&e) != 0) {
      switch (e.type) {
      case SDL_QUIT: quit = true; break;
      }
    }

    uint16_t opcode = cpu_fetch(cpu);
    cpu_execute(cpu, opcode);
    display_render();

    SDL_Delay(20);
  };

  SDL_Quit();
}