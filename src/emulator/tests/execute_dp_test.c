#include "../utils/types_and_macros.h"

#include "../execute/execute.h"

#include "../utils/tools.h"
#include "../utils/unit_test.h"

int main(void)
{
  ArmState arm_state = init_state_for_test();

  add_test("Test for data processing execution");
  {
    // set_cond = false S = false is_imm = false
    proc_t dp_ins1
        = { .operand2 = { .shift_reg = { .Rm = 1, 0, .type = LSL, .val = 1 } },
            .Rd       = 2,
            .Rn       = 3,
            .set_cond = false, // CPSR flags not update
            .opcode   = AND,
            .is_imm   = false, // op2 is a register
            0,
            .cond = 0 };
    arm_state         = init_state_for_test();
    arm_state->reg[1] = to_bf(0x0000000B); // Rm = 1011
    arm_state->reg[2] = to_bf(0);          // Rd
    arm_state->reg[3] = to_bf(2);          // Rn = 10

    execute_DP(dp_ins1, arm_state);

    test_int_v(to_int(arm_state->reg[2]),
               2, // 10
               "Rm = 1011, Rn = 10, 1011 becomes 10110, 10110 AND 10, so Rd = "
               "10, set_cond = false S = false is_imm = false");

    free(arm_state);

    // set_cond = true S = false is_imm = false
    proc_t dp_ins2
        = { .operand2 = { .shift_reg = { .Rm = 1, 0, .type = ASR, .val = 3 } },
            .Rd       = 2,
            .Rn       = 3,
            .set_cond = false, // CPSR flags not update
            .opcode   = SUB,
            .is_imm   = false, // op2 is a register
            0,
            .cond = 1 };
    arm_state         = init_state_for_test();
    arm_state->reg[1] = to_bf(0xA000000B); // Rm = 1010..1011
    arm_state->reg[2] = to_bf(0);          // Rd
    arm_state->reg[3] = to_bf(0xF8000002); // Rn = 1111100..0000

    execute_DP(dp_ins2, arm_state);

    test_int_v(to_int(arm_state->reg[2]),
               0x4000001, // 1000...0001
               "Rm = 1010..1011, Rn = 10, 1010..1011 becomes 1111010..0001, "
               "1111100..0000 SUB 1111010..0001, so Rd = 1000...0001, "
               "set_cond = true S = false is_imm = false");

    free(arm_state);

    // S = true is_imm = false
    proc_t dp_ins3
        = { .operand2 = { .shift_reg = { .Rm = 1, 0, .type = ROR, .val = 3 } },
            .Rd       = 2,
            .Rn       = 3,
            .set_cond = true, // CPSR flags update
            .opcode   = TST,
            .is_imm   = false, // op2 is a register
            0,
            .cond = 0 };
    arm_state         = init_state_for_test();
    arm_state->reg[1] = to_bf(0x0000005D); // Rm = 1011101
    arm_state->reg[2] = to_bf(0);          // Rd
    arm_state->reg[3] = to_bf(3);          // Rn = 11

    execute_DP(dp_ins3, arm_state);

    test_int_v(to_int(arm_state->reg[2]),
               0, // result not written
               "Rm = 1011101, Rn = 10, 1011101 becomes 1010..1011, 1010..1011 "
               "AND 11, Rd = 0, S = true is_imm = false");

    // C is set to the carry out from any shift operation
    // C is set to 0 as the result is not written
    test_true(arm_state->carry == 0);

    // N is set to bit 31 of the result
    test_true(arm_state->neg == get_bit(0, 31));

    // Z is set if and only if the result is zero.
    test_true(arm_state->zero == (to_int(arm_state->reg[2]) == 0));

    free(arm_state);

    // S = false is_imm = true
    proc_t dp_ins4 = { .operand2 = { .rot_imm.imm    = 0x0000000B, // 1011
                                     .rot_imm.amount = 1 },
                       .Rd       = 1,
                       .Rn       = 2,
                       .set_cond = false, // CPSR flags not update
                       .opcode   = ORR,
                       .is_imm   = true, // op2 is an immediate constant
                       0,
                       .cond = 0 };
    arm_state      = init_state_for_test();

    arm_state->reg[1] = to_bf(0); // Rd
    arm_state->reg[2] = to_bf(3); // Rn = 11

    execute_DP(dp_ins4, arm_state);

    test_int_v(to_int(arm_state->reg[1]),
               0x6000003, // 1100...0011
               "1011 becomes 1100..0010, 1100...0010 ORR 11, so Rd = "
               "1100...0011, S = false is_imm = true");

    free(arm_state);

    // S = true is_imm = true
    proc_t dp_ins5 = { .operand2 = { .rot_imm.imm    = 0x0000001A, // 11010
                                     .rot_imm.amount = 2 },
                       .Rd       = 1,
                       .Rn       = 2,
                       .set_cond = true, // CPSR flags update
                       .opcode   = ADD,
                       .is_imm   = true, // op2 is an immediate constant
                       0,
                       .cond = 0 };
    arm_state      = init_state_for_test();

    arm_state->reg[1] = to_bf(0); // Rd
    arm_state->reg[2] = to_bf(2); // Rn = 10

    execute_DP(dp_ins5, arm_state);

    test_int_v(to_int(arm_state->reg[1]),
               0x5000003, // 1010..0011
               "11010 becomes 1010..0001, 1010..0001 ADD 10, so Rd = "
               "1010..0011, S = true is_imm = true");

    // C is set to the carry out of the bit 31 of the ALU
    // C is set to 0 as the addition does not produced a carry
    test_true(arm_state->carry == 0);

    // N is set to bit 31 of the result
    test_true(arm_state->neg == get_bit(0x5000003, 31));

    // Z is set if and only if the result is zero.
    test_true(arm_state->zero == (to_int(arm_state->reg[1]) == 0));

    free(arm_state);
  }
}
