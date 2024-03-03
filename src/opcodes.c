#include "opcodes.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Opcode opcodes_table[0xFFFF + 1]; // 1.75MB for opcodes table

typedef enum { NNN, XKK, XYN, XY, X, NONE } OpcodeVarType;

bool replace_var(char *str, char *var, uint16_t value)
{
  char *found = strstr(str, var);
  if (found == NULL) { return false; }

  char hex_format[20];
  snprintf(hex_format, sizeof(hex_format), "%%0%lux", strlen(var));

  char *replacement = malloc(strlen(var) + 1);
  sprintf(replacement, hex_format, value);

  strncpy(found, replacement, strlen(var));
  free(replacement);
  return true;
}

void fill_disasm_template(uint16_t opcode, OpcodeVarType type,
                          const char *disasm_template, char *dest)
{
  strcpy(dest, disasm_template);
  switch (type) {
  case NNN: replace_var(dest, "nnn", opcode & 0x0FFF); break;
  case XKK:
    replace_var(dest, "x", opcode & 0x0F00);
    replace_var(dest, "kk", opcode & 0x00FF);
    break;
  case XYN:
    replace_var(dest, "x", opcode & 0x0F00);
    replace_var(dest, "y", opcode & 0x00F0);
    replace_var(dest, "n", opcode & 0x000F);
    break;
  case XY:
    replace_var(dest, "x", opcode & 0x0F00);
    replace_var(dest, "y", opcode & 0x00F0);
    break;
  case X: replace_var(dest, "x", opcode & 0x0F00); break;
  case NONE: break;
  }
}

OpcodeVarType get_type(const char *template)
{
  if (strstr(template, "nnn")) {
    return NNN;
  } else if (strstr(template, "xkk")) {
    return XKK;
  } else if (strstr(template, "xyn")) {
    return XYN;
  } else if (strstr(template, "xy")) {
    return XY;
  } else if (strstr(template, "x")) {
    return X;
  }
  return NONE;
}

uint16_t get_mask(OpcodeVarType type)
{
  switch (type) {
  case NNN:
  case XKK:
  case XYN: return 0x0FFF;
  case XY: return 0x0FF0;
  case X: return 0x0F00;
  default: return 0x0000;
  }
}

uint16_t get_min_opcode(const char template[5], OpcodeVarType type)
{
  char buffer[5];
  strcpy(buffer, template);
  switch (type) {
  case NNN: replace_var(buffer, "nnn", 0); break;
  case XKK: replace_var(buffer, "xkk", 0); break;
  case XYN: replace_var(buffer, "xyn", 0); break;
  case XY: replace_var(buffer, "xy", 0); break;
  case X: replace_var(buffer, "x", 0); break;
  case NONE: break;
  }
  return strtol(buffer, NULL, 16);
}

void opcode_register(const char template[5], CpuInstruction instruction,
                     const char disasm_template[20])
{
  OpcodeVarType type = get_type(template);
  uint16_t min_opcode = get_min_opcode(template, type);

  if (type == NONE) {
    opcodes_table[min_opcode].instruction = instruction;
    fill_disasm_template(min_opcode, type, disasm_template,
                         opcodes_table[min_opcode].disassembled);
    return;
  }

  uint16_t mask = get_mask(type);
  uint16_t max_opcode = min_opcode | mask;

  int shift_amount = 0;
  while ((mask & 1) != 1) {
    mask >>= 1;
    shift_amount++;
  }
  mask <<= shift_amount; // Shift mask back :D
  uint16_t step = 1 << shift_amount;

  for (int opcode = min_opcode; opcode <= max_opcode; opcode += step) {
    opcodes_table[opcode].instruction = instruction;
    fill_disasm_template(opcode, type, disasm_template,
                         opcodes_table[opcode].disassembled);
  }
}

Opcode *get_opcode(uint16_t opcode)
{
  return &opcodes_table[opcode];
}