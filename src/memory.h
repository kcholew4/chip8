#pragma once
#include <stddef.h>
#include <stdint.h>

void memory_init();
void memory_destroy();

uint8_t memory_read_byte(uint16_t address);
uint16_t memory_read_opcode(uint16_t address);

void memory_write(uint16_t address, uint8_t byte);
void memory_write_bytes(uint16_t start, uint8_t *bytes, size_t size);