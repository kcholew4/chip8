#pragma once
#include <SDL.h>
#include <stdbool.h>

#define CELL_SCALE 10
#define WINDOW_WIDTH (64 * CELL_SCALE)
#define WINDOW_HEIGHT (32 * CELL_SCALE)

void render_screen(SDL_Renderer *renderer);
bool draw_sprite(int x, int y, int h);