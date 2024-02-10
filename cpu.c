#include "includes/cpu.h"
#include "includes/display.h"
#include "includes/memory.h"

CPU *create_cpu(Memory *memory, Display *display)
{
  CPU *cpu = calloc(1, sizeof(CPU));
  cpu->memory = memory;
  cpu->display = display;
  return cpu;
}

void execute(CPU *cpu, uint16_t instruction)
{
  uint16_t type = instruction & 0xF000;

  switch (type) {
  case 0xD000: {
    // Dxyn - Display n-byte sprite starting at memory location I at (Vx, Vy),
    // set VF = collision.
    int x = (instruction & 0x0F00) >> 8;
    int y = (instruction & 0x00F0) >> 4;
    int n = (instruction & 0x000F);

    uint8_t *sprite = malloc(sizeof(uint8_t) * n);

    for (int i = 0; i < n; i++) {
      sprite[i] = read_memory_byte(cpu->memory, cpu->I + i);
    }

    draw_sprite(cpu->display, sprite, n, cpu->V[x], cpu->V[y]);
    render_screen(cpu->display);

    free(sprite);
    break;
  }
  case 0xA000: {
    // Annn - The value of register I is set to nnn.
    uint16_t value = instruction & 0x0FFF;
    cpu->I = value;
    break;
  }
  case 0x6000: {
    // 6xkk - The interpreter puts the value kk into register Vx.
    int x = (instruction & 0x0F00) >> 8;
    uint8_t value = (instruction & 0x00FF);
    cpu->V[x] = value;
    break;
  }
  }
}