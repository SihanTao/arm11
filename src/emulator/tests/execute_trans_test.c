#include <stdlib.h>
#include <math.h>

#include "../../global_utils/types_and_macros.h"

#include "../execute/execute_helper.h"

#include "../../global_utils/tools.h"
#include "../utils/unit_test.h"
#include "../utils/init_arm_state.h"
#include "../utils/load_store.h"

int main(void)
{
  ArmState arm_state = init_state();

  add_test("Test for Trans execution");
  {
    trans_t trans_ins1 = { .offset  = { .rot_imm.imm    = 0x0000000B, // 1011
                                       .rot_imm.amount = 0 }, // offset = 11
                           .Rd      = 1,
                           .Rn      = 2,
                           .is_load = false, // str
                           0,
                           .is_up  = false, // down
                           .is_pre = false, // post
                           .is_reg  = false, // is immediate
                           01,
                           };
    arm_state          = init_state();
    arm_state->reg[1]  =0xFFFFABCD; // Rd
                                            // offseted address = 40
                                            // but post indexing
    arm_state->reg[2] =51;          // Rn,

    execute_trans(trans_ins1, arm_state);

    test_int_v(arm_state->reg[2], 40,
               "post indexing, base register value is changed");

    test_int_v(load(51, arm_state->memory), 0xFFFFABCD,
               "STR Rd, [Rn] #offset, all false");

    free(arm_state);

    // condition is set
    trans_t trans_ins2 = { .offset  = { .rot_imm.imm    = 0x0000000B, // 1011
                                       .rot_imm.amount = 0 }, // offset = 11
                           .Rd      = 1,
                           .Rn      = 2,
                           .is_load = false, // str
                           0,
                           .is_up  = false, // down
                           .is_pre = false, // post
                           .is_reg  = false, // is immediate
                           01,
                           };
    arm_state          = init_state();
    arm_state->reg[1]  =0xFFFFABCD; // Rd
                                            // offseted address = 40
                                            // but post indexing
    arm_state->reg[2] =51;          // Rn,

    execute_trans(trans_ins2, arm_state);

    test_int_v(arm_state->reg[2], 40,
               "post indexing, base register value is changed");

    test_int_v(load(51, arm_state->memory), 0xFFFFABCD,
               "STR Rd, [Rn] #offset, all false");

    free(arm_state);

    // is_load = true is_up = false is_pre = false is_reg = false
    trans_t trans_ins3 = { .offset  = { .rot_imm.imm    = 0x00000018,
                                       .rot_imm.amount = 0 }, // offset = 24
                           .Rd      = 1,
                           .Rn      = 2,
                           .is_load = true, // load
                           0,
                           .is_up  = false, // sub
                           .is_pre = false, // post
                           .is_reg  = false, // is_reg
                           01,
                           };
    arm_state          = init_state();
    arm_state->reg[1]  =0;  // Rd
    arm_state->reg[2]  =72; // Rn

    store(0xFFFFABCD, 72, arm_state->memory);
    execute_trans(trans_ins3, arm_state);

    test_int_v(arm_state->reg[2], 48,
               "post indexing, base register value is changed");
    test_int_v(arm_state->reg[1], 0xFFFFABCD,
               "LDR Rd, [Rn], #offset, is_load = true"
               "is_up = false is_pre = false is_reg = false");

    free(arm_state);

    // is_load = false is_up = true is_pre = false is_reg = false
    trans_t trans_ins4 = { .offset  = { .rot_imm.imm    = 0xC,
                                       .rot_imm.amount = 1 }, // offset = 3
                           .Rd      = 1,
                           .Rn      = 2,
                           .is_load = false, // store 0,
                           .is_up   = true,  // add
                           .is_pre  = false, // post
                           .is_reg   = false, // is_reg 01,
                           };
    arm_state          = init_state();
    arm_state->reg[1]  =0xFFFFABC1; // Rd
    arm_state->reg[2]  =10;         // Rn

    execute_trans(trans_ins4, arm_state);

    test_int_v(arm_state->reg[2], 13,
               "post indexing, base register value is changed");
    test_int_v(load(10, arm_state->memory), 0xFFFFABC1,
               "STR Rd, [Rn], #offset, is_load =false "
               "is_up = true is_pre = false is_reg = false");

    free(arm_state);

    // is_load = false is_up = false is_pre = true is_reg = false
    trans_t trans_ins5 = { .offset  = { .rot_imm.imm    = 0xC,
                                       .rot_imm.amount = 1 }, // offset = 3
                           .Rd      = 1,
                           .Rn      = 2,
                           .is_load = false, // store 0, .is_up  =
                           false,            // sub
                           .is_pre = true,   // pre
                           .is_reg  = false,  // is_reg 01,
                           };
    arm_state          = init_state();
    arm_state->reg[1]  =0xFFFFABC5; // Rd
    arm_state->reg[2]  =13;         // Rn

    execute_trans(trans_ins5, arm_state);

    test_int_v(load(10, arm_state->memory), 0xFFFFABC5,
               "STR Rd, [Rn, #offset], is_load = false "
               "is_up = false is_pre = true is_reg = false");

    free(arm_state);
    /*
                // is_load = false is_up = false is_pre = false is_reg = true
                trans_t trans_ins6
                    = { .offset  = { .shift_reg = { .Rm = 1, .type = LSL, .val
       = 1 } }, .Rd      = 2, .Rn      = 3, .is_load = false, // store 0,
       .is_up  = false, // sub .is_pre = false, // post .is_reg  = true,  // reg
       01, }; arm_state         = init_state(); arm_state->reg[1] =
           0x0000000B; // Rm arm_state->reg[2] =1;          //
       Rd arm_state->reg[3] =0;          // Rn

                execute_trans(trans_ins6, arm_state);

                test_true(&arm_state->reg[3] == &arm_state->reg[3] -
       0x00000016); test_int_v(to_int(arm_state->reg[3]), 1, "STR Rd, [Rn],
       #12, is_load = false is_up = false is_pre = " "false is_reg = true");

                free(arm_state);

                // is_load = true is_up = true is_pre = false is_reg = false
                trans_t trans_ins7
                    = { .offset  = { .rot_imm.imm = 0x0000001A, .rot_imm.amount
       = 2
           }, .Rd      = 1, .Rn      = 2, .is_load = true, // load 0, .is_up  =
           true,  // add .is_pre = false, // post .is_reg  = false, // is_reg 01,
                        };
                arm_state         = init_state();
                arm_state->reg[1] =0; // Rd
                arm_state->reg[2] =2; // Rn

                execute_trans(trans_ins7, arm_state);

                test_true(&arm_state->reg[2] == &arm_state->reg[2] +
       0xA0000001); test_int_v(to_int(arm_state->reg[1]), 2, "LDR Rd, [Rn],
       #12, is_load = true is_up = true is_pre = false " "is_reg = false");

                free(arm_state);

                // is_load = true is_up = false is_pre = true is_reg = false
                trans_t trans_ins8
                    = { .offset  = { .rot_imm.imm = 0x0000001A, .rot_imm.amount
       = 2
           }, .Rd      = 1, .Rn      = 2, .is_load = true, // load 0, .is_up  =
           false, // sub .is_pre = true,  // pre .is_reg  = false, // is_reg 01,
                        };
                arm_state         = init_state();
                arm_state->reg[1] o_bf(0; // Rd
                // arm_state->reg[2] - 0xA0000001 =2; // Rn

                execute_trans(trans_ins8, arm_state);

                test_int_v(to_int(arm_state->reg[1]), 2,
                           "LDR Rd, [Rn, #12], is_load = true is_up = false
       is_pre = true " "is_reg = false");

                free(arm_state);

                // is_load = true is_up = false is_pre = false is_reg = true
                trans_t trans_ins9
                    = { .offset  = { .shift_reg = { .Rm = 1, .type = LSR, .val
       = 2 } }, .Rd      = 2, .Rn      = 3, .is_load = true, // load 0, .is_up
       = false, // sub .is_pre = false, // post .is_reg  = true,  // reg 01,
       }; arm_state         = init_state(); arm_state->reg[1] =
           0x0000001A; // Rm arm_state->reg[2] =0;          //
       Rd arm_state->reg[3] t= o_bf(2);          // Rn

                execute_trans(trans_ins9, arm_state);

                test_true(&arm_state->reg[3] == &arm_state->reg[3] -
       0x00000006); test_int_v(to_int(arm_state->reg[2]), 2, "LDR Rd, [Rn],
       #12, is_load = true is_up = false is_pre = " "false is_reg = true");

                free(arm_state);

                    // is_load = false is_up = true is_pre = true is_reg = false
                    trans_t trans_ins10
                        = { .offset  = { .rot_imm.imm = 0x0000000B,
       .rot_imm.amount = 1
               }, .Rd      = 1, .Rn      = 2, .is_load = false, // store 0,
       .is_up = true,  // add .is_pre = true,  // pre .is_reg = false, // is_reg
       01, }; arm_state         = init_state(); arm_state->reg[1] =
           3;
               // Rd
                    // arm_state->reg[2] + 0xC0000002 =0; // Rn

                    execute_trans(trans_ins10, arm_state);

                    test_int_v(to_int(arm_state->reg[2]) + 0xC0000002, 3,
                               "STR Rd, [Rn, #12], is_load = false is_up = true
           is_pre = true " "is_reg = false");

                    free(arm_state);

                    // is_load = false is_up = true is_pre = false is_reg = true
                    trans_t trans_ins11
                        = { .offset  = { .shift_reg = { .Rm = 1, .type = ASR,
       .val = 3 } }, .Rd      = 2, .Rn      = 3, .is_load = false, // store 0,
           .is_up  = true,  // add .is_pre = false, // post .is_reg = true,  //
       reg 01, }; arm_state         = init_state(); arm_state->reg[1]
       =0xA000000B; // Rm arm_state->reg[2] =2;          // Rd
       arm_state->reg[3] =0;          // Rn

                    execute_trans(trans_ins11, arm_state);

                    test_true(&arm_state->reg[3] == &arm_state->reg[3] +
           0xF4000001); test_int_v(to_int(arm_state->reg[3]), 2, "STR Rd, [Rn],
           #12, is_load = false is_up = true is_pre = " "false is_reg = true");

                    free(arm_state);

                    // is_load = false is_up = false is_pre = true is_reg = true
                    trans_t trans_ins12
                        = { .offset  = { .shift_reg = { .Rm = 1, .type = ROR,
       .val = 3 } }, .Rd      = 2, .Rn      = 3, .is_load = false, // store 0,
           .is_up  = false, // sub .is_pre = true,  // pre .is_reg = true,  //
       reg 01, }; arm_state         = init_state(); arm_state->reg[1]
       =0x0000005D; // Rm arm_state->reg[2] =2;          // Rd
                    // arm_state->reg[3] - 0xA000000B =0; // Rn

                    execute_trans(trans_ins12, arm_state);

                    test_int_v(to_int(arm_state->reg[3]) - 0xA000000B, 2,
                               "STR Rd, [Rn, #12], is_load = false is_up =
       false is_pre = " "true is_reg = true");

                    free(arm_state);

                    // is_load = true is_up = true is_pre = true is_reg = false
                    trans_t trans_ins13
                        = { .offset  = { .shift_reg = { .Rm = 1, .type = ROR,
       .val = 3 } }, .Rd      = 2, .Rn      = 3, .is_load = true, // load 0,
       .is_up = true,  // add .is_pre = true,  // pre .is_reg = false, // reg
       01, }; arm_state         = init_state(); arm_state->reg[1] =
               0x0000005D; // Rm arm_state->reg[2] =0; // Rd
                    // arm_state->reg[3] + 0xA000000B =2; // Rn

                    execute_trans(trans_ins13, arm_state);

                    test_int_v(to_int(arm_state->reg[2]), 2,
                               "LDR Rd, [Rn, #12], is_load = true is_up = true
           is_pre = true " "is_reg = false");

                    free(arm_state);

                    // is_load = true is_up = true is_pre = false is_reg = true
                    trans_t trans_ins14
                        = { .offset  = { .shift_reg = { .Rm = 1, .type = ASR,
       .val = 3 } }, .Rd      = 2, .Rn      = 3, .is_load = true, // load 0,
       .is_up = true,  // add .is_pre = false, // post .is_reg = true,  // reg
       01, }; arm_state         = init_state(); arm_state->reg[1] =
               0xA000000B; // Rm arm_state->reg[2] =0; // Rd
       arm_state->reg[3] =2;          // Rn

                    execute_trans(trans_ins14, arm_state);

                    test_true(&arm_state->reg[3] == &arm_state->reg[3] +
           0xF4000001); test_int_v(to_int(arm_state->reg[2]), 2, "LDR Rd, [Rn],
           #12, is_load = true is_up = true is_pre = false " "is_reg = true");

                    free(arm_state);

                    // is_load = true is_up = false is_pre = true is_reg = true
                    trans_t trans_ins15
                        = { .offset  = { .shift_reg = { .Rm = 1, .type = LSR,
       .val = 2 } }, .Rd      = 2, .Rn      = 3, .is_load = true, // load 0,
       .is_up = false, // sub .is_pre = true,  // pre .is_reg = true,  // reg
       01, }; arm_state                      = init_state();
       arm_state->reg[1] =0x0000001A; // Rm arm_state->reg[2] =
       0; // Rd
                    // arm_state->reg[3] - 0x00000006 =2;          //
       Rn

                    execute_trans(trans_ins15, arm_state);

                    test_int_v(to_int(arm_state->reg[2]), 2,
                               "LDR Rd, [Rn, #12], is_load = true is_up = false
           is_pre = true " "is_reg = true");

                    free(arm_state);

                    // is_load = false is_up = true is_pre = true is_reg = true
                    trans_t trans_ins16
                        = { .offset  = { .shift_reg = { .Rm = 1, .type = LSL,
       .val = 1 } }, .Rd      = 2, .Rn      = 3, .is_load = false, // store 0,
           .is_up  = true, // add .is_pre = true, // pre .is_reg = true, // reg
       01, .cond = 0
               }; arm_state                      = init_state();
       arm_state->reg[1] =0x0000000B; // Rm arm_state->reg[2] =
       1; // Rd
                    // arm_state->reg[3] + 0x00000016 =0;          //
       Rn

                    execute_trans(trans_ins16, arm_state);

                    test_int_v(to_int(arm_state->reg[3]) + 0x00000016, 1,
                               "STR Rd, [Rn, #12], is_load = false is_up = true
           is_pre = true " "is_reg = true");

                    free(arm_state);

                    // is_load = true is_up = true is_pre = true is_reg = true
                    trans_t trans_ins17
                        = { .offset  = { .shift_reg = { .Rm = 1, .type = LSL,
       .val = 1 } }, .Rd      = 2, .Rn      = 3, .is_load = true, // load 0,
       .is_up = true, // add .is_pre = true, // pre .is_reg = true, // reg 01,
       .cond =
           0
               }; arm_state                      = init_state();
       arm_state->reg[1] =0x0000000B; // Rm arm_state->reg[2] =
       0; // Rd
                  // arm_state->memory[arm_state->reg[3] + ]
                    // arm_state->reg[3] + 0x00000016 =2;          //
       Rn

                    execute_trans(trans_ins17, arm_state);

                    test_int_v(to_int(arm_state->reg[2]), 2,
                               "LDR Rd, [Rn, #offset], is_load = true is_up =
       true is_pre = " "true is_reg = true");

                    free(arm_state);
                    */
  }

  end_all_tests();

  return 0;
}
