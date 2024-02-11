#pragma once
#include <stdbool.h>
#include <stdint.h>

typedef struct {
  uint8_t bytes[0xF000];
  uint16_t stack[0x10];
} Memory;

uint8_t memory_read_byte(Memory *memory, uint16_t address);
void write_memory_byte(Memory *memory, uint16_t address, uint8_t byte);
Memory *memory_create();