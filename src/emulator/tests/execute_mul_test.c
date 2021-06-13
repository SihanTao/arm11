#include "../../global_utils/types_and_macros.h"

#include "../execute/execute_helper.h"

#include "../../global_utils/tools.h"
#include "../utils/unit_test.h"
#include "../utils/init_arm_state.h"

int main(void)
{
  ArmState arm_state = init_state();

  add_test("Test for Mul execution");
  {
    // set_cond = true acc = false
    mul_t mul_ins1 = { .Rm = 1,
                       1001,
                       .Rs       = 2,
                       .Rn       = 3,
                       .Rd       = 4,
                       .set_cond = true,
                       .acc      = false,
                       0,
                       };

    arm_state->reg[1] =0xFFFFFFFD; // Rm
    arm_state->reg[2] =0xF;        // Rs
    arm_state->reg[4] =0;          // Rd

    execute_mul(mul_ins1, arm_state);

    test_int_v(arm_state->reg[4], 0xEFFFFFFD3,
               "mul 0xFFFFFFFFD 0xF 0, set_cond true acc false");

    // N is set to bit 31 of the result
    test_true(arm_state->neg == get_bit(0xFFFFFFD3, 31));

    // Z is set if and only if the result is zero.
    test_true(arm_state->zero == (arm_state->reg[4] == 0));

    free(arm_state);

    // set_cond = true acc = flase condition is set
    mul_t mul_ins2 = { .Rm = 1,
                       1001,
                       .Rs       = 2,
                       .Rn       = 3,
                       .Rd       = 4,
                       .set_cond = true,
                       .acc      = false,
                       01,
                       };

    arm_state         = init_state();
    arm_state->reg[1] =0xFFFFFFFD; // Rm
    arm_state->reg[2] =0xF;        // Rs
    arm_state->reg[4] =0;          // Rd

    execute_mul(mul_ins2, arm_state);

    test_int_v(arm_state->reg[4], 0xEFFFFFFD3,
               "mul 0xFFFFFFFFD 0xF 0, set_cond true acc false");

    // N is set to bit 31 of the result
    test_true(arm_state->neg == get_bit(0xFFFFFFD3, 31));

    // Z is set if and only if the result is zero.
    test_true(arm_state->zero == (arm_state->reg[4] == 0));

    free(arm_state);

    // set_cond = true acc = true
    mul_t mul_ins3 = { .Rm = 1,
                       1001,
                       .Rs       = 2,
                       .Rn       = 3,
                       .Rd       = 4,
                       .set_cond = true,
                       .acc      = true,
                       0,
                       };

    arm_state         = init_state();
    arm_state->reg[1] =1; // Rm
    arm_state->reg[2] =2; // Rs
    arm_state->reg[3] =3; // Rn
    arm_state->reg[4] =0; // Rd

    execute_mul(mul_ins3, arm_state);

    test_int_v(arm_state->reg[4], 0x00000005,
               "mul 1 2 3 0, set_cond true acc true");

    // N is set to bit 31 of the result
    test_true(arm_state->neg == get_bit(0x00000005, 31));

    // Z is set if and only if the result is zero.
    test_true(arm_state->zero == (arm_state->reg[4] == 0));

    free(arm_state);

    // set_cond = false acc = true
    mul_t mul_ins4 = { .Rm = 1,
                       1001,
                       .Rs       = 2,
                       .Rn       = 3,
                       .Rd       = 4,
                       .set_cond = false,
                       .acc      = true,
                       0,
                       };

    arm_state         = init_state();
    arm_state->reg[1] =1; // Rm
    arm_state->reg[2] =2; // Rs
    arm_state->reg[3] =3; // Rn
    arm_state->reg[4] =0; // Rd

    execute_mul(mul_ins4, arm_state);

    test_int_v(arm_state->reg[4], 0x00000005,
               "mul 1 2 3 0, set_cond false acc true");

    free(arm_state);

    // set_cond = false acc = flase
    mul_t mul_ins5 = { .Rm = 1,
                       1001,
                       .Rs       = 2,
                       .Rn       = 3,
                       .Rd       = 4,
                       .set_cond = false,
                       .acc      = false,
                       0,
                       };

    arm_state         = init_state();
    arm_state->reg[1] =0xFFFFFFFD; // Rm
    arm_state->reg[2] =0xF;        // Rs
    arm_state->reg[4] =0;          // Rd

    execute_mul(mul_ins5, arm_state);

    test_int_v(arm_state->reg[4], 0xEFFFFFFD3,
               "mul 0xFFFFFFFFD 0xF 0, set_cond false acc false");

    free(arm_state);
  }

  end_all_tests();
}
