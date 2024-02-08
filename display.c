#include "display.h"
#include <SDL.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

uint8_t screen[64][32] = {0};

// TODO: Do error handling
void render_screen(SDL_Renderer *renderer)
{
  SDL_RenderClear(renderer);
  for (int x = 0; x < 64; x++) {
    for (int y = 0; y < 32; y++) {
      uint8_t cell = screen[x][y];
      SDL_Rect rect = {(x * CELL_SCALE), (y * CELL_SCALE), CELL_SCALE,
                       CELL_SCALE};
      if (cell) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
      } else {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
      }
      SDL_RenderFillRect(renderer, &rect);
    }
  }
  SDL_RenderPresent(renderer);
}

bool draw_sprite(int x, int y, int h)
{
  const int w = 8;
  bool flipped = false;
  for (int i = x; i < x + w; i++) {
    for (int j = y; j < y + h; j++) {
      bool is_on = screen[i][j] ^ 1;

      if (screen[i][j] && !is_on && !flipped) { flipped = true; }

      screen[i][j] = is_on;
    }
  }
  return flipped;
}