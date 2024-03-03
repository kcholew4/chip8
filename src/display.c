#include "display.h"

Display *display;

void display_clear()
{
  display->pending_render = true;
  for (int i = 0; i < 64; i++) {
    for (int j = 0; j < 32; j++) { display->pixels[i][j] = false; }
  }
}

bool *sprite_to_bits(uint8_t *sprite, int n)
{
  const int bits = 8 * n;
  bool *bits_arr = calloc(1, sizeof(bool) * bits);

  for (int i = 0; i < bits; i++) {
    uint8_t mask = (0b10000000 >> i % 8);
    int byte_number = i / 8;
    bits_arr[i] = (mask & sprite[byte_number]) != 0;
  }

  return bits_arr;
}

// Put sprite in displays's memory
bool display_draw(uint8_t *sprite, int n, int x, int y)
{
  display->pending_render = true;
  bool collision = false;
  bool *sprite_bits = sprite_to_bits(sprite, n);
  int bits_to_draw = 8 * n;
  int drawn = 0;

  while (drawn < bits_to_draw) {
    int collumn = ((drawn % 8) + x) % 64;
    int row = ((drawn / 8) + y) % 32;

    bool current_state = display->pixels[collumn][row];
    bool next_state = current_state ^ sprite_bits[drawn];

    if (current_state == next_state) {
      drawn++;
      continue;
    }

    if (current_state && !next_state) { collision = true; }

    display->pixels[collumn][row] = next_state;
    drawn++;
  }
  free(sprite_bits);
  return collision;
}

void display_render()
{
  SDL_SetRenderDrawColor(display->renderer, 0, 0, 0, 255);
  SDL_RenderClear(display->renderer);
  for (int x = 0; x < 64; x++) {
    for (int y = 0; y < 32; y++) {
      if (!display->pixels[x][y]) { continue; }

      SDL_Rect rect = {(x * CELL_SCALE), (y * CELL_SCALE), CELL_SCALE,
                       CELL_SCALE};
      SDL_SetRenderDrawColor(display->renderer, 255, 255, 255, 255);

      SDL_RenderFillRect(display->renderer, &rect);
    }
  }
  SDL_RenderPresent(display->renderer);
  display->pending_render = false;
}

void display_refresh()
{
  if (display->pending_render) { display_render(); }
}

bool display_init()
{
  display = calloc(1, sizeof(Display));

  display->window =
      SDL_CreateWindow("Chip8 Emulator", SDL_WINDOWPOS_UNDEFINED,
                       SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);

  if (display->window == NULL) {
    SDL_Log("Could not create window: %s", SDL_GetError());
    return false;
  }

  display->renderer =
      SDL_CreateRenderer(display->window, -1, SDL_RENDERER_ACCELERATED);

  if (display->renderer == NULL) {
    SDL_Log("Could not create renderer: %s", SDL_GetError());
    SDL_DestroyWindow(display->window);
    return false;
  }

  return true;
}

void display_destroy()
{
  free(display);
  SDL_DestroyRenderer(display->renderer);
  SDL_DestroyWindow(display->window);
}