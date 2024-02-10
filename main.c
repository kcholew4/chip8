#include "includes/cpu.h"
#include <SDL.h>
#include <stdbool.h>
#include <stdio.h>

void handle_key(SDL_KeyCode code)
{
  SDL_Log("Key pressed: %d", code);
}

void display_predefined(CPU *cpu)
{
  clear_screen(cpu->display);
  for (int i = 0; i < 0x10; i++) {
    cpu->I = i * 5;

    int column = i % 4;
    int row = i / 4;

    cpu->V[0] = column * 5;
    cpu->V[1] = row * 6;
    execute(cpu, 0xD015);
  }
  render_screen(cpu->display);
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

  display_predefined(cpu);

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