#pragma once
#include <stdbool.h>
#include <stdint.h>

typedef struct {
  uint8_t bytes[0xF000];
  uint16_t stack[0x10];
  bool error;
} Memory;

uint8_t read_memory_byte(Memory *memory, uint16_t address);
void write_memory_byte(Memory *memory, uint16_t address, uint8_t byte);
Memory *create_memory();