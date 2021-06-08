#include <stdlib.h>
#include <math.h>
#include "../utils/types_and_macros.h"
#include "../execute/execute_helper.h"
#include "../execute/execute_SDT.h"
#include "../execute/execute_DP.h"
#include "../execute/execute.h"
#include "../utils/tools.h"
#include "../utils/unit_test.h"

static ArmState init_state();

int main(void)
{
  ArmState arm_state = init_state();

  add_test("Test for rotate");
  {
      test_int_v(rotate(1, 0x0000000B), 0x80000005, "1011 becomes 1000..0101");

      test_int_v(rotate(2, 0x0000001A), 0x40000006, "11010 becomes 0100..0110");

      test_int_v(rotate(3, 0x0000005D), 0xA000000B, "1011101 becomes 1010..1011");
  }

  add_test("Test for shift");
  {
      test_int_v(shift(0x0000000B, 1, LSL), 0x00000016, "1011 becomes 10110");

      test_int_v(shift(0x0000001A, 2, LSR), 0x00000006, "11010 becomes 110");

      test_int_v(shift(0xA000000B, 3, ASR), 0xF4000001, "1010..1011 becomes 1111010..0001");

      test_int_v(shift(0x0000005D, 3, ROR), 0xA000000B, "1011101 becomes 1010..1011");
  }

  add_test("Test for arith_right");
  {
      test_int_v(arith_right(1, 0x0000001A), 0x0000000D, "..11010 becomes ..1101");

      test_int_v(arith_right(2, 0x80000005), 0xE0000001, "1000..0101 becomes 1110..0001");

      test_int_v(arith_right(3, 0xA000000B), 0xF4000001, "1010..1011 becomes 1111010..0001");
  }

  add_test("Test for reg_imm_handle");
  {
      bitfield reg_imm_bf[1];
      reg_or_imm_t imm1 = 
      {.rot_imm.imm = 0x0000000B,
       .rot_imm.amount = 1};

      test_int_v(reg_imm_handle(reg_imm_bf, imm1, 0), 0xC0000002, "1011 becomes 1100..0010");

      bitfield reg_imm_bf[1];
      reg_or_imm_t imm2 = 
      {.rot_imm.imm = 0x0000001A,
       .rot_imm.amount = 2};

      test_int_v(reg_imm_handle(reg_imm_bf, imm2, 0), 0xA0000001, "11010 becomes 1010..0001");
 
      reg_or_imm_t reg1 = 
      {.shift_reg = {
        .Rm = 1,
        0,
        .type = LSL,
        .val = 1}};
      
      bitfield reg_imm_bf[1] = to_bf(0x0000000B);//Rm
      test_int_v(reg_imm_handle(reg_imm_bf, reg1, 1), 0x00000016, "1011 becomes 10110");

      reg_or_imm_t reg2 = 
      {.shift_reg = {
        .Rm = 1,
        0,
        .type = LSR,
        .val = 2}};
      
      bitfield reg_imm_bf[1] = to_bf(0x0000001A);//Rm
      test_int_v(reg_imm_handle(reg_imm_bf, reg2, 1), 0x00000006, "11010 becomes 110");

      reg_or_imm_t reg3 = 
      {.shift_reg = {
        .Rm = 1,
        0,
        .type = ASR,
        .val = 3}};
      
      bitfield reg_imm_bf[1] = to_bf(0xA000000B);//Rm
      test_int_v(reg_imm_handle(reg_imm_bf, reg3, 1), 0xF4000001, "1010..1011 becomes 1111010..0001");

      reg_or_imm_t reg4 = 
      {.shift_reg = {
        .Rm = 1,
        0,
        .type = ROR,
        .val = 3}};
      
      bitfield reg_imm_bf[1] = to_bf(0x0000005D);//Rm
      test_int_v(reg_imm_handle(reg_imm_bf, reg4, 1), 0xA000000B, "1011101 becomes 1010..1011");
  }

  add_test("Test for data processing execution");
  {
    proc_t dp_ins1 =
    {
      .operand2 = 
      {
        .shift_reg = {
          .Rm = 1,
          .type = 2,
          .val = 3
        }
      },
      .Rd = 2,
      .Rn = 3,
      .S = false,//CPSR flags not update
      .opcode = 4,
      .is_imm = false,//op2 is a register
      0,
      .cond = 0
      };
    arm_state = init_state();
    arm_state->reg[1] = to_bf(0xFFFFFFFD); // Rm
    arm_state->reg[2] = to_bf(0xF);        // Rd
    arm_state->reg[3] = to_bf(0);          // Rn

    execute_DP(dp_ins1, arm_state);

    test_int_v(
        to_int(arm_state->reg[2]),
        0xEFFFFFFD3,
        "mul 0xFFFFFFFFD 0xF 0, set_cond false is_imm false");

    free(arm_state);

    //condition is set
    proc_t dp_ins2 =
    {
      .operand2 = 
      {
        .shift_reg = {
          .Rm = 1,
          .type = 2,
          .val = 3
        }
      },
      .Rd = 2,
      .Rn = 3,
      .S = false,//CPSR flags not update
      .opcode = 4,
      .is_imm = false,//op2 is a register
      0,
      .cond = 1
    };
    arm_state = init_state();
    arm_state->reg[1] = to_bf(0xFFFFFFFD); // op2
    arm_state->reg[2] = to_bf(0xF);        // Rd
    arm_state->reg[3] = to_bf(0);          // Rn

    execute_DP(dp_ins2, arm_state);

    test_int_v(
        to_int(arm_state->reg[2]),
        0xEFFFFFFD3,
        "mul 0xFFFFFFFFD 0xF 0, set_cond false is_imm false");

    free(arm_state);

    //set_cond = true is_imm = false
    proc_t dp_ins3 =
    {
      .operand2 = 
      {
        .shift_reg = {
          .Rm = 1,
          .type = 2,
          .val = 3
        }
      },
      .Rd = 2,
      .Rn = 3,
      .S = true, //CPSR flags update
      .opcode = 4,
      .is_imm = false,//op2 is a register
      0,
      .cond = 0
    };
    arm_state = init_state();
    arm_state->reg[1] = to_bf(0xFFFFFFFD); // op2
    arm_state->reg[2] = to_bf(0xF);        // Rd
    arm_state->reg[3] = to_bf(0);          // Rn

    execute_DP(dp_ins3, arm_state);

    test_int_v(
        to_int(arm_state->reg[2]),
        0xEFFFFFFD3,
        "mul 0xFFFFFFFFD 0xF 0, set_cond true is_imm false");

    // C is set to the carry out from any shift operation
    //test_true(arm_state->carry == );

    // N is set to bit 31 of the result
    test_true(arm_state->neg == get_bit(0xFFFFFFD3, 31));

    // Z is set if and only if the result is zero.
    test_true(arm_state->zero == (to_int(arm_state->reg[2]) == 0));

    free(arm_state);


    //set_cond = false is_imm = true
    proc_t dp_ins4 =
    {
      .operand2 = 
      {
        .rot_imm.imm = 1,
        .rot_imm.amount = 2
      },
      .Rd = 2,
      .Rn = 3,
      .S = false, //CPSR flags not update
      .opcode = 4,
      .is_imm = true, //op2 is an immediate constant
      0,
      .cond = 0
    };
    arm_state = init_state();

    arm_state->reg[2] = to_bf(0xF);        // Rd
    arm_state->reg[3] = to_bf(0);          // Rn

    execute_DP(dp_ins4, arm_state);

    test_int_v(
        to_int(arm_state->reg[2]),
        0xEFFFFFFD3,
        "mul 0xFFFFFFFFD 0xF 0, set_cond false is_imm true");

    free(arm_state);

    //set_cond = true is_imm = true
    proc_t dp_ins5 =
    {
      .operand2 = 
      {
        .rot_imm.imm = 1,
        .rot_imm.amount = 2
      },
      .Rd = 2,
      .Rn = 3,
      .S = true, //CPSR flags update
      .opcode = 4,
      .is_imm = true, //op2 is an immediate constant
      0,
      .cond = 0
    };
    arm_state = init_state();

    arm_state->reg[2] = to_bf(0xF);        // Rd
    arm_state->reg[3] = to_bf(0);          // Rn

    execute_DP(dp_ins5, arm_state);

    test_int_v(
        to_int(arm_state->reg[2]),
        0xEFFFFFFD3,
        "mul 0xFFFFFFFFD 0xF 0, set_cond true is_imm true");

    // C is set to the carry out from any shift operation
    //test_true(arm_state->carry == );

    // N is set to bit 31 of the result
    test_true( arm_state->neg == get_bit(0xFFFFFFD3, 31));

    // Z is set if and only if the result is zero.
    test_true(arm_state->zero == (to_int(arm_state->reg[2]) == 0));

    free(arm_state);
  }
  

  add_test("Test for Mul execution");
  {
    // set_cond = true acc = false
    mul_t mul_ins1 =
    {
      .Rm = 1,
      1001,
      .Rs = 2,
      .Rn = 3,
      .Rd = 4,
      .set_cond = true,
      .acc = false,
      0,
      .cond = 0
    };

    arm_state->reg[1] = to_bf(0xFFFFFFFD); // Rm
    arm_state->reg[2] = to_bf(0xF);        // Rs
    arm_state->reg[4] = to_bf(0);          // Rd

    execute_MUL(mul_ins1, arm_state);

    test_int_v(
        to_int(arm_state->reg[4]),
        0xEFFFFFFD3,
        "mul 0xFFFFFFFFD 0xF 0, set_cond true acc false");

    // N is set to bit 31 of the result
    test_true( arm_state->neg == get_bit(0xFFFFFFD3, 31));

    // Z is set if and only if the result is zero.
    test_true(arm_state->zero == (to_int(arm_state->reg[4]) == 0));

    free(arm_state);

    // set_cond = true acc = flase condition is set
    mul_t mul_ins2 =
    {     
      .Rm = 1,
      1001,
      .Rs = 2,
      .Rn = 3,
      .Rd = 4,
      .set_cond = true,
      .acc = false,
      01,
      .cond = 1
    };

    arm_state = init_state();
    arm_state->reg[1] = to_bf(0xFFFFFFFD); // Rm
    arm_state->reg[2] = to_bf(0xF);        // Rs
    arm_state->reg[4] = to_bf(0);          // Rd

    execute_MUL(mul_ins2, arm_state);

    test_int_v(
        to_int(arm_state->reg[4]),
        0xEFFFFFFD3,
        "mul 0xFFFFFFFFD 0xF 0, set_cond true acc false");

    // N is set to bit 31 of the result
    test_true( arm_state->neg == get_bit(0xFFFFFFD3, 31));

    // Z is set if and only if the result is zero.
    test_true(arm_state->zero == (to_int(arm_state->reg[4]) == 0));

    free(arm_state);

    // set_cond = true acc = true
    mul_t mul_ins3 =
    {         
      .Rm = 1,
      1001,
      .Rs = 2,
      .Rn = 3,
      .Rd = 4,
      .set_cond = true,
      .acc = true,
      0,
      .cond = 0
    };

    arm_state = init_state();
    arm_state->reg[1] = to_bf(1); // Rm
    arm_state->reg[2] = to_bf(2); // Rs
    arm_state->reg[3] = to_bf(3); // Rn
    arm_state->reg[4] = to_bf(0); // Rd

    execute_MUL(mul_ins3, arm_state);

    test_int_v(
        to_int(arm_state->reg[4]),
        0x00000005,
        "mul 1 2 3 0, set_cond true acc true");

    // N is set to bit 31 of the result
    test_true( arm_state->neg == get_bit(0x00000005, 31));

    // Z is set if and only if the result is zero.
    test_true(arm_state->zero == (to_int(arm_state->reg[4]) == 0));

    free(arm_state);

    // set_cond = false acc = true
    mul_t mul_ins4 =
    {
      .Rm = 1,
      1001,
      .Rs = 2,
      .Rn = 3,
      .Rd = 4,
      .set_cond = false,
      .acc = true,
      0,
      .cond = 0
    };

    arm_state = init_state();
    arm_state->reg[1] = to_bf(1); // Rm
    arm_state->reg[2] = to_bf(2); // Rs
    arm_state->reg[3] = to_bf(3); // Rn
    arm_state->reg[4] = to_bf(0); // Rd

    execute_MUL(mul_ins4, arm_state);

    test_int_v(
        to_int(arm_state->reg[4]),
        0x00000005,
        "mul 1 2 3 0, set_cond false acc true");

    free(arm_state);

    // set_cond = false acc = flase
    mul_t mul_ins5 =
    {
      .Rm = 1,
      1001,
      .Rs = 2,
      .Rn = 3,
      .Rd = 4,
      .set_cond = false,
      .acc = false,
      0,
      .cond = 0
    };

    arm_state = init_state();
    arm_state->reg[1] = to_bf(0xFFFFFFFD); // Rm
    arm_state->reg[2] = to_bf(0xF);        // Rs
    arm_state->reg[4] = to_bf(0);          // Rd

    execute_MUL(mul_ins5, arm_state);

    test_int_v(
        to_int(arm_state->reg[4]),
        0xEFFFFFFD3,
        "mul 0xFFFFFFFFD 0xF 0, set_cond false acc false");

    free(arm_state);
   }

  add_test("Test for Trans execution");
  {
    /* STR R0, [R1, #12]
     * LDR R0, [R1, #12]
     * STR R0, [R1], #12
     * LDR R0, [R1], #12
     */

    trans_t trans_ins1 =
    {
      .offset = 
      {
        .rot_imm.imm = 1,
        .rot_imm.amount = 2
      },
      .Rd = 2,
      .Rn = 3,
      .is_load = false,//str
      0,
      .is_up = false,//sub
      .is_pre = false,//post
      .is_imm = false,//is_imm
      01,
      .cond = 0
    };
    arm_state = init_state();
    arm_state->reg[1] = to_bf(2); // offset, 0000 0000 0010
    arm_state->reg[2] = to_bf(0); // Rd
    arm_state->reg[3] = to_bf(1); // Rn,

    execute_TRANS(trans_ins1, arm_state);

    test_int_v(
        to_int(arm_state->reg[2]),
        1,
        "STR R0, [R1], #12, all false");

    free(arm_state);

    //condition is set
    trans_t trans_ins2 =
    {
      .offset = 
      {
        .rot_imm.imm = 1,
        .rot_imm.amount = 2
      },
      .Rd = 2,
      .Rn = 3,
      .is_load = false,//store
      0,
      .is_up = false,//sub
      .is_pre = false,//post
      .is_imm = false,//is_imm
      01,
      .cond = 1
    };
    arm_state = init_state();
    arm_state->reg[1] = to_bf(1); // offset
    arm_state->reg[2] = to_bf(0); // Rd
    arm_state->reg[3] = to_bf(2); // Rn

    execute_TRANS(trans_ins2, arm_state);

    test_int_v(
        to_int(arm_state->reg[2]),
        1,
        "STR R0, [R1], #12, all false");

    free(arm_state);

    //is_load = true is_up = false is_pre = false is_imm = false
    trans_t trans_ins3 =
    {
      .offset = 
      {
        .rot_imm.imm = 1,
        .rot_imm.amount = 2
      },
      .Rd = 2,
      .Rn = 3,
      .is_load = true, //load
      0,
      .is_up = false,//sub
      .is_pre = false,//post
      .is_imm = false,//is_imm
      01,
      .cond = 0
    };
    arm_state = init_state();
    arm_state->reg[1] = to_bf(1); // offset
    arm_state->reg[2] = to_bf(0); // Rd
    arm_state->reg[3] = to_bf(2); // Rn

    execute_TRANS(trans_ins3, arm_state);

    test_int_v(
        to_int(arm_state->reg[2]),
        1,
        "LDR R0, [R1], #12, is_load = true is_up = false is_pre = false is_imm = false");

    free(arm_state);

    //is_load = false is_up = true is_pre = false is_imm = false
    trans_t trans_ins4 =
    {
      .offset = 
      {
        .rot_imm.imm = 1,
        .rot_imm.amount = 2
      },
      .Rd = 2,
      .Rn = 3,
      .is_load = false,//store
      0,
      .is_up = true, //add
      .is_pre = false,//post
      .is_imm = false,//is_imm
      01,
      .cond = 0
    };
    arm_state = init_state();
    arm_state->reg[1] = to_bf(1); // offset
    arm_state->reg[2] = to_bf(0); // Rd
    arm_state->reg[3] = to_bf(2); // Rn

    execute_TRANS(trans_ins4, arm_state);

    test_int_v(
        to_int(arm_state->reg[2]),
        1,
        "STR R0, [R1], #12, is_load = false is_up = true is_pre = false is_imm = false");

    free(arm_state);

    //is_load = false is_up = false is_pre = true is_imm = false
    trans_t trans_ins5 =
    {
      .offset = 
      {
        .rot_imm.imm = 1,
        .rot_imm.amount = 2
      },
      .Rd = 2,
      .Rn = 3,
      .is_load = false,//store
      0,
      .is_up = false,//sub
      .is_pre = true, //pre
      .is_imm = false,//is_imm
      01,
      .cond = 0
    };
    arm_state = init_state();
    arm_state->reg[1] = to_bf(1); // offset
    arm_state->reg[2] = to_bf(0); // Rd
    arm_state->reg[3] = to_bf(2); // Rn

    execute_TRANS(trans_ins5, arm_state);

    test_int_v(
        to_int(arm_state->reg[2]),
        1,
        "STR R0, [R1, #12], is_load = false is_up = false is_pre = true is_imm = false");

    free(arm_state);

    //is_load = false is_up = false is_pre = false is_imm = true
    trans_t trans_ins6 =
    {
      .offset = 
      {
        .shift_reg = 
        {
          .Rm = 1,
          .type = 2,
          .val = 3
        }
      },
      .Rd = 2,
      .Rn = 3,
      .is_load = false,//store
      0,
      .is_up = false,//sub
      .is_pre = false,//post
      .is_imm = true, //reg
      01,
      .cond = 0
    };
    arm_state = init_state();
    arm_state->reg[1] = to_bf(1); // offset
    arm_state->reg[2] = to_bf(0); // Rd
    arm_state->reg[3] = to_bf(2); // Rn

    execute_TRANS(trans_ins6, arm_state);

    test_int_v(
        to_int(arm_state->reg[2]),
        1,
        "STR R0, [R1], #12, is_load = false is_up = false is_pre = false is_imm = true");

    free(arm_state);

    //is_load = true is_up = true is_pre = false is_imm = false
    trans_t trans_ins7 =
    {
      .offset = 
      {
        .rot_imm.imm = 1,
        .rot_imm.amount = 2
      },
      .Rd = 2,
      .Rn = 3,
      .is_load = true, //load
      0,
      .is_up = true, //add
      .is_pre = false,//post
      .is_imm = false,//is_imm
      01,
      .cond = 0
    };
    arm_state = init_state();
    arm_state->reg[1] = to_bf(1); // offset
    arm_state->reg[2] = to_bf(0); // Rd
    arm_state->reg[3] = to_bf(2); // Rn

    execute_TRANS(trans_ins7, arm_state);

    test_int_v(
        to_int(arm_state->reg[2]),
        1,
        "LDR R0, [R1], #12, is_load = true is_up = true is_pre = false is_imm = false");

    free(arm_state);

    //is_load = true is_up = false is_pre = true is_imm = false
    trans_t trans_ins8 =
    {
      .offset = 
      {
        .rot_imm.imm = 1,
        .rot_imm.amount = 2
      },
      .Rd = 2,
      .Rn = 3,
      .is_load = true, //load
      0,
      .is_up = false,//sub
      .is_pre = true, //pre
      .is_imm = false,//is_imm
      01,
      .cond = 0
    };
    arm_state = init_state();
    arm_state->reg[1] = to_bf(1); // offset
    arm_state->reg[2] = to_bf(0); // Rd
    arm_state->reg[3] = to_bf(2); // Rn

    execute_TRANS(trans_ins8, arm_state);

    test_int_v(
        to_int(arm_state->reg[2]),
        1,
        "LDR R0, [R1, #12], is_load = true is_up = false is_pre = true is_imm = false");

    free(arm_state);

    //is_load = true is_up = false is_pre = false is_imm = true
    trans_t trans_ins9 =
    {
      .offset = 
      {
        .shift_reg = 
        {
          .Rm = 1,
          .type = 2,
          .val = 3
        }
      },
      .Rd = 2,
      .Rn = 3,
      .is_load = true, //load
      0,
      .is_up = false,//sub
      .is_pre = false,//post
      .is_imm = true, //reg
      01,
      .cond = 0
    };
    arm_state = init_state();
    arm_state->reg[1] = to_bf(1); // offset
    arm_state->reg[2] = to_bf(0); // Rd
    arm_state->reg[3] = to_bf(2); // Rn

    execute_TRANS(trans_ins9, arm_state);

    test_int_v(
        to_int(arm_state->reg[2]),
        1,
        "LDR R0, [R1], #12, is_load = true is_up = false is_pre = false is_imm = true");

    free(arm_state);

    //is_load = false is_up = true is_pre = true is_imm = false
    trans_t trans_ins10 =
    {
      .offset = 
      {
        .rot_imm.imm = 1,
        .rot_imm.amount = 2
      },
      .Rd = 2,
      .Rn = 3,
      .is_load = false, //store
      0,
      .is_up = true,//add
      .is_pre = true,//pre
      .is_imm = false, //is_imm
      01,
      .cond = 0
    };
    arm_state = init_state();
    arm_state->reg[1] = to_bf(1); // offset
    arm_state->reg[2] = to_bf(0); // Rd
    arm_state->reg[3] = to_bf(2); // Rn

    execute_TRANS(trans_ins10, arm_state);

    test_int_v(
        to_int(arm_state->reg[2]),
        1,
        "STR R0, [R1, #12], is_load = false is_up = true is_pre = true is_imm = false");

    free(arm_state);

    //is_load = false is_up = true is_pre = false is_imm = true
    trans_t trans_ins11 =
    {
      .offset = 
      {
        .shift_reg = 
        {
          .Rm = 1,
          .type = 2,
          .val = 3
        }
      },
      .Rd = 2,
      .Rn = 3,
      .is_load = false, //store
      0,
      .is_up = true,//add
      .is_pre = false,//post
      .is_imm = true, //reg
      01,
      .cond = 0
    };
    arm_state = init_state();
    arm_state->reg[1] = to_bf(1); // offset
    arm_state->reg[2] = to_bf(0); // Rd
    arm_state->reg[3] = to_bf(2); // Rn

    execute_TRANS(trans_ins11, arm_state);

    test_int_v(
        to_int(arm_state->reg[2]),
        1,
        "STR R0, [R1], #12, is_load = false is_up = true is_pre = false is_imm = true");

    free(arm_state);

    //is_load = false is_up = false is_pre = true is_imm = true
    trans_t trans_ins12 =
    {
      .offset = 
      {
        .shift_reg = 
        {
          .Rm = 1,
          .type = 2,
          .val = 3
        }
      },
      .Rd = 2,
      .Rn = 3,
      .is_load = false, //store
      0,
      .is_up = false,//sub
      .is_pre = true,//pre
      .is_imm = true, //reg
      01,
      .cond = 0
    };
    arm_state = init_state();
    arm_state->reg[1] = to_bf(1); // offset
    arm_state->reg[2] = to_bf(0); // Rd
    arm_state->reg[3] = to_bf(2); // Rn

    execute_TRANS(trans_ins12, arm_state);

    test_int_v(
        to_int(arm_state->reg[2]),
        1,
        "STR R0, [R1, #12], is_load = false is_up = false is_pre = true is_imm = true");

    free(arm_state);

    //is_load = true is_up = true is_pre = true is_imm = false
    trans_t trans_ins13 =
    {
      .offset = 
      {
        .shift_reg = 
        {
          .Rm = 1,
          .type = 2,
          .val = 3
        }
      },
      .Rd = 2,
      .Rn = 3,
      .is_load = true, //load
      0,
      .is_up = true,//add
      .is_pre = true,//pre
      .is_imm = false, //reg
      01,
      .cond = 0
    };
    arm_state = init_state();
    arm_state->reg[1] = to_bf(1); // offset
    arm_state->reg[2] = to_bf(0); // Rd
    arm_state->reg[3] = to_bf(2); // Rn

    execute_TRANS(trans_ins13, arm_state);

    test_int_v(
        to_int(arm_state->reg[2]),
        1,
        "LDR R0, [R1, #12], is_load = true is_up = true is_pre = true is_imm = false");

    free(arm_state);

    //is_load = true is_up = true is_pre = false is_imm = true
    trans_t trans_ins14 =
    {
      .offset = 
      {
        .shift_reg = 
        {
          .Rm = 1,
          .type = 2,
          .val = 3
        }
      },
      .Rd = 2,
      .Rn = 3,
      .is_load = true, //load
      0,
      .is_up = true,//add
      .is_pre = false,//post
      .is_imm = true, //reg
      01,
      .cond = 0
    };
    arm_state = init_state();
    arm_state->reg[1] = to_bf(1); // offset
    arm_state->reg[2] = to_bf(0); // Rd
    arm_state->reg[3] = to_bf(2); // Rn

    execute_TRANS(trans_ins14, arm_state);

    test_int_v(
        to_int(arm_state->reg[2]),
        1,
        "LDR R0, [R1], #12, is_load = true is_up = true is_pre = false is_imm = true");

    free(arm_state);

    //is_load = true is_up = false is_pre = true is_imm = true
    trans_t trans_ins15 =
    {
      .offset = 
      {
        .shift_reg = 
        {
          .Rm = 1,
          .type = 2,
          .val = 3
        }
      },
      .Rd = 2,
      .Rn = 3,
      .is_load = true, //load
      0,
      .is_up = false,//sub
      .is_pre = true,//pre
      .is_imm = true, //reg
      01,
      .cond = 0
    };
    arm_state = init_state();
    arm_state->reg[1] = to_bf(1); // offset
    arm_state->reg[2] = to_bf(0); // Rd
    arm_state->reg[3] = to_bf(2); // Rn

    execute_TRANS(trans_ins15, arm_state);

    test_int_v(
        to_int(arm_state->reg[2]),
        1,
        "LDR R0, [R1, #12], is_load = true is_up = false is_pre = true is_imm = true");

    free(arm_state);

    //is_load = false is_up = true is_pre = true is_imm = true
    trans_t trans_ins16 =
    {
      .offset = 
      {
        .shift_reg = 
        {
          .Rm = 1,
          .type = 2,
          .val = 3
        }
      },
      .Rd = 2,
      .Rn = 3,
      .is_load = false, //store
      0,
      .is_up = true,//add
      .is_pre = true,//pre
      .is_imm = true, //reg
      01,
      .cond = 0
    };
    arm_state = init_state();
    arm_state->reg[1] = to_bf(1); // offset
    arm_state->reg[2] = to_bf(0); // Rd
    arm_state->reg[3] = to_bf(2); // Rn

    execute_TRANS(trans_ins16, arm_state);

    test_int_v(
        to_int(arm_state->reg[2]),
        1,
        "STR R0, [R1, #12], is_load = false is_up = true is_pre = true is_imm = true");

    free(arm_state);

    //is_load = true is_up = true is_pre = true is_imm = true
    trans_t trans_ins17 =
    {
      .offset = 
      {
        .shift_reg = 
        {
          .Rm = 1,
          .type = 2,
          .val = 3
        }
      },
      .Rd = 2,
      .Rn = 3,
      .is_load = true, //load
      0,
      .is_up = true,//add
      .is_pre = true,//pre
      .is_imm = true, //reg
      01,
      .cond = 0
    };
    arm_state = init_state();
    arm_state->reg[1] = to_bf(1); // offset
    arm_state->reg[2] = to_bf(0); // Rd
    arm_state->reg[3] = to_bf(2); // Rn

    execute_TRANS(trans_ins17, arm_state);

    test_int_v(
        to_int(arm_state->reg[2]),
        1,
        "LDR R0, [R1, #12], is_load = true is_up = true is_pre = true is_imm = true");

    free(arm_state);

  }

  add_test("Test for Branch execution");
  {
    branch_t branch_ins1 =
    {
      .offset = 1,
      1010,
      .cond = 0
    };
    arm_state = init_state();
    execute_BRANCH(branch_ins1, arm_state);

    //condition is set
    branch_t branch_ins2 =
    {
      .offset = 1,
      1010,
      .cond = 1
    };
    arm_state = init_state();
    execute_BRANCH(branch_ins2, arm_state);
  }

  end_all_tests();

  return 0;
}

ArmState init_state()
{
  ArmState result = (ArmState)malloc(sizeof(arm_state_struct));
  if (result == NULL)
  {
    return NULL;
  }

  result->reg = calloc(NUM_OF_REG, sizeof(bitfield));
  result->memory = calloc(MAX_MEMORY_ADDRESS, sizeof(byte));
  if (result->reg == NULL || result->memory == NULL)
  {
    return NULL;
  }

  result->pc = 0;
  result->neg = false;
  result->zero = false;
  result->carry = false;
  result->ovflw = false;

  return result;
}