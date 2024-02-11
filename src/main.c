#include "cpu/cpu.h"
#include "display.h"
#include "memory.h"
#include <SDL.h>
#include <stdbool.h>
#include <stdio.h>

int main()
{
  SDL_Init(SDL_INIT_VIDEO);

  SDL_Window *window =
      SDL_CreateWindow("Chip8 Emulator", SDL_WINDOWPOS_UNDEFINED,
                       SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);

  if (window == NULL) {
    SDL_Log("Could not create window: %s", SDL_GetError());
    return 1;
  }

  SDL_Renderer *renderer =
      SDL_CreateRenderer(window, 0, SDL_RENDERER_PRESENTVSYNC);

  if (renderer == NULL) {
    SDL_Log("Could not create renderer: %s", SDL_GetError());
    return 1;
  }

  bool quit = false;
  SDL_Event e;

  FILE *file = fopen("../programs/Airplane.ch8", "rb");

  if (file == NULL) {
    SDL_Log("Can't open file");
    return 1;
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
    return 1;
  }

  free(buffer);

  while (!quit) {
    while (SDL_PollEvent(&e) != 0) {
      switch (e.type) {
      case SDL_QUIT: quit = true; break;
      }
    }

    SDL_Delay(20);
  };

  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}