#pragma once
#include "display.h"
#include "memory.h"
#include <stdbool.h>
#include <stdint.h>
#include <time.h>

typedef struct {
  uint8_t V[16];
  uint16_t I;
  uint16_t PC;
  uint16_t SP;
  uint8_t DT;
  uint8_t ST;
  Memory *memory;
  Display *display;
} CPU;

void execute(CPU *cpu, uint16_t instruction);
CPU *create_cpu(Memory *memory, Display *display);