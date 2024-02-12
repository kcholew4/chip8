#pragma once
#include <stdbool.h>
#include <stdint.h>

typedef struct {
  void (*write)(uint16_t address, uint8_t byte);
  uint8_t (*read_byte)(uint16_t address);
  uint16_t (*read_opcode)(uint16_t address);
} MemoryController;

typedef struct {
  bool (*draw)(uint8_t *sprite, int n, int x, int y);
  void (*clear)();
} DisplayController;
