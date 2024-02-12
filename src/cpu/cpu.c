#include "cpu.h"
#include "dispatcher.h"
#include "instructions.h"

void _cpu_init()
{
  dispatcher_register(0x00E0, 0x0000, cpu_cls);        // 00E0 - CLS
  dispatcher_register(0x00EE, 0x0000, cpu_ret);        // 00EE - RET
  dispatcher_register(0x1000, 0x0FFF, cpu_jp);         // 1nnn - JP addr
  dispatcher_register(0x2000, 0x0FFF, cpu_call);       // 2nnn - CALL addr
  dispatcher_register(0x3000, 0x0FFF, cpu_se_vx);      // 3xkk - SE Vx, byte
  dispatcher_register(0x4000, 0x0FFF, cpu_sne_vx);     // 4xkk - SNE Vx, byte
  dispatcher_register(0x5000, 0x0FF0, cpu_se_vx_vy);   // 5xy0 - SE Vx, Vy
  dispatcher_register(0x6000, 0x0FFF, cpu_ld_vx);      // 6xkk - LD Vx, byte
  dispatcher_register(0x7000, 0x0FFF, cpu_add_vx);     // 7xkk - ADD Vx, byte
  dispatcher_register(0x8000, 0x0FF0, cpu_ld_vx_vy);   // 8xy0 - LD Vx, Vy
  dispatcher_register(0x8001, 0x0FF0, cpu_or_vx_vy);   // 8xy1 - OR Vx, Vy
  dispatcher_register(0x8002, 0x0FF0, cpu_and_vx_vy);  // 8xy2 - AND Vx, Vy
  dispatcher_register(0x8003, 0x0FF0, cpu_xor_vx_vy);  // 8xy3 - XOR Vx, Vy
  dispatcher_register(0x8004, 0x0FF0, cpu_add_vx_vy);  // 8xy4 - ADD Vx, Vy
  dispatcher_register(0x8005, 0x0FF0, cpu_sub_vx_vy);  // 8xy5 - SUB Vx, Vy
  dispatcher_register(0x8006, 0x0FF0, cpu_shr_vx_vy);  // 8xy6 - SHR Vx {, Vy}
  dispatcher_register(0x8007, 0x0FF0, cpu_subn_vx_vy); // 8xy7 - SUBN Vx, Vy
  dispatcher_register(0x800E, 0x0FF0, cpu_shl_vx_vy);  // 8xyE - SHL Vx {, Vy}
  dispatcher_register(0x9000, 0x0FF0, cpu_sne_vx_vy);  // 9xy0 - SNE Vx, Vy
  dispatcher_register(0xA000, 0x0FFF, cpu_ld_i);       // Annn - LD I, addr
  dispatcher_register(0xB000, 0x0FFF, cpu_jp_v0);      // Bnnn - JP V0, addr
  dispatcher_register(0xC000, 0x0FFF, cpu_rnd_vx);     // Cxkk - RND Vx, byte
  dispatcher_register(0xD000, 0x0FFF,
                      cpu_drw_vx_vy);               // Dxyn - DRW Vx, Vy, nibble
  dispatcher_register(0xE09E, 0x0F00, cpu_skp_vx);  // Ex9E - SKP Vx
  dispatcher_register(0xE0A1, 0x0F00, cpu_sknp_vx); // ExA1 - SKNP Vx
  dispatcher_register(0xF007, 0x0F00, cpu_ld_vx_dt); // Fx07 - LD Vx, DT
  dispatcher_register(0xF00A, 0x0F00, cpu_ld_vx_k);  // Fx0A - LD Vx, K
  dispatcher_register(0xF015, 0x0F00, cpu_ld_dt_vx); // Fx15 - LD DT, Vx
  dispatcher_register(0xF018, 0x0F00, cpu_ld_st_vx); // Fx18 - LD ST, Vx
  dispatcher_register(0xF01E, 0x0F00, cpu_add_i_vx); // Fx1E - ADD I, Vx
  dispatcher_register(0xF029, 0x0F00, cpu_ld_f_vx);  // Fx29 - LD F, Vx
  dispatcher_register(0xF033, 0x0F00, cpu_ld_b_vx);  // Fx33 - LD B, Vx
  dispatcher_register(0xF055, 0x0F00, cpu_ld_i_vx);  // Fx55 - LD [I], Vx
  dispatcher_register(0xF065, 0x0F00, cpu_ld_vx_i);  // Fx65 - LD Vx, [I]
}

CPU *cpu_create()
{
  CPU *cpu = calloc(1, sizeof(CPU));
  srand(time(NULL));
  _cpu_init();
  return cpu;
}

// MemoryController and DisplayController can be unset for testing purposes

void cpu_set_memory_controller(CPU *cpu, MemoryController *memory_controller)
{
  cpu->memory_controller = memory_controller;
}

void cpu_set_display_controller(CPU *cpu, DisplayController *display_controller)
{
  cpu->display_controller = display_controller;
}

uint16_t cpu_fetch(CPU *cpu)
{
  if (cpu->memory_controller == NULL) {
    cpu->PC += 2;
    return 0x0000;
  }

  uint16_t opcode = cpu->memory_controller->read_opcode(cpu->PC);
  cpu->PC += 2;
  return opcode;
}

void cpu_execute(CPU *cpu, uint16_t opcode)
{
  CpuInstruction *cpu_instruction = instructions_table[opcode];

  if (cpu_instruction == NULL) {
    printf("undefined instruction: %x\n", opcode);
    return;
  }

  cpu_instruction(cpu, opcode);
}