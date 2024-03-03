#include "memory.h"
#include <stdlib.h>

uint8_t memory[0x10000];

/* Predefined spirtes representing hexadecimal digits (0-F) */
const uint8_t sprites[80] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, 0x20, 0x60, 0x20, 0x20, 0x70, 0xF0, 0x10,
    0xF0, 0x80, 0xF0, 0xF0, 0x10, 0xF0, 0x10, 0xF0, 0x90, 0x90, 0xF0, 0x10,
    0x10, 0xF0, 0x80, 0xF0, 0x10, 0xF0, 0xF0, 0x80, 0xF0, 0x90, 0xF0, 0xF0,
    0x10, 0x20, 0x40, 0x40, 0xF0, 0x90, 0xF0, 0x90, 0xF0, 0xF0, 0x90, 0xF0,
    0x10, 0xF0, 0xF0, 0x90, 0xF0, 0x90, 0x90, 0xE0, 0x90, 0xE0, 0x90, 0xE0,
    0xF0, 0x80, 0x80, 0x80, 0xF0, 0xE0, 0x90, 0x90, 0x90, 0xE0, 0xF0, 0x80,
    0xF0, 0x80, 0xF0, 0xF0, 0x80, 0xF0, 0x80, 0x80,
};

void memory_init()
{
  for (int i = 0; i < sizeof(sprites) / sizeof(uint8_t); i++) {
    memory[i] = sprites[i];
  }
}

uint8_t memory_read_byte(uint16_t address)
{
  return memory[address];
}

uint16_t memory_read_opcode(uint16_t address)
{
  uint16_t upper_byte = memory[address] << 8;
  uint16_t lower_byte = memory[address + 1];
  return upper_byte | lower_byte;
}

void memory_write(uint16_t address, uint8_t byte)
{
  memory[address] = byte;
}