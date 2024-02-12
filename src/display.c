#include "display.h"
#include "controllers.h"

bool pixels[64][32];
bool display_pending_render = false;
SDL_Renderer *renderer;
SDL_Window *window;

void display_clear()
{
  display_pending_render = true;
  for (int i = 0; i < 64; i++) {
    for (int j = 0; j < 32; j++) { pixels[i][j] = false; }
  }
}

void display_render()
{
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);
  for (int x = 0; x < 64; x++) {
    for (int y = 0; y < 32; y++) {
      if (!pixels[x][y]) { continue; }

      SDL_Rect rect = {(x * CELL_SCALE), (y * CELL_SCALE), CELL_SCALE,
                       CELL_SCALE};
      SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

      SDL_RenderFillRect(renderer, &rect);
    }
  }
  SDL_RenderPresent(renderer);
  display_pending_render = false;
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

// Put sprite in displays's memory
bool display_draw(uint8_t *sprite, int n, int x, int y)
{
  display_pending_render = true;
  bool collision = false;
  bool *sprite_bits = sprite_to_bits(sprite, n);
  int bits_to_draw = 8 * n;
  int drawn = 0;

  while (drawn < bits_to_draw) {
    int collumn = (drawn % 8) + (x % 64);
    int row = (drawn / 8) + (y % 32);

    bool current_cell = pixels[collumn][row];
    bool next_cell = current_cell ^ sprite_bits[drawn];

    if (current_cell == next_cell) {
      drawn++;
      continue;
    }

    pixels[collumn][row] = next_cell;
    drawn++;
  }
  free(sprite_bits);
  return collision;
}

bool display_init()
{
  window =
      SDL_CreateWindow("Chip8 Emulator", SDL_WINDOWPOS_UNDEFINED,
                       SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);

  if (window == NULL) {
    SDL_Log("Could not create window: %s", SDL_GetError());
    return false;
  }

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  if (renderer == NULL) {
    SDL_Log("Could not create renderer: %s", SDL_GetError());
    SDL_DestroyWindow(window);
    return false;
  }

  return true;
}

void display_destroy()
{
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
}

DisplayController *display_create_controller()
{
  DisplayController *display_controller = malloc(sizeof(DisplayController));
  display_controller->draw = display_draw;
  display_controller->clear = display_clear;
  return display_controller;
}