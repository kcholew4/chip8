#pragma once
#include <SDL.h>
#include <stdbool.h>

#define CELL_SCALE 10
#define WINDOW_WIDTH (64 * CELL_SCALE)
#define WINDOW_HEIGHT (32 * CELL_SCALE)

bool draw_sprite(SDL_Renderer *renderer, int x, int y, int h);
void clear_screen(SDL_Renderer *renderer);
void render_screen(SDL_Renderer *renderer);