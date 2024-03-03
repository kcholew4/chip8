#pragma once
#include "cpu.h"

void cpu_sys(CPU *cpu, uint16_t opcode);
void cpu_cls(CPU *cpu, uint16_t opcode);
void cpu_ret(CPU *cpu, uint16_t opcode);
void cpu_jp(CPU *cpu, uint16_t opcode);
void cpu_call(CPU *cpu, uint16_t opcode);
void cpu_se_vx(CPU *cpu, uint16_t opcode);
void cpu_sne_vx(CPU *cpu, uint16_t opcode);
void cpu_se_vx_vy(CPU *cpu, uint16_t opcode);
void cpu_ld_vx(CPU *cpu, uint16_t opcode);
void cpu_add_vx(CPU *cpu, uint16_t opcode);
void cpu_ld_vx_vy(CPU *cpu, uint16_t opcode);
void cpu_or_vx_vy(CPU *cpu, uint16_t opcode);
void cpu_and_vx_vy(CPU *cpu, uint16_t opcode);
void cpu_xor_vx_vy(CPU *cpu, uint16_t opcode);
void cpu_add_vx_vy(CPU *cpu, uint16_t opcode);
void cpu_sub_vx_vy(CPU *cpu, uint16_t opcode);
void cpu_shr_vx_vy(CPU *cpu, uint16_t opcode);
void cpu_subn_vx_vy(CPU *cpu, uint16_t opcode);
void cpu_shl_vx_vy(CPU *cpu, uint16_t opcode);
void cpu_sne_vx_vy(CPU *cpu, uint16_t opcode);
void cpu_ld_i(CPU *cpu, uint16_t opcode);
void cpu_jp_v0(CPU *cpu, uint16_t opcode);
void cpu_rnd_vx(CPU *cpu, uint16_t opcode);
void cpu_drw_vx_vy(CPU *cpu, uint16_t opcode);
void cpu_skp_vx(CPU *cpu, uint16_t opcode);
void cpu_sknp_vx(CPU *cpu, uint16_t opcode);
void cpu_ld_vx_dt(CPU *cpu, uint16_t opcode);
void cpu_ld_vx_k(CPU *cpu, uint16_t opcode);
void cpu_ld_dt_vx(CPU *cpu, uint16_t opcode);
void cpu_ld_st_vx(CPU *cpu, uint16_t opcode);
void cpu_add_i_vx(CPU *cpu, uint16_t opcode);
void cpu_ld_f_vx(CPU *cpu, uint16_t opcode);
void cpu_ld_b_vx(CPU *cpu, uint16_t opcode);
void cpu_ld_i_vx(CPU *cpu, uint16_t opcode);
void cpu_ld_vx_i(CPU *cpu, uint16_t opcode);