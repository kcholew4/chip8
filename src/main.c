#include "emulation.h"
#include <stdio.h>

#ifndef TEST_ENV

int main(int argc, char *argv[])
{
  if (argc != 2) {
    printf("Usage: chip8 [program]\n");
    emulation_init("roms/games/Breakout (Brix hack) [David Winter, 1997].ch8");
    return 1;
  }

  emulation_init(argv[1]);
  return 0;
}

#endif