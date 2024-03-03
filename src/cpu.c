#include "cpu.h"
#include "instructions.h"
#include "opcodes.h"

CPU *cpu;

void cpu_init()
{
  opcode_register("0nnn", cpu_sys, "SYS nnn");
  opcode_register("00E0", cpu_cls, "CLS");
  opcode_register("00EE", cpu_ret, "RET");
  opcode_register("1nnn", cpu_jp, "JP nnn");
  opcode_register("2nnn", cpu_call, "CALL nnn");
  opcode_register("3xkk", cpu_se_vx, "SE Vx, kk");
  opcode_register("4xkk", cpu_sne_vx, "SNE Vx, kk");
  opcode_register("5xy0", cpu_se_vx_vy, "SE Vx, Vy");
  opcode_register("6xkk", cpu_ld_vx, "LD Vx, kk");
  opcode_register("7xkk", cpu_add_vx, "ADD Vx, kk");
  opcode_register("8xy0", cpu_ld_vx_vy, "LD Vx, Vy");
  opcode_register("8xy1", cpu_or_vx_vy, "OR Vx, Vy");
  opcode_register("8xy2", cpu_and_vx_vy, "AND Vx, Vy");
  opcode_register("8xy3", cpu_xor_vx_vy, "XOR Vx, Vy");
  opcode_register("8xy4", cpu_add_vx_vy, "ADD Vx, Vy");
  opcode_register("8xy5", cpu_sub_vx_vy, "SUB Vx, Vy");
  opcode_register("8xy6", cpu_shr_vx_vy, "SHR Vx {, Vy}");
  opcode_register("8xy7", cpu_subn_vx_vy, "SUBN Vx, Vy");
  opcode_register("8xyE", cpu_shl_vx_vy, "SHL Vx {, Vy}");
  opcode_register("9xy0", cpu_sne_vx_vy, "SNE Vx, Vy");
  opcode_register("Annn", cpu_ld_i, "LD I, nnn");
  opcode_register("Bnnn", cpu_jp_v0, "JP V0, nnn");
  opcode_register("Cxkk", cpu_rnd_vx, "RND Vx, kk");
  opcode_register("Dxyn", cpu_drw_vx_vy, "DRW Vx, Vy, n");
  opcode_register("Ex9E", cpu_skp_vx, "SKP Vx");
  opcode_register("ExA1", cpu_sknp_vx, "SKNP Vx");
  opcode_register("Fx07", cpu_ld_vx_dt, "LD Vx, DT");
  opcode_register("Fx0A", cpu_ld_vx_k, "LD Vx, K");
  opcode_register("Fx15", cpu_ld_dt_vx, "LD DT, Vx");
  opcode_register("Fx18", cpu_ld_st_vx, "LD ST, Vx");
  opcode_register("Fx1E", cpu_add_i_vx, "ADD I, Vx");
  opcode_register("Fx29", cpu_ld_f_vx, "LD F, Vx");
  opcode_register("Fx33", cpu_ld_b_vx, "LD B, Vx");
  opcode_register("Fx55", cpu_ld_i_vx, "LD [I], Vx");
  opcode_register("Fx65", cpu_ld_vx_i, "LD Vx, [I]");
}

void cpu_create()
{
  cpu = calloc(1, sizeof(CPU));
  cpu->PC = 0x200;
  srand(time(NULL));
  cpu_init();
}

void cpu_destroy()
{
  free(cpu);
}

uint16_t cpu_fetch()
{
  uint16_t opcode = memory_read_opcode(cpu->PC);
  cpu->PC += 2;
  return opcode;
}

uint64_t get_timestamp()
{
  struct timespec ts;
  timespec_get(&ts, TIME_UTC);
  return ts.tv_sec * 1000LL + ts.tv_nsec / (1000LL * 1000LL);
}

void check_timers()
{
  uint64_t timestamp = get_timestamp();
  const int duration = 1000 / 60;
  if (cpu->DT > 0 && cpu->dt_updated + duration <= timestamp) {
    cpu->dt_updated = timestamp;
    cpu->DT--;
  }
  if (cpu->ST > 0) {
    // Play sound here...
    if (cpu->st_updated + duration <= timestamp) { cpu->ST--; }
  }
}

void cpu_execute(uint16_t opcode)
{
  Opcode *opcode_info = get_opcode(opcode);

  if (opcode_info->instruction == NULL) {
    printf("undefined instruction: %x\n", opcode);
    return;
  }

  // printf("%x: %s | PC: %04x -> %04x, SP: %04x, I: %04x\n", opcode,
  //        opcode_info->disassembled, cpu->PC, memory_read_opcode(cpu->PC),
  //        cpu->SP, cpu->I);

  opcode_info->instruction(cpu, opcode);

  // printf("PC: %04x -> %04x, SP: %04x, I: %04x\n", cpu->PC,
  //        memory_read_opcode(cpu->PC), cpu->SP, cpu->I);
}

void cpu_cycle()
{
  if (cpu->key_wait) {
    if (cpu->key != cpu->key_v) { return; }
    cpu->key_wait = false;
  }

  check_timers();
  cpu_execute(cpu_fetch());
}
