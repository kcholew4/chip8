#include "display.h"
#include <SDL.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

uint8_t screen[64][32] = {0};

void clear_screen(SDL_Renderer *renderer)
{
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);
  for (int i = 0; i < 64; i++) {
    for (int j = 0; j < 32; j++) { screen[i][j] = 0; }
  }
}

void render_screen(SDL_Renderer *renderer)
{
  SDL_RenderPresent(renderer);
}

bool draw_sprite(SDL_Renderer *renderer, int x, int y, int h)
{
  const int w = 8;
  bool flipped = false;

  for (int i = x; i < x + w; i++) {
    for (int j = y; j < y + h; j++) {
      bool current_cell = screen[i][j];
      bool next_cell = current_cell ^ 1;

      if (current_cell == next_cell) { continue; }

      SDL_Rect rect = {(i * CELL_SCALE), (j * CELL_SCALE), CELL_SCALE,
                       CELL_SCALE};

      if (current_cell && !next_cell) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        if (!flipped) { flipped = true; }
      } else {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
      }

      SDL_RenderFillRect(renderer, &rect);
      screen[i][j] = next_cell;
    }
  }
  return flipped;
}