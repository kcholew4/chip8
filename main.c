#include <SDL.h>
#include <stdbool.h>
#include <stdio.h>

#define CELL_SCALE 10
#define WINDOW_WIDTH (64 * CELL_SCALE)
#define WINDOW_HEIGHT (32 * CELL_SCALE)

void handle_key(SDL_KeyCode code)
{
  SDL_Log("Key pressed: %d", code);
}

int draw_cell(SDL_Renderer *renderer, int x, int y, bool fill)
{
  SDL_Rect rect = {x * CELL_SCALE, y * CELL_SCALE, CELL_SCALE, CELL_SCALE};

  if (!fill) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  } else {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  }

  int status = SDL_RenderFillRect(renderer, &rect);
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  return status;
}

int main()
{
  SDL_Init(SDL_INIT_VIDEO);

  SDL_Window *window = SDL_CreateWindow(
      "Chip8 Emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
      WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_METAL);

  if (window == NULL) {
    SDL_Log("Could not create window: %s", SDL_GetError());
    return 1;
  }

  SDL_Renderer *renderer =
      SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  if (renderer == NULL) {
    SDL_Log("Could not create renderer: %s", SDL_GetError());
    return 1;
  }

  bool quit = false;

  while (!quit) {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
      switch (e.type) {
      case SDL_QUIT: quit = true; break;
      case SDL_KEYDOWN: handle_key(e.key.keysym.sym); break;
      }
    }

    SDL_RenderClear(renderer);
    draw_cell(renderer, 32, 16, true);
    SDL_RenderPresent(renderer);

    SDL_Delay(100);
  };

  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}