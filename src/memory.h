#pragma once
#include <stdint.h>

void memory_init();
uint8_t memory_read_byte(uint16_t address);
uint16_t memory_read_opcode(uint16_t address);
void memory_write(uint16_t address, uint8_t byte);

extern uint8_t memory[0x10000];