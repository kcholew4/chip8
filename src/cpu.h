#pragma once
#include "display.h"
#include "memory.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define COWGOD_COMPATIBLE

typedef void (*CpuSync)();

typedef struct {
  uint8_t V[16];
  uint16_t I;
  uint16_t PC;
  uint16_t SP;
  uint8_t DT;
  uint8_t ST;
  uint16_t stack[0x10];
  uint64_t dt_updated;
  uint64_t st_updated;
  CpuSync sync;
  uint8_t key; // Currently pressed key (0-F), if none: 0xFF

  bool key_wait;
  uint8_t key_v;
  uint16_t last_opcode;
} CPU;

extern CPU *cpu;

void cpu_create();
void cpu_destroy();

uint16_t cpu_fetch();
void cpu_execute(uint16_t instruction);
void cpu_cycle();