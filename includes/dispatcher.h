#pragma once
#include "cpu.h"
#include "instructions.h"

typedef void(CpuInstruction)(CPU *cpu, uint16_t opcode);

extern CpuInstruction *instructions_table[0xFFFF + 1]; // Takes 64KB

void dispatcher_register(uint16_t type, uint16_t placeholder,
                         CpuInstruction *instruction);