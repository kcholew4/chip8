#pragma once
#include "controllers.h"
#include <SDL.h>
#include <stdbool.h>
#include <stdint.h>

#define CELL_SCALE 10
#define WINDOW_WIDTH (64 * CELL_SCALE)
#define WINDOW_HEIGHT (32 * CELL_SCALE)

bool display_init();
void display_destroy();

// To use this function display_init() must be called first
void display_render();
DisplayController *display_create_controller();
extern bool display_pending_render;