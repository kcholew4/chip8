#include "cpu/cpu.h"
#include <criterion/criterion.h>

CPU *cpu = NULL;

void setup()
{
  cpu = cpu_create();
}

void teardown()
{
  free(cpu);
}

// Helper function to set a specific register for pre-test conditions
void set_register(uint8_t reg, uint8_t value)
{
  cpu->V[reg] = value;
}

void fetch_and_execute(CPU *cpu, uint16_t opcode)
{
  cpu_fetch(cpu); // Increases PC by 2
  cpu_execute(cpu, opcode);
}

Test(cpu_opcode, RET, .init = setup, .fini = teardown)
{
  // Assuming there's a way to simulate CALL to set up the stack
  uint16_t return_addr = 0x300;
  cpu->stack[0] = return_addr;
  cpu->SP = 1;
  fetch_and_execute(cpu, 0x00EE); // Opcode for RET
  cr_assert(cpu->PC == return_addr,
            "PC should be set to the address on the top of the stack");
}

Test(opcode, JP_addr, .init = setup, .fini = teardown)
{
  fetch_and_execute(cpu, 0x1000 | 0x0ABC);
  cr_assert(cpu->PC == 0x0ABC, "PC should be set to 0x0ABC");
}

Test(cpu_opcode, CALL_addr, .init = setup, .fini = teardown)
{
  uint16_t addr = 0x023A;
  fetch_and_execute(cpu, 0x2000 | addr);
  cr_assert(cpu->PC == addr,
            "PC should be set to the address of the subroutine");
}

Test(cpu_opcode, SE_Vx_byte, .init = setup, .fini = teardown)
{
  set_register(0x1, 0xAB); // Set V1 = 0xAB
  cpu->PC = 0x200;
  fetch_and_execute(cpu, 0x31AB); // SE V1, 0xAB
  cr_assert(cpu->PC == 0x204,
            "PC should be 0x204 to skip the next instruction if V1 == 0xAB");
}

Test(cpu_opcode, SNE_Vx_byte, .init = setup, .fini = teardown)
{
  set_register(0x1, 0xAB);
  cpu->PC = 0x200;
  fetch_and_execute(cpu, 0x41AC); // SNE V1, 0xAC
  cr_assert(cpu->PC == 0x204,
            "PC should skip the next instruction if V1 != 0xAC");
}

Test(cpu_opcode, SE_Vx_Vy, .init = setup, .fini = teardown)
{
  set_register(0x1, 0xAB);
  set_register(0x2, 0xAB);
  cpu->PC = 0x200;
  fetch_and_execute(cpu, 0x5120); // SE V1, V2
  cr_assert(cpu->PC == 0x204,
            "PC should skip the next instruction if V1 == V2");
}

Test(cpu_opcode, SNE_Vx_Vy, .init = setup, .fini = teardown)
{
  set_register(0x1, 0xAB);
  set_register(0x2, 0xAC);
  cpu->PC = 0x200;
  fetch_and_execute(cpu, 0x9120); // SNE V1, V2
  cr_assert(cpu->PC == 0x204,
            "PC should skip the next instruction if V1 != V2");
}

Test(cpu_opcode, SE_Vx_byte_equals, .init = setup, .fini = teardown)
{
  set_register(0, 0xAB); // Set V0 = 0xAB
  cpu->PC = 0x200;
  fetch_and_execute(cpu, 0x30AB); // SE V0, 0xAB
  cr_assert(cpu->PC == 0x204, "PC should skip next instruction if V0 == 0xAB");
}

Test(cpu_opcode, SNE_Vx_byte_not_equals, .init = setup, .fini = teardown)
{
  set_register(0, 0xAB);
  cpu->PC = 0x200;
  fetch_and_execute(cpu, 0x40AC); // SNE V0, 0xAC
  cr_assert(cpu->PC == 0x204, "PC should skip next instruction if V0 != 0xAC");
}

Test(cpu_opcode, SE_Vx_Vy_equals, .init = setup, .fini = teardown)
{
  set_register(0, 0xAB);
  set_register(1, 0xAB);
  cpu->PC = 0x200;
  fetch_and_execute(cpu, 0x5010); // SE V0, V1
  cr_assert(cpu->PC == 0x204, "PC should skip next instruction if V0 == V1");
}

Test(cpu_opcode, LD_Vx_byte, .init = setup, .fini = teardown)
{
  fetch_and_execute(cpu, 0x60AB); // LD V0, 0xAB
  cr_assert(cpu->V[0] == 0xAB, "V0 should be set to 0xAB");
}

Test(cpu_opcode, ADD_Vx_byte, .init = setup, .fini = teardown)
{
  set_register(0x0, 0x01);
  fetch_and_execute(cpu, 0x7001); // ADD V0, 0x01
  cr_assert(cpu->V[0] == 0x02, "V0 should be 0x02 after addition");
}

Test(cpu_opcode, OR_Vx_Vy, .init = setup, .fini = teardown)
{
  set_register(0, 0x01);
  set_register(1, 0x02);
  fetch_and_execute(cpu, 0x8011); // OR V0, V1
  cr_assert(cpu->V[0] == (0x01 | 0x02), "V0 should be the result of V0 OR V1");
}

Test(cpu_opcode, AND_Vx_Vy, .init = setup, .fini = teardown)
{
  set_register(0, 0x03);
  set_register(1, 0x01);
  fetch_and_execute(cpu, 0x8012); //
}

Test(cpu_opcode, XOR_Vx_Vy, .init = setup, .fini = teardown)
{
  set_register(0, 0x01);
  set_register(1, 0x03);
  fetch_and_execute(cpu, 0x8013); // XOR V0, V1
  cr_assert(cpu->V[0] == (0x01 ^ 0x03), "V0 should be the result of V0 XOR V1");
}

Test(cpu_opcode, ADD_Vx_Vy_with_carry, .init = setup, .fini = teardown)
{
  set_register(0, 0xFF);
  set_register(1, 0x02);
  fetch_and_execute(cpu, 0x8014); // ADD V0, V1
  cr_assert(cpu->V[0] == 0x01 && cpu->V[0xF] == 1,
            "V0 should be 0x01 and VF should be set to 1 due to carry");
}

Test(cpu_opcode, SUB_Vx_Vy, .init = setup, .fini = teardown)
{
  set_register(0, 0x05);
  set_register(1, 0x03);
  fetch_and_execute(cpu, 0x8015); // SUB V0, V1
  cr_assert(cpu->V[0] == 0x02 && cpu->V[0xF] == 1,
            "V0 should be 0x02 and VF should be set to 1 as there's no borrow");
}

Test(cpu_opcode, SHR_Vx, .init = setup, .fini = teardown)
{
  set_register(0, 0x02);
  fetch_and_execute(cpu, 0x8016); // SHR V0
  cr_assert(cpu->V[0] == 0x01 && cpu->V[0xF] == 0,
            "V0 should be 0x01 after shift and VF should be 0");
}

Test(cpu_opcode, SUBN_Vx_Vy, .init = setup, .fini = teardown)
{
  set_register(0, 0x03);
  set_register(1, 0x05);
  fetch_and_execute(cpu, 0x8017); // SUBN V0, V1
  cr_assert(cpu->V[0] == 0x02 && cpu->V[0xF] == 1,
            "V0 should be 0x02 and VF should be 1 as there's no borrow");
}

Test(cpu_opcode, SHL_Vx, .init = setup, .fini = teardown)
{
  set_register(0, 0x80);
  fetch_and_execute(cpu, 0x801E); // SHL V0
  cr_assert(cpu->V[0] == 0x00, "V0 should be 0x00 by 2 after shift");
  cr_assert(cpu->V[0xF] == 1, "VF should be 1 due to loss of MSB");
}

// Skipping DxyN as it requires access to display

// Skipping keyboard access related opcodes (Ex9E and ExA1)

// Skipping sound and delay timer opcodes as they may require specific hardware
// or emulator functionality not covered in the provided tests

Test(cpu_opcode, LD_I_addr, .init = setup, .fini = teardown)
{
  fetch_and_execute(cpu, 0xA123); // LD I, addr
  cr_assert(cpu->I == 0x0123, "I register should be set to 0x0123");
}

Test(cpu_opcode, JP_V0_addr, .init = setup, .fini = teardown)
{
  set_register(0, 0x01); // Assuming V0 is used for the jump calculation
  fetch_and_execute(cpu, 0xB123); // JP V0, addr
  cr_assert(cpu->PC == 0x0124, "PC should be set to the address plus V0");
}

Test(cpu_opcode, RND_Vx_byte, .init = setup, .fini = teardown)
{
  fetch_and_execute(cpu, 0xC001); // RND V0, byte
  // This test is nondeterministic due to the random nature of the operation
  // Consider mocking the random function if deterministic behavior is desired
  cr_assert(cpu->V[0] <= 0x01, "V0 should be set to a random byte AND 0x01");
}

// Skipping Fx07, Fx0A, Fx15, Fx18 due to their dependency on timers and input

Test(cpu_opcode, ADD_I_Vx, .init = setup, .fini = teardown)
{
  cpu->I = 0x0001;
  set_register(0, 0x01);
  fetch_and_execute(cpu, 0xF01E); // ADD I, Vx
  cr_assert(cpu->I == 0x0002, "I should be incremented by V0");
}

// Skipping Fx29, Fx33, Fx55, Fx65 as they involve operations with memory
