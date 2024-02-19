#include "cpu.h"
#include <stdint.h>

typedef void (*CpuInstruction)(CPU *cpu, uint16_t opcode);

typedef struct {
  CpuInstruction instruction;
  char disassembled[20];
} Opcode;

void opcode_register(const char template[5], CpuInstruction instruction,
                     const char disasm_template[20]);
Opcode *get_opcode(uint16_t opcode);