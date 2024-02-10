#include "includes/display.h"

Display *create_display(SDL_Renderer *renderer)
{
  Display *display = calloc(1, sizeof(Display));
  display->renderer = renderer;
  return display;
}

void clear_screen(Display *display)
{
  SDL_SetRenderDrawColor(display->renderer, 0, 0, 0, 255);
  SDL_RenderClear(display->renderer);
  for (int i = 0; i < 64; i++) {
    for (int j = 0; j < 32; j++) { display->pixels[i][j] = false; }
  }
}

void render_screen(Display *display)
{
  SDL_RenderPresent(display->renderer);
}

bool *sprite_to_bits(uint8_t *sprite, int n)
{
  const int bits = 8 * n;
  bool *bits_arr = calloc(1, sizeof(bool) * bits);

  for (int i = 0; i < bits; i++) {
    uint8_t mask = (0b10000000 >> i % 8);
    int byte_number = i / 8;
    bits_arr[i] = mask & sprite[byte_number];
  }

  return bits_arr;
}

bool draw_sprite(Display *display, uint8_t *sprite, int n, int x, int y)
{
  bool collision = false;
  bool *sprite_bits = sprite_to_bits(sprite, n);
  int bits_to_draw = 8 * n;
  int drawn = 0;

  while (drawn < bits_to_draw) {
    int collumn = drawn % 8 + x;
    int row = drawn / 8 + y;

    int tmp = sprite_bits[drawn];

    bool current_cell = display->pixels[collumn][row];
    bool next_cell = current_cell ^ sprite_bits[drawn];

    if (current_cell == next_cell) {
      drawn++;
      continue;
    }

    SDL_Rect rect = {(collumn * CELL_SCALE), (row * CELL_SCALE), CELL_SCALE,
                     CELL_SCALE};

    if (current_cell && !next_cell) {
      SDL_SetRenderDrawColor(display->renderer, 0, 0, 0, 255);
      if (!collision) { collision = true; }
    } else {
      SDL_SetRenderDrawColor(display->renderer, 255, 255, 255, 255);
    }

    SDL_RenderFillRect(display->renderer, &rect);
    display->pixels[collumn][row] = next_cell;

    drawn++;
  }
  free(sprite_bits);
  return collision;
}