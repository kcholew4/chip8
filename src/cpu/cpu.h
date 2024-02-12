#pragma once
#include "controllers.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
  uint8_t V[16];
  uint16_t I;
  uint16_t PC;
  uint16_t SP;
  uint8_t DT;
  uint8_t ST;
  uint16_t stack[0x10];
  MemoryController *memory_controller;
  DisplayController *display_controller;
} CPU;

CPU *cpu_create();
void cpu_set_memory_controller(CPU *cpu, MemoryController *memory_controller);
void cpu_set_display_controller(CPU *cpu,
                                DisplayController *display_controller);
void cpu_execute(CPU *cpu, uint16_t instruction);
uint16_t cpu_fetch(CPU *cpu);