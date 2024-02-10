#include "cpu.h"
#include <SDL.h>
#include <stdbool.h>
#include <stdio.h>

void handle_key(SDL_KeyCode code)
{
  SDL_Log("Key pressed: %d", code);
}

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

  Display *display = create_display(renderer);
  Memory *memory = create_memory();
  CPU *cpu = create_cpu(memory, display);

  clear_screen(display);
  execute(cpu, 0xD005);

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