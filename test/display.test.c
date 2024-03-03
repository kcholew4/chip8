#include "display.h"
#include <criterion/criterion.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

void setup()
{
  SDL_Init(SDL_INIT_VIDEO);
  display_init();
}

void teardown()
{
  display_destroy();
  SDL_Quit();
}

void set_pixels(Display *display, bool value, size_t n)
{
  memset(display->pixels, value ? 0xFF : 0x00, n);
}

Test(display, clear_screen, .init = setup, .fini = teardown)
{
  set_pixels(display, 1, 64 * 32);
  display_clear();
  for (int x = 0; x < 64; x++) {
    for (int y = 0; y < 32; y++) {
      cr_assert(display->pixels[x][y] == false,
                "Pixel at (%d, %d) should be set to 0", x, y);
    }
  }
}

Test(display, draw_pixel, .init = setup, .fini = teardown)
{
  for (int y = 0; y < 32; y++) {
    for (int x = 0; x < 64; x++) {
      uint8_t pixel[1] = {0b10000000};
      display_draw(pixel, 1, x, y);
      display_render();
      SDL_PollEvent(NULL);
      cr_assert(display->pixels[x][y] == true,
                "Pixel should be set to 1 at (%d, %d)", x, y);
    }
  }
}

Test(display, draw_pixel_xor, .init = setup, .fini = teardown)
{
  set_pixels(display, 1, 64 * 32);
  for (int y = 0; y < 32; y++) {
    for (int x = 0; x < 64; x++) {
      uint8_t pixel[1] = {0b10000000};
      display_draw(pixel, 1, x, y);
      display_render();
      SDL_PollEvent(NULL);
      cr_assert(display->pixels[x][y] == false,
                "Pixel should be set to 0 at (%d, %d)", x, y);
    }
  }
}

Test(display, draw_pixel_wrap, .init = setup, .fini = teardown)
{
  for (int y = 0; y < 0x1F; y++) {
    for (int x = 0; x < 0x1F; x++) {
      display_clear();
      uint8_t pixel[1] = {0b10000000};
      display_draw(pixel, 1, x, y);
      display_render();
      SDL_PollEvent(NULL);
      cr_assert(display->pixels[x % 64][y % 32] == true,
                "Pixel should be set to 1 at (%d, %d) for x=%d, y=%d", x % 64,
                y % 32, x, y);
    }
  }
}

Test(display, draw_pixel_collision, .init = setup, .fini = teardown)
{
  for (int collision_x = 0; collision_x < 64; collision_x++) {
    for (int collision_y = 0; collision_y < 32; collision_y++) {
      display->pixels[collision_x][collision_y] = true;
    };

    for (int y = 0; y < 32; y++) {
      for (int x = 0; x < 64; x++) {
        uint8_t pixel[1] = {0b10000000};
        bool collision = display_draw(pixel, 1, x, y);

        if (x == collision_x) {
          cr_assert(collision == true,
                    "collision should be set to true at (%d, %d)", x, y);
        } else {
          cr_assert(collision == false,
                    "collision should be set to false at (%d, %d)", x, y);
        }
        display_render();
        SDL_PollEvent(NULL);
      }
    }
    display_clear();
  }
}