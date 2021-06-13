#include <stdio.h>
#include "../../global_utils/types_and_macros.h"

#include "../execute/execute_helper.h"

#include "../utils/init_arm_state.h"
#include "../../global_utils/tools.h"
#include "../utils/unit_test.h"

int main(void)
{
  ArmState arm_state = init_state();

  add_test("Test for data processing execution");
  {
    // set_cond = false S = false iFlag = false
    proc_t dp_ins1
        = { .operand2 = { .shift_reg = { .val = 1, .type = LSL, 0, .Rm = 1 } },
            .Rd       = 2,
            .Rn       = 3,
            .set_cond = false, // CPSR flags not update
            .opcode   = AND,
            .iFlag    = false, // op2 is a register
            0,
            };
    arm_state         = init_state();
    arm_state->reg[1] = 0x0000000B; // Rm = 0b1011
    arm_state->reg[2] = 0;          // Rd
    arm_state->reg[3] = 2;          // Rn = 0b10

    execute_proc(dp_ins1, arm_state);

    printf("arm_state->reg[2] :>> %p\n,", arm_state->reg[2]); // DELETE_MARK

    test_int_v(arm_state->reg[2],
               2, // 0b10
               "Rm = 1011, Rn = 10, 1011 becomes 10110, 10110 AND 10, so Rd = "
               "10, set_cond = false S = false iFlag = false");

    free(arm_state);

    // set_cond = true S = false iFlag = false
    proc_t dp_ins2
        = { .operand2 = { .shift_reg = { .val = 3, .type = LSR, 0, .Rm = 1 } },
            .Rd       = 2,
            .Rn       = 3,
            .set_cond = false, // CPSR flags not update
            .opcode   = SUB,
            .iFlag    = false, // op2 is a register
            0,
            };
    arm_state         = init_state();
    arm_state->reg[1] = 0xA000000B; // Rm = 1010..1011
    arm_state->reg[2] = 0;          // Rd
    arm_state->reg[3] = 0xF8000002; // Rn = 1111100..00010

    execute_proc(dp_ins2, arm_state);

    test_int_v(arm_state->reg[2],
               0xe4000001, // 1000...0001
               "Rm = 1010..1011, LSR becomes 0x14000001, "
               "F8000002 - 0x14000001, so Rd = e4000001, "
               "set_cond = true S = false iFlag = false\n");

    free(arm_state);

    // S = true iFlag = false
    proc_t dp_ins3
        = { .operand2 = { .shift_reg = { .val = 3, .type = ROR, 0, .Rm = 1 } },
            .Rd       = 2,
            .Rn       = 3,
            .set_cond = true, // CPSR flags update
            .opcode   = TST,
            .iFlag    = false, // op2 is a register
            0,
            };
    arm_state         = init_state();
    arm_state->reg[1] = 0x0000005D; // Rm = 1011101
    arm_state->reg[2] = 0xFFFFABCD; // Rd
    arm_state->reg[3] = 3;          // Rn = 11

    execute_proc(dp_ins3, arm_state);

    test_int_v(arm_state->reg[2],
               0xFFFFABCD, // result not written
               "Rm = 1011101, 1011101 becomes 1010..1011 "
               "1010..1011 TST 11, Rd = 0, S = true iFlag = false");

    // C is set to the carry out from any shift operation
    // C is set to 0 as the result is not written
    test_true(arm_state->carry);

    // N is set to bit 31 of the result
    // A000000B & 3 = 3 -> neg = false
    test_false(arm_state->neg);

    // Z is set if and only if the result is zero.
    test_false(arm_state->zero);

    free(arm_state);

    // S = false iFlag = true
    proc_t dp_ins4
        = { .operand2 = { .rot_imm.amount = 1,         // multiplied by 2 so 2
                          .rot_imm.imm    = 0x0000000B }, // 1011
            .Rd       = 1,
            .Rn       = 2,
            .set_cond = false, // CPSR flags not update
            .opcode   = ORR,
            .iFlag    = true, // op2 is an immediate constant
            0,
            };
    arm_state = init_state();

    arm_state->reg[1] = 0; // Rd
    arm_state->reg[2] = 3; // Rn = 11

    execute_proc(dp_ins4, arm_state);

    // rotated = C0000002
    // C0000002 | 3 = C0000003
    test_int_v(arm_state->reg[1],
               0xC0000003, // 1100...0011
               "1011 becomes 1100..0010, 1100...0010 ORR 11, so Rd = "
               "1100...0011, S = false iFlag = true");

    free(arm_state);

    // S = true iFlag = true
    proc_t dp_ins5
        = { .operand2 = { .rot_imm.amount = 2, // * 2, so amount is 4
                          .rot_imm.imm    = 0x0000001A },
            .Rd       = 1,
            .Rn       = 2,
            .set_cond = true, // CPSR flags update
            .opcode   = ADD,
            .iFlag    = true, // op2 is an immediate constant
            0,
            };
    arm_state = init_state();

    arm_state->reg[1] = 0; // Rd
    arm_state->reg[2] = 2; // Rn = 10

    execute_proc(dp_ins5, arm_state);

    test_int_v(arm_state->reg[1],
               0xA0000003, // 1010..0011
               "11010 becomes 1010..0001, 1010..0001 ADD 10, so Rd = "
               "1010..0011, S = true iFlag = true");

    // TODO : haven't done ALU carry functionality
    // // C is set to the carry out of the bit 31 of the ALU
    // // C is set to 0 as the addition does not produced a carry
    // test_true(arm_state->carry);

    // N is set to bit 31 of the result
    test_true(arm_state->neg == get_bit(arm_state->reg[1], 31));

    // Z is set if and only if the result is zero.
    test_true(arm_state->zero == (arm_state->reg[1] == 0));

    free(arm_state);
  }

  end_all_tests();
}
