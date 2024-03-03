#include "emulation.h"
#include <stdio.h>

#ifndef TEST_ENV

int main(int argc, char *argv[])
{
  if (argc != 2) {
    printf("Usage: chip8 [program]\n");
    return 1;
  }

  emulation_init(argv[1]);
  return 0;
}

#endif