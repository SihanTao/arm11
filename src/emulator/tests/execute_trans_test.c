#include <stdlib.h>
#include <math.h>

#include "../utils/types_and_macros.h"

#include "../execute/execute_helper.h"

#include "../utils/tools.h"
#include "../utils/unit_test.h"
#include "../utils/init_arm_state.h"
#include "../utils/load_store.h"

int main(void)
{
  ArmState arm_state = init_state();

  add_test("Test for Trans execution");
  {
    /*
     * STR R0, [R1, #12]
     * LDR R0, [R1, #12]
     * STR R0, [R1], #12
     * LDR R0, [R1], #12
     */

    trans_t trans_ins1 = { .offset  = { .rot_imm.imm    = 0x0000000B, // 1011
                                       .rot_imm.amount = 0 },
                           .Rd      = 1,
                           .Rn      = 2,
                           .is_load = false, // str
                           0,
                           .is_up  = false, // down
                           .is_pre = false, // post
                           .iFlag  = false, // is immediate
                           01,
                           .cond = 0 };
    arm_state          = init_state();
    arm_state->reg[1]  = to_bf(0xFFFFABCD); // Rd
                                            // offseted address = 40
                                            // but post indexing
    arm_state->reg[2] = to_bf(51);          // Rn,

    byte *memory = arm_state->memory;

    execute_TRANS(trans_ins1, arm_state);

    test_int_v(to_int(arm_state->reg[2]), 40,
               "post indexing, base register value is changed");

    test_int_v(to_int(load(51, memory)), 0xFFFFABCD, "store to memory");

    free(arm_state);
    /*
        // condition is set
        trans_t trans_ins2
            = { .offset  = { .rot_imm.imm = 0x0000001A, .rot_imm.amount = 2 },
                .Rd      = 2,
                .Rn      = 3,
                .is_load = false, // store
                0,
                .is_up  = false, // sub
                .is_pre = false, // post
                .iFlag  = false, // iFlag
                01,
                .cond = 1 };
        arm_state         = init_state();
        arm_state->reg[2] = to_bf(1); // Rd
        arm_state->reg[3] = to_bf(0); // Rn

        execute_TRANS(trans_ins2, arm_state);

        test_true(&arm_state->reg[2] == &arm_state->reg[2] - 0xA0000001);
        test_int_v(to_int(arm_state->reg[2]), 1,
                   "STR Rd, [Rn], #12, 11010 becomes 1010..0001, all false");

        free(arm_state);

        // is_load = true is_up = false is_pre = false iFlag = false
        trans_t trans_ins3
            = { .offset  = { .rot_imm.imm = 0x0000001A, .rot_imm.amount = 2 },
                .Rd      = 1,
                .Rn      = 2,
                .is_load = true, // load
                0,
                .is_up  = false, // sub
                .is_pre = false, // post
                .iFlag  = false, // iFlag
                01,
                .cond = 0 };
        arm_state         = init_state();
        arm_state->reg[1] = to_bf(0); // Rd
        arm_state->reg[2] = to_bf(2); // Rn

        execute_TRANS(trans_ins3, arm_state);

        test_true(&arm_state->reg[2] == &arm_state->reg[2] - 0xA0000001);
        test_int_v(to_int(arm_state->reg[1]), 2,
                   "LDR Rd, [Rn], #12, 11010 becomes 1010..0001, is_load = true
       " "is_up = false is_pre = false iFlag = false");

        free(arm_state);

            // is_load = false is_up = true is_pre = false iFlag = false
            trans_t trans_ins4
                = { .offset  = { .rot_imm.imm = 0x0000000B, .rot_imm.amount = 1
       }, .Rd      = 1, .Rn      = 2, .is_load = false, // store 0, .is_up  =
       true,  // add .is_pre = false, // post .iFlag  = false, // iFlag 01,
                    .cond = 0 };
            arm_state         = init_state();
            arm_state->reg[1] = to_bf(2); // Rd
            arm_state->reg[2] = to_bf(0); // Rn

            execute_TRANS(trans_ins4, arm_state);

            test_true(&arm_state->reg[2] == &arm_state->reg[2] + 0xC0000002);
            test_int_v(to_int(arm_state->reg[2]), 2,
                       "STR Rd, [Rn], #12, 1011 becomes 1100..0010, is_load =
       false " "is_up = true is_pre = false iFlag = false");

            free(arm_state);

            // is_load = false is_up = false is_pre = true iFlag = false
            trans_t trans_ins5
                = { .offset  = { .rot_imm.imm = 0x0000000B, .rot_imm.amount = 1
       }, .Rd      = 1, .Rn      = 2, .is_load = false, // store 0, .is_up  =
       false, // sub .is_pre = true,  // pre .iFlag  = false, // iFlag 01,
                    .cond = 0 };
            arm_state         = init_state();
            arm_state->reg[1] = to_bf(3); // Rd
            arm_state->reg[2] = to_bf(0); // Rn

            execute_TRANS(trans_ins5, arm_state);

            test_int_v(to_int(arm_state->reg[2]) - 0xC0000002, 3,
                       "STR Rd, [Rn, #12], 1011 becomes 1100..0010, is_load =
       false " "is_up = false is_pre = true iFlag = false");

            free(arm_state);

            // is_load = false is_up = false is_pre = false iFlag = true
            trans_t trans_ins6
                = { .offset  = { .shift_reg = { .Rm = 1, .type = LSL, .val = 1
       } }, .Rd      = 2, .Rn      = 3, .is_load = false, // store 0, .is_up  =
       false, // sub .is_pre = false, // post .iFlag  = true,  // reg 01, .cond
       = 0 }; arm_state         = init_state(); arm_state->reg[1] =
       to_bf(0x0000000B); // Rm arm_state->reg[2] = to_bf(1);          // Rd
            arm_state->reg[3] = to_bf(0);          // Rn

            execute_TRANS(trans_ins6, arm_state);

            test_true(&arm_state->reg[3] == &arm_state->reg[3] - 0x00000016);
            test_int_v(to_int(arm_state->reg[3]), 1,
                       "STR Rd, [Rn], #12, is_load = false is_up = false is_pre
       = " "false iFlag = true");

            free(arm_state);

            // is_load = true is_up = true is_pre = false iFlag = false
            trans_t trans_ins7
                = { .offset  = { .rot_imm.imm = 0x0000001A, .rot_imm.amount = 2
       }, .Rd      = 1, .Rn      = 2, .is_load = true, // load 0, .is_up  =
       true,  // add .is_pre = false, // post .iFlag  = false, // iFlag 01,
                    .cond = 0 };
            arm_state         = init_state();
            arm_state->reg[1] = to_bf(0); // Rd
            arm_state->reg[2] = to_bf(2); // Rn

            execute_TRANS(trans_ins7, arm_state);

            test_true(&arm_state->reg[2] == &arm_state->reg[2] + 0xA0000001);
            test_int_v(to_int(arm_state->reg[1]), 2,
                       "LDR Rd, [Rn], #12, is_load = true is_up = true is_pre =
           false " "iFlag = false");

            free(arm_state);

            // is_load = true is_up = false is_pre = true iFlag = false
            trans_t trans_ins8
                = { .offset  = { .rot_imm.imm = 0x0000001A, .rot_imm.amount = 2
       }, .Rd      = 1, .Rn      = 2, .is_load = true, // load 0, .is_up  =
       false, // sub .is_pre = true,  // pre .iFlag  = false, // iFlag 01,
                    .cond = 0 };
            arm_state         = init_state();
            arm_state->reg[1] = to_bf(0); // Rd
            // arm_state->reg[2] - 0xA0000001 = to_bf(2); // Rn

            execute_TRANS(trans_ins8, arm_state);

            test_int_v(to_int(arm_state->reg[1]), 2,
                       "LDR Rd, [Rn, #12], is_load = true is_up = false is_pre
       = true " "iFlag = false");

            free(arm_state);

            // is_load = true is_up = false is_pre = false iFlag = true
            trans_t trans_ins9
                = { .offset  = { .shift_reg = { .Rm = 1, .type = LSR, .val = 2
       } }, .Rd      = 2, .Rn      = 3, .is_load = true, // load 0, .is_up  =
       false, // sub .is_pre = false, // post .iFlag  = true,  // reg 01, .cond
       = 0 }; arm_state         = init_state(); arm_state->reg[1] =
       to_bf(0x0000001A); // Rm arm_state->reg[2] = to_bf(0);          // Rd
            arm_state->reg[3] = to_bf(2);          // Rn

            execute_TRANS(trans_ins9, arm_state);

            test_true(&arm_state->reg[3] == &arm_state->reg[3] - 0x00000006);
            test_int_v(to_int(arm_state->reg[2]), 2,
                       "LDR Rd, [Rn], #12, is_load = true is_up = false is_pre
       = " "false iFlag = true");

            free(arm_state);

                // is_load = false is_up = true is_pre = true iFlag = false
                trans_t trans_ins10
                    = { .offset  = { .rot_imm.imm = 0x0000000B, .rot_imm.amount
       = 1
           }, .Rd      = 1, .Rn      = 2, .is_load = false, // store 0, .is_up
       = true,  // add .is_pre = true,  // pre .iFlag = false, // iFlag 01,
       .cond = 0 }; arm_state         = init_state(); arm_state->reg[1] =
       to_bf(3);
           // Rd
                // arm_state->reg[2] + 0xC0000002 = to_bf(0); // Rn

                execute_TRANS(trans_ins10, arm_state);

                test_int_v(to_int(arm_state->reg[2]) + 0xC0000002, 3,
                           "STR Rd, [Rn, #12], is_load = false is_up = true
       is_pre = true " "iFlag = false");

                free(arm_state);

                // is_load = false is_up = true is_pre = false iFlag = true
                trans_t trans_ins11
                    = { .offset  = { .shift_reg = { .Rm = 1, .type = ASR, .val
       = 3 } }, .Rd      = 2, .Rn      = 3, .is_load = false, // store 0,
       .is_up  = true,  // add .is_pre = false, // post .iFlag = true,  // reg
       01, .cond = 0 }; arm_state         = init_state(); arm_state->reg[1] =
           to_bf(0xA000000B); // Rm arm_state->reg[2] = to_bf(2);          //
       Rd arm_state->reg[3] = to_bf(0);          // Rn

                execute_TRANS(trans_ins11, arm_state);

                test_true(&arm_state->reg[3] == &arm_state->reg[3] +
       0xF4000001); test_int_v(to_int(arm_state->reg[3]), 2, "STR Rd, [Rn],
       #12, is_load = false is_up = true is_pre = " "false iFlag = true");

                free(arm_state);

                // is_load = false is_up = false is_pre = true iFlag = true
                trans_t trans_ins12
                    = { .offset  = { .shift_reg = { .Rm = 1, .type = ROR, .val
       = 3 } }, .Rd      = 2, .Rn      = 3, .is_load = false, // store 0,
       .is_up  = false, // sub .is_pre = true,  // pre .iFlag = true,  // reg
       01, .cond = 0 }; arm_state         = init_state(); arm_state->reg[1] =
           to_bf(0x0000005D); // Rm arm_state->reg[2] = to_bf(2);          //
       Rd
                // arm_state->reg[3] - 0xA000000B = to_bf(0); // Rn

                execute_TRANS(trans_ins12, arm_state);

                test_int_v(to_int(arm_state->reg[3]) - 0xA000000B, 2,
                           "STR Rd, [Rn, #12], is_load = false is_up = false
       is_pre = " "true iFlag = true");

                free(arm_state);

                // is_load = true is_up = true is_pre = true iFlag = false
                trans_t trans_ins13
                    = { .offset  = { .shift_reg = { .Rm = 1, .type = ROR, .val
       = 3 } }, .Rd      = 2, .Rn      = 3, .is_load = true, // load 0, .is_up
       = true,  // add .is_pre = true,  // pre .iFlag = false, // reg 01, .cond
       = 0 }; arm_state         = init_state(); arm_state->reg[1] =
           to_bf(0x0000005D); // Rm arm_state->reg[2] = to_bf(0);          //
       Rd
                // arm_state->reg[3] + 0xA000000B = to_bf(2); // Rn

                execute_TRANS(trans_ins13, arm_state);

                test_int_v(to_int(arm_state->reg[2]), 2,
                           "LDR Rd, [Rn, #12], is_load = true is_up = true
       is_pre = true " "iFlag = false");

                free(arm_state);

                // is_load = true is_up = true is_pre = false iFlag = true
                trans_t trans_ins14
                    = { .offset  = { .shift_reg = { .Rm = 1, .type = ASR, .val
       = 3 } }, .Rd      = 2, .Rn      = 3, .is_load = true, // load 0, .is_up
       = true,  // add .is_pre = false, // post .iFlag = true,  // reg 01,
       .cond = 0 }; arm_state         = init_state(); arm_state->reg[1] =
           to_bf(0xA000000B); // Rm arm_state->reg[2] = to_bf(0);          //
       Rd arm_state->reg[3] = to_bf(2);          // Rn

                execute_TRANS(trans_ins14, arm_state);

                test_true(&arm_state->reg[3] == &arm_state->reg[3] +
       0xF4000001); test_int_v(to_int(arm_state->reg[2]), 2, "LDR Rd, [Rn],
       #12, is_load = true is_up = true is_pre = false " "iFlag = true");

                free(arm_state);

                // is_load = true is_up = false is_pre = true iFlag = true
                trans_t trans_ins15
                    = { .offset  = { .shift_reg = { .Rm = 1, .type = LSR, .val
       = 2 } }, .Rd      = 2, .Rn      = 3, .is_load = true, // load 0, .is_up
       = false, // sub .is_pre = true,  // pre .iFlag = true,  // reg 01, .cond
       = 0 }; arm_state                      = init_state(); arm_state->reg[1]
       = to_bf(0x0000001A); // Rm arm_state->reg[2]              = to_bf(0); //
           Rd
                // arm_state->reg[3] - 0x00000006 = to_bf(2);          // Rn

                execute_TRANS(trans_ins15, arm_state);

                test_int_v(to_int(arm_state->reg[2]), 2,
                           "LDR Rd, [Rn, #12], is_load = true is_up = false
       is_pre = true " "iFlag = true");

                free(arm_state);

                // is_load = false is_up = true is_pre = true iFlag = true
                trans_t trans_ins16
                    = { .offset  = { .shift_reg = { .Rm = 1, .type = LSL, .val
       = 1 } }, .Rd      = 2, .Rn      = 3, .is_load = false, // store 0,
       .is_up  = true, // add .is_pre = true, // pre .iFlag = true, // reg 01,
       .cond = 0
           }; arm_state                      = init_state(); arm_state->reg[1]
       = to_bf(0x0000000B); // Rm arm_state->reg[2]              = to_bf(1); //
           Rd
                // arm_state->reg[3] + 0x00000016 = to_bf(0);          // Rn

                execute_TRANS(trans_ins16, arm_state);

                test_int_v(to_int(arm_state->reg[3]) + 0x00000016, 1,
                           "STR Rd, [Rn, #12], is_load = false is_up = true
       is_pre = true " "iFlag = true");

                free(arm_state);

                // is_load = true is_up = true is_pre = true iFlag = true
                trans_t trans_ins17
                    = { .offset  = { .shift_reg = { .Rm = 1, .type = LSL, .val
       = 1 } }, .Rd      = 2, .Rn      = 3, .is_load = true, // load 0, .is_up
       = true, // add .is_pre = true, // pre .iFlag = true, // reg 01, .cond =
       0
           }; arm_state                      = init_state(); arm_state->reg[1]
       = to_bf(0x0000000B); // Rm arm_state->reg[2]              = to_bf(0); //
           Rd
              // arm_state->memory[arm_state->reg[3] + ]
                // arm_state->reg[3] + 0x00000016 = to_bf(2);          // Rn

                execute_TRANS(trans_ins17, arm_state);

                test_int_v(to_int(arm_state->reg[2]), 2,
                           "LDR Rd, [Rn, #offset], is_load = true is_up = true
           is_pre = " "true iFlag = true");

                free(arm_state);
                */
  }

  add_test("Test for Branch execution");
  {
    branch_t branch_ins1 = { .offset = 1, 1010, .cond = 0 };
    arm_state            = init_state();
    execute_BRANCH(branch_ins1, arm_state);

    // condition is set
    branch_t branch_ins2 = { .offset = 1, 1010, .cond = 1 };
    arm_state            = init_state();
    execute_BRANCH(branch_ins2, arm_state);
  }

  end_all_tests();

  return 0;
}
