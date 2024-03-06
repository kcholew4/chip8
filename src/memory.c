#include "memory.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint8_t *memory;

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

void memory_write_bytes(uint16_t start, uint8_t *bytes, size_t size)
{
  for (size_t i = 0; i < size; i++) { memory_write(start + i, bytes[i]); }
}

void memory_init()
{
  memory = calloc(0x10000, sizeof(uint8_t));
  memory_write_bytes(0, sprites, sizeof(sprites));
}

void memory_destroy()
{
  free(memory);
}