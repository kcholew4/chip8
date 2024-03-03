#pragma once
#include <SDL.h>
#include <stdbool.h>
#include <stdint.h>

#define CELL_SCALE 10
#define WINDOW_WIDTH (64 * CELL_SCALE)
#define WINDOW_HEIGHT (32 * CELL_SCALE)

typedef struct {
  bool pixels[64][32];
  bool pending_render;
  SDL_Renderer *renderer;
  SDL_Window *window;
} Display;

extern Display *display;

bool display_init();
void display_destroy();

bool display_draw(uint8_t *sprite, int n, int x, int y);
void display_clear();
void display_refresh();