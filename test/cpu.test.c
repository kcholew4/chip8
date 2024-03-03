#include "cpu.h"
#include <criterion/criterion.h>

void setup()
{
  memory_init();
  cpu_create();
}

void teardown()
{
  cpu_destroy();
}

// Helper function to set a specific register for pre-test conditions
void set_register(uint8_t reg, uint8_t value)
{
  cpu->V[reg] = value;
}

void fetch_and_execute(uint16_t opcode)
{
  cpu_fetch(); // Increases PC by 2
  cpu_execute(opcode);
}

Test(cpu_opcode, SYS, .init = setup, .fini = teardown)
{
  fetch_and_execute(0x0FFF);
  cr_assert(cpu->PC == 0x0FFF, "PC should be 0x0FFF");

  fetch_and_execute(0x0000);
  cr_assert(cpu->PC == 0x0000, "PC should be 0x0000");

  memory_write(0x0000, 0x0A);
  memory_write(0x0001, 0xBC);
  memory_write(0x0ABC, 0x0D);
  memory_write(0x0ABD, 0xAD);
  cpu_execute(cpu_fetch());
  cr_assert(cpu->PC == 0x0ABC, "PC should be 0x0ABC");
  cpu_execute(cpu_fetch());
  cr_assert(cpu->PC == 0x0DAD, "PC should be 0x0DAD");
}

Test(cpu_opcode, CLS, .init = setup, .fini = teardown)
{
  display_init();
  uint8_t sprite[] = {0xFF};
  display_draw(sprite, 1, 0, 0);
  fetch_and_execute(0x00E0);
  cr_assert(display->pixels[0][0] == 0x0000, "Display should be cleared");
}

Test(cpu_opcode, RET, .init = setup, .fini = teardown)
{
  // Assuming there's a way to simulate CALL to set up the stack
  uint16_t return_addr = 0x300;
  cpu->stack[0] = return_addr;
  cpu->SP = 1;
  fetch_and_execute(0x00EE); // Opcode for RET
  cr_assert(cpu->PC == return_addr,
            "PC should be set to the address on the top of the stack");
}

Test(opcode, JP_addr, .init = setup, .fini = teardown)
{
  fetch_and_execute(0x1000 | 0x0ABC);
  cr_assert(cpu->PC == 0x0ABC, "PC should be set to 0x0ABC");
}

Test(cpu_opcode, CALL_addr, .init = setup, .fini = teardown)
{
  uint16_t addr = 0x023A;
  fetch_and_execute(0x2000 | addr);
  cr_assert(cpu->PC == addr,
            "PC should be set to the address of the subroutine");
}

Test(cpu_opcode, SE_Vx_byte, .init = setup, .fini = teardown)
{
  set_register(0x1, 0xAB); // Set V1 = 0xAB
  cpu->PC = 0x200;
  fetch_and_execute(0x31AB); // SE V1, 0xAB
  cr_assert(cpu->PC == 0x204,
            "PC should be 0x204 to skip the next instruction if V1 == 0xAB");
}

Test(cpu_opcode, SNE_Vx_byte, .init = setup, .fini = teardown)
{
  set_register(0x1, 0xAB);
  cpu->PC = 0x200;
  fetch_and_execute(0x41AC); // SNE V1, 0xAC
  cr_assert(cpu->PC == 0x204,
            "PC should skip the next instruction if V1 != 0xAC");
}

Test(cpu_opcode, SE_Vx_Vy, .init = setup, .fini = teardown)
{
  set_register(0x1, 0xAB);
  set_register(0x2, 0xAB);
  cpu->PC = 0x200;
  fetch_and_execute(0x5120); // SE V1, V2
  cr_assert(cpu->PC == 0x204,
            "PC should skip the next instruction if V1 == V2");
}

Test(cpu_opcode, SNE_Vx_Vy, .init = setup, .fini = teardown)
{
  set_register(0x1, 0xAB);
  set_register(0x2, 0xAC);
  cpu->PC = 0x200;
  fetch_and_execute(0x9120); // SNE V1, V2
  cr_assert(cpu->PC == 0x204,
            "PC should skip the next instruction if V1 != V2");
}

Test(cpu_opcode, SE_Vx_byte_equals, .init = setup, .fini = teardown)
{
  set_register(0, 0xAB); // Set V0 = 0xAB
  cpu->PC = 0x200;
  fetch_and_execute(0x30AB); // SE V0, 0xAB
  cr_assert(cpu->PC == 0x204, "PC should skip next instruction if V0 == 0xAB");
}

Test(cpu_opcode, SNE_Vx_byte_not_equals, .init = setup, .fini = teardown)
{
  set_register(0, 0xAB);
  cpu->PC = 0x200;
  fetch_and_execute(0x40AC); // SNE V0, 0xAC
  cr_assert(cpu->PC == 0x204, "PC should skip next instruction if V0 != 0xAC");
}

Test(cpu_opcode, SE_Vx_Vy_equals, .init = setup, .fini = teardown)
{
  set_register(0, 0xAB);
  set_register(1, 0xAB);
  cpu->PC = 0x200;
  fetch_and_execute(0x5010); // SE V0, V1
  cr_assert(cpu->PC == 0x204, "PC should skip next instruction if V0 == V1");
}

Test(cpu_opcode, LD_Vx_byte, .init = setup, .fini = teardown)
{
  fetch_and_execute(0x60AB); // LD V0, 0xAB
  cr_assert(cpu->V[0] == 0xAB, "V0 should be set to 0xAB");
}

Test(cpu_opcode, ADD_Vx_byte, .init = setup, .fini = teardown)
{
  set_register(0x0, 0x01);
  fetch_and_execute(0x7001); // ADD V0, 0x01
  cr_assert(cpu->V[0] == 0x02, "V0 should be 0x02 after addition");
}

Test(cpu_opcode, OR_Vx_Vy, .init = setup, .fini = teardown)
{
  set_register(0, 0x01);
  set_register(1, 0x02);
  fetch_and_execute(0x8011); // OR V0, V1
  cr_assert(cpu->V[0] == (0x01 | 0x02), "V0 should be the result of V0 OR V1");
}

Test(cpu_opcode, AND_Vx_Vy, .init = setup, .fini = teardown)
{
  set_register(0, 0x03);
  set_register(1, 0x01);
  fetch_and_execute(0x8012);
}

Test(cpu_opcode, XOR_Vx_Vy, .init = setup, .fini = teardown)
{
  set_register(0, 0x01);
  set_register(1, 0x03);
  fetch_and_execute(0x8013); // XOR V0, V1
  cr_assert(cpu->V[0] == (0x01 ^ 0x03), "V0 should be the result of V0 XOR V1");
}

Test(cpu_opcode, ADD_Vx_Vy_with_carry, .init = setup, .fini = teardown)
{
  set_register(0, 0xFF);
  set_register(1, 0x02);
  fetch_and_execute(0x8014); // ADD V0, V1
  cr_assert(cpu->V[0] == 0x01 && cpu->V[0xF] == 1,
            "V0 should be 0x01 and VF should be set to 1 due to carry");
}

Test(cpu_opcode, SUB_Vx_Vy, .init = setup, .fini = teardown)
{
  set_register(0, 0x05);
  set_register(1, 0x03);
  fetch_and_execute(0x8015); // SUB V0, V1
  cr_assert(cpu->V[0] == 0x02 && cpu->V[0xF] == 1,
            "V0 should be 0x02 and VF should be set to 1 as there's no borrow");
}

Test(cpu_opcode, SHR_Vx, .init = setup, .fini = teardown)
{
  set_register(0, 0x02);
  fetch_and_execute(0x8016); // SHR V0
  cr_assert(cpu->V[0] == 0x01 && cpu->V[0xF] == 0,
            "V0 should be 0x01 after shift and VF should be 0");
}

Test(cpu_opcode, SUBN_Vx_Vy, .init = setup, .fini = teardown)
{
  set_register(0, 0x03);
  set_register(1, 0x05);
  fetch_and_execute(0x8017); // SUBN V0, V1
  cr_assert(cpu->V[0] == 0x02 && cpu->V[0xF] == 1,
            "V0 should be 0x02 and VF should be 1 as there's no borrow");
}

Test(cpu_opcode, SHL_Vx, .init = setup, .fini = teardown)
{
  set_register(0, 0x80);
  fetch_and_execute(0x801E); // SHL V0
  cr_assert(cpu->V[0] == 0x00, "V0 should be 0x00 by 2 after shift");
  cr_assert(cpu->V[0xF] == 1, "VF should be 1 due to loss of MSB");
}

// Skipping DxyN as it requires access to display

// Skipping keyboard access related opcodes (Ex9E and ExA1)

// Skipping sound and delay timer opcodes as they may require specific hardware
// or emulator functionality not covered in the provided tests

Test(cpu_opcode, LD_I_addr, .init = setup, .fini = teardown)
{
  fetch_and_execute(0xA123); // LD I, addr
  cr_assert(cpu->I == 0x0123, "I register should be set to 0x0123");
}

Test(cpu_opcode, JP_V0_addr, .init = setup, .fini = teardown)
{
  set_register(0, 0x01);     // Assuming V0 is used for the jump calculation
  fetch_and_execute(0xB123); // JP V0, addr
  cr_assert(cpu->PC == 0x0124, "PC should be set to the address plus V0");
}

Test(cpu_opcode, RND_Vx_byte, .init = setup, .fini = teardown)
{
  fetch_and_execute(0xC001); // RND V0, byte
  // This test is nondeterministic due to the random nature of the operation
  // Consider mocking the random function if deterministic behavior is desired
  cr_assert(cpu->V[0] <= 0x01, "V0 should be set to a random byte AND 0x01");
}

// Skipping Fx07, Fx0A, Fx15, Fx18 due to their dependency on timers and input

Test(cpu_opcode, ADD_I_Vx, .init = setup, .fini = teardown)
{
  cpu->I = 0x0001;
  set_register(0, 0x01);
  fetch_and_execute(0xF01E); // ADD I, Vx
  cr_assert(cpu->I == 0x0002, "I should be incremented by V0");
}

/* Memory operations */

Test(cpu_opcode, LD_F_Vx, .init = setup, .fini = teardown)
{
  set_register(0, 0x0F);     // Set V0 = 0x0F
  fetch_and_execute(0xF029); // LD F, V0
  cr_assert(
      cpu->I == 0x004B,
      "I register should be set to the location of the sprite for digit V0");
}

Test(cpu_opcode, LD_B_Vx, .init = setup, .fini = teardown)
{
  set_register(0, 123);      // Set V0 = 123
  fetch_and_execute(0xF033); // LD B, V0

  // Memory locations I, I+1, I+2 should contain the BCD representation of 123
  cr_assert(memory_read_byte(cpu->I) == 1,
            "Hundreds digit should be stored in memory at location I");
  cr_assert(memory_read_byte(cpu->I + 1) == 2,
            "Tens digit should be stored in memory at location I+1");
  cr_assert(memory_read_byte(cpu->I + 2) == 3,
            "Ones digit should be stored in memory at location I+2");
}

Test(cpu_opcode, LD_I_Vx, .init = setup, .fini = teardown)
{
  for (int i = 0; i < 8; i++) {
    set_register(i, i * 2); // Set V0-V7 = 0, 2, 4, 6, 8, 10, 12, 14
  }

  cpu->I = 0x300; // Set initial memory address

  fetch_and_execute(0xFF55); // LD [I], V7

  // Make sure values of V0-V7 are stored in memory starting at address I
  for (int i = 0; i < 8; i++) {
    cr_assert(memory_read_byte(cpu->I + i) == i * 2,
              "Value of V%d should be stored in memory at address I+%d", i, i);
  }
}

Test(cpu_opcode, LD_Vx_I, .init = setup, .fini = teardown)
{
  cpu->I = 0x400; // Set initial memory address

  for (int i = 0; i < 8; i++) {
    memory_write(cpu->I + i, i * 3); // Store values 0, 3, 6, 9, 12, 15, 18, 21
                                     // in memory starting at address I
  }

  fetch_and_execute(0xFF65); // LD V0, [I]

  // Make sure values are read from memory starting at address I into V0-V7
  for (int i = 0; i < 8; i++) {
    cr_assert(cpu->V[i] == i * 3,
              "Value at memory address I+%d should be stored in V%d", i, i);
  }
}

Test(cpu_opcode, SE_Vx_Vy_not_equal, .init = setup, .fini = teardown)
{
  set_register(1, 0x10); // Set V1
  set_register(2, 0x20); // Set V2
  uint16_t initialPC = cpu->PC;

  fetch_and_execute(0x5120); // Should not skip next instruction
  cr_assert(cpu->PC == initialPC + 2,
            "PC should increment by 2, not skipping the next instruction");
}

Test(cpu_opcode, SE_Vx_Vy_equal, .init = setup, .fini = teardown)
{
  set_register(1, 0x10); // Set V1
  set_register(2, 0x10); // Set V2
  uint16_t initialPC = cpu->PC;

  fetch_and_execute(0x5120); // Should skip next instruction
  cr_assert(cpu->PC == initialPC + 4,
            "PC should increment by 4, skipping the next instruction");
}

Test(cpu_opcode, LD_I_Vx_after, .init = setup, .fini = teardown)
{
  // Initially set registers V0-V7
  for (uint8_t i = 0; i <= 8; ++i) { set_register(i, i); }
  cpu->I = 0x300;
  fetch_and_execute(0xF855); // Note: Assuming F855 opcode for demonstration

  // Verify memory contents
  for (uint8_t i = 0; i <= 0x8; ++i) {
    cr_assert(memory_read_byte(cpu->I + i) == i,
              "Memory at location I+%d should hold value %d", i, i);
  }
#ifndef COWGOD_COMPATIBLE
  // Verify I register
  cr_assert(cpu->I == 0x300 + 0x8 + 1,
            "I register should correctly increment after operation");
#endif
}

Test(cpu_opcode, ADD_Vx_byte_overflow, .init = setup, .fini = teardown)
{
  set_register(0x0, 0xFF);   // Set V0 to its max value
  fetch_and_execute(0x7001); // ADD V0, 0x01, expecting overflow
  // Test directly checks V0 content
  cr_assert(cpu->V[0] == 0x00, "V0 should overflow and wrap to 0x00");
}