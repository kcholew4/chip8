#pragma once
#include <SDL.h>
#include <stdbool.h>
#include <stdint.h>

#define CELL_SCALE 10
#define WINDOW_WIDTH (64 * CELL_SCALE)
#define WINDOW_HEIGHT (32 * CELL_SCALE)

typedef struct {
  bool pixels[64][32];
  SDL_Renderer *renderer;
} Display;

Display *create_display(SDL_Renderer *renderer);
bool draw_sprite(Display *display, uint8_t *sprite, int size, int x, int y);
void clear_screen(Display *display);
void render_screen(Display *display);