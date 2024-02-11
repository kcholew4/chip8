#include "dispatcher.h"

CpuInstruction *instructions_table[0xFFFF + 1];

void dispatcher_register(uint16_t type, uint16_t placeholder,
                         CpuInstruction *instruction)
{
  if (placeholder == 0x0000) {
    instructions_table[type] = instruction;
    return;
  }

  uint16_t max_opcode = type | placeholder;

  int shift_amount = 0;
  while ((placeholder & 1) != 1) {
    placeholder >>= 1;
    shift_amount++;
  }

  uint16_t step = 1 << shift_amount;
  for (int i = type; i <= max_opcode; i += step) {
    instructions_table[i] = instruction;
  }
}