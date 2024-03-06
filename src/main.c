#include "emulation.h"
#include <stdio.h>

#ifndef TEST_ENV

int main(int argc, char *argv[])
{
#ifndef __EMSCRIPTEN__
  if (argc != 2) {
    printf("Usage: chip8 [program]\n");
    return 1;
  }

  // emulation_init(argv[1]); <-- Change that
#endif

  return 0;
}

#endif