#include "includes/memory.h"
#include <stdlib.h>

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

uint8_t read_memory_byte(Memory *memory, uint16_t address)
{
  if (address > 0xFFF) {
    memory->error = true;
    return 0;
  }
  return memory->bytes[address];
}

void write_memory_byte(Memory *memory, uint16_t address, uint8_t byte)
{
  if (address > 0xFFF) {
    memory->error = true;
    return;
  }
  memory->bytes[address] = byte;
}

Memory *create_memory()
{
  Memory *memory = calloc(1, sizeof(Memory));
  memory->error = false;
  for (int i = 0; i < sizeof(sprites) / sizeof(uint8_t); i++) {
    memory->bytes[i] = sprites[i];
  }
  return memory;
}