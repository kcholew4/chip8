#include "instructions.h"

/*
nnn or addr - A 12-bit value, the lowest 12 bits of the instruction
n or nibble - A 4-bit value, the lowest 4 bits of the instruction
x - A 4-bit value, the lower 4 bits of the high byte of the instruction
y - A 4-bit value, the upper 4 bits of the low byte of the instruction
kk or byte - An 8-bit value, the lowest 8 bits of the instruction
*/

uint16_t _get_addr(uint16_t opcode)
{
  return opcode & 0x0FFF;
}

uint8_t _get_nibble(uint16_t opcode)
{
  return opcode & 0x000F;
}

uint8_t _get_x(uint16_t opcode)
{
  return (opcode & 0x0F00) >> 8;
}

uint8_t _get_y(uint16_t opcode)
{
  return (opcode & 0x00F0) >> 4;
}

uint8_t _get_byte(uint16_t opcode)
{
  return (opcode & 0x00FF);
}

void cpu_sys(CPU *cpu, uint16_t opcode)
{
  return;
}

void cpu_cls(CPU *cpu, uint16_t opcode)
{
  display_clear();
}

void cpu_ret(CPU *cpu, uint16_t opcode)
{
  cpu->PC = cpu->stack[--cpu->SP];
}

void cpu_jp(CPU *cpu, uint16_t opcode)
{
  uint16_t addr = _get_addr(opcode);
  cpu->PC = addr;
}

void cpu_call(CPU *cpu, uint16_t opcode)
{
  uint16_t addr = _get_addr(opcode);
  cpu->stack[cpu->SP++] = cpu->PC;
  cpu->PC = addr;
}

void cpu_se_vx(CPU *cpu, uint16_t opcode)
{
  int x = _get_x(opcode);
  uint8_t kk = _get_byte(opcode);
  if (cpu->V[x] == kk) { cpu->PC += 2; }
}

void cpu_sne_vx(CPU *cpu, uint16_t opcode)
{
  int x = _get_x(opcode);
  uint8_t kk = _get_byte(opcode);
  if (cpu->V[x] != kk) { cpu->PC += 2; }
}

void cpu_se_vx_vy(CPU *cpu, uint16_t opcode)
{
  int x = _get_x(opcode);
  int y = _get_y(opcode);
  if (cpu->V[x] == cpu->V[y]) { cpu->PC += 2; }
}

void cpu_ld_vx(CPU *cpu, uint16_t opcode)
{
  int x = _get_x(opcode);
  uint16_t kk = _get_byte(opcode);
  cpu->V[x] = kk;
}

void cpu_add_vx(CPU *cpu, uint16_t opcode)
{
  int x = _get_x(opcode);
  uint16_t kk = _get_byte(opcode);
  cpu->V[x] += kk;
}

void cpu_ld_vx_vy(CPU *cpu, uint16_t opcode)
{
  int x = _get_x(opcode);
  int y = _get_y(opcode);
  cpu->V[x] = cpu->V[y];
}

void cpu_or_vx_vy(CPU *cpu, uint16_t opcode)
{
  int x = _get_x(opcode);
  int y = _get_y(opcode);
  cpu->V[x] = cpu->V[x] | cpu->V[y];
}

void cpu_and_vx_vy(CPU *cpu, uint16_t opcode)
{
  int x = _get_x(opcode);
  int y = _get_y(opcode);
  cpu->V[x] = cpu->V[x] & cpu->V[y];
}

void cpu_xor_vx_vy(CPU *cpu, uint16_t opcode)
{
  int x = _get_x(opcode);
  int y = _get_y(opcode);
  cpu->V[x] = cpu->V[x] ^ cpu->V[y];
}

void cpu_add_vx_vy(CPU *cpu, uint16_t opcode)
{
  int x = _get_x(opcode);
  int y = _get_y(opcode);

  uint16_t sum = cpu->V[x] + cpu->V[y];
  if (sum > 0xFF) {
    cpu->V[0xF] = 1;
  } else {
    cpu->V[0xF] = 0;
  }

  cpu->V[x] = sum & 0x00FF;
}

void cpu_sub_vx_vy(CPU *cpu, uint16_t opcode)
{
  int x = _get_x(opcode);
  int y = _get_y(opcode);

  if (cpu->V[x] >= cpu->V[y]) {
    cpu->V[0xF] = 1;
  } else {
    cpu->V[0xF] = 0;
  }

  cpu->V[x] -= cpu->V[y];
}

void cpu_shr_vx_vy(CPU *cpu, uint16_t opcode)
{
  int x = _get_x(opcode);
  int y = _get_y(opcode);
  cpu->V[0xF] = (cpu->V[x] & 1);
#ifdef COWGOD_COMPATIBLE
  cpu->V[x] >>= 1;
#else
  cpu->V[x] = cpu->V[y] >> 1;
#endif
}

void cpu_subn_vx_vy(CPU *cpu, uint16_t opcode)
{
  int x = _get_x(opcode);
  int y = _get_y(opcode);

  if (cpu->V[y] > cpu->V[x]) {
    cpu->V[0xF] = 1;
  } else {
    cpu->V[0xF] = 0;
  }

  cpu->V[x] = cpu->V[y] - cpu->V[x];
}

void cpu_shl_vx_vy(CPU *cpu, uint16_t opcode)
{
  int x = _get_x(opcode);
  int y = _get_y(opcode);
  cpu->V[0xF] = (cpu->V[x] & 0x0080) >> 7;
#ifdef COWGOD_COMPATIBLE
  cpu->V[x] <<= 1;
#else
  cpu->V[x] = cpu->V[y] << 1;
#endif
}

void cpu_sne_vx_vy(CPU *cpu, uint16_t opcode)
{
  int x = _get_x(opcode);
  int y = _get_y(opcode);
  if (cpu->V[x] != cpu->V[y]) { cpu->PC += 2; }
}

void cpu_ld_i(CPU *cpu, uint16_t opcode)
{
  uint16_t addr = _get_addr(opcode);
  cpu->I = addr;
}

void cpu_jp_v0(CPU *cpu, uint16_t opcode)
{
  uint16_t addr = _get_addr(opcode);
  cpu->PC = addr + cpu->V[0];
}

void cpu_rnd_vx(CPU *cpu, uint16_t opcode)
{
  int x = _get_x(opcode);
  uint8_t kk = _get_byte(opcode);
  uint8_t rnd = rand() % 256;
  cpu->V[x] = rnd & kk;
}

void cpu_drw_vx_vy(CPU *cpu, uint16_t opcode)
{
  int x = _get_x(opcode);
  int y = _get_y(opcode);
  uint8_t n = _get_nibble(opcode);

  uint8_t *sprite = malloc(sizeof(uint8_t) * n);

  for (int i = 0; i < n; i++) { sprite[i] = memory_read_byte(cpu->I + i); }

  cpu->V[0xF] = display_draw(sprite, n, cpu->V[x], cpu->V[y]);
  free(sprite);
}

void cpu_skp_vx(CPU *cpu, uint16_t opcode)
{
  cpu->sync();
  uint8_t vx = cpu->V[_get_x(opcode)];
  if (cpu->key == vx) { cpu->PC += 2; }
}

void cpu_sknp_vx(CPU *cpu, uint16_t opcode)
{
  cpu->sync();
  uint8_t vx = cpu->V[_get_x(opcode)];
  if (cpu->key != vx) { cpu->PC += 2; }
}

void cpu_ld_vx_dt(CPU *cpu, uint16_t opcode)
{
  cpu->V[_get_x(opcode)] = cpu->DT;
}

void cpu_ld_vx_k(CPU *cpu, uint16_t opcode)
{
  cpu->key_wait = true;
  cpu->key_v = _get_x(opcode);
  cpu->sync();
}

void cpu_ld_dt_vx(CPU *cpu, uint16_t opcode)
{
  int x = _get_x(opcode);
  cpu->DT = cpu->V[x];
}

void cpu_ld_st_vx(CPU *cpu, uint16_t opcode)
{
  int x = _get_x(opcode);
  cpu->ST = cpu->V[x];
}

void cpu_add_i_vx(CPU *cpu, uint16_t opcode)
{
  cpu->I += cpu->V[_get_x(opcode)];
}

void cpu_ld_f_vx(CPU *cpu, uint16_t opcode)
{
  int vx = cpu->V[_get_x(opcode)];
  cpu->I = 5 * vx;
}

void cpu_ld_b_vx(CPU *cpu, uint16_t opcode)
{
  uint8_t vx = cpu->V[_get_x(opcode)];

  uint8_t hundreds = vx / 100;
  uint8_t tens = (vx - hundreds * 100) / 10;
  uint8_t ones = vx - hundreds * 100 - tens * 10;

  memory_write(cpu->I, hundreds);
  memory_write(cpu->I + 1, tens);
  memory_write(cpu->I + 2, ones);
}

void cpu_ld_i_vx(CPU *cpu, uint16_t opcode)
{
  int x = _get_x(opcode);
  for (int i = 0; i <= x; i++) { memory_write(cpu->I + i, cpu->V[i]); }
#ifndef COWGOD_COMPATIBLE
  cpu->I += x + 1;
#endif
}

void cpu_ld_vx_i(CPU *cpu, uint16_t opcode)
{
  int x = _get_x(opcode);
  for (int i = 0; i <= x; i++) { cpu->V[i] = memory_read_byte(cpu->I + i); }
#ifndef COWGOD_COMPATIBLE
  cpu->I += x + 1;
#endif
}