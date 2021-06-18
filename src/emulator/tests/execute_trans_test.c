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
                           10,
                           };
    arm_state          = init_state();
    arm_state->reg[1]  =0xFFFFABCD; // Rd
                                            // offseted address = 40
                                            // but post indexing
    arm_state->reg[2] =51;          // Rn,

    execute_trans(trans_ins1, arm_state);

    test_int_v(arm_state->reg[2], 40,
               "post indexing, base register value is changed");

    bitfield *result = 0;
    load(51, arm_state->memory, result);
    test_int_v(result, 0xFFFFABCD,
               "STR Rd, [Rn] #offset, all false");

    free(arm_state);

    // is_load = true is_up = false is_pre = false is_reg = false
    trans_t trans_ins2 = { .offset  = { .rot_imm.imm    = 0x00000018,
                                       .rot_imm.amount = 0 }, // offset = 24
                           .Rd      = 1,
                           .Rn      = 2,
                           .is_load = true, // load
                           0,
                           .is_up  = false, // sub
                           .is_pre = false, // post
                           .is_reg  = false, // is_reg
                           10,
                           };
    arm_state          = init_state();
    arm_state->reg[1]  =0;  // Rd
    arm_state->reg[2]  =72; // Rn

    store(0xFFFFABCD, 72, arm_state->memory);
    execute_trans(trans_ins2, arm_state);

    test_int_v(arm_state->reg[2], 48,
               "post indexing, base register value is changed");
    test_int_v(arm_state->reg[1], 0xFFFFABCD,
               "LDR Rd, [Rn], #offset, is_load = true"
               "is_up = false is_pre = false is_reg = false");

    free(arm_state);

    // is_load = false is_up = true is_pre = false is_reg = false
    trans_t trans_ins3 = { .offset  = { .rot_imm.imm    = 0xC,
                                       .rot_imm.amount = 1 }, // offset = 3
                           .Rd      = 1,
                           .Rn      = 2,
                           .is_load = false, // store 0,
                           .is_up   = true,  // add
                           .is_pre  = false, // post
                           .is_reg   = false, // is_reg 
                           10,
                           };
    arm_state          = init_state();
    arm_state->reg[1]  =0xFFFFABC1; // Rd
    arm_state->reg[2]  =10;         // Rn

    execute_trans(trans_ins3, arm_state);

    test_int_v(arm_state->reg[2], 13,
               "post indexing, base register value is changed");
    
    result = 0;
    load(10, arm_state->memory, result);
    test_int_v(result, 0xFFFFABC1,
               "STR Rd, [Rn], #offset, is_load =false "
               "is_up = true is_pre = false is_reg = false");

    free(arm_state);

    // is_load = false is_up = false is_pre = true is_reg = false
    trans_t trans_ins4 = { .offset  = { .rot_imm.imm    = 0xC,
                                       .rot_imm.amount = 1 }, // offset = 3
                           .Rd      = 1,
                           .Rn      = 2,
                           .is_load = false, // store 0, 
                           .is_up   = false, // sub
                           .is_pre  = true,  // pre
                           .is_reg  = false, // is_reg 
                           10,
                           };
    arm_state          = init_state();
    arm_state->reg[1]  =0xFFFFABC5; // Rd
    arm_state->reg[2]  =13;         // Rn

    execute_trans(trans_ins4, arm_state);

    result = 0;
    load(10, arm_state->memory, result);
    test_int_v(result, 0xFFFFABC5,
               "STR Rd, [Rn, #offset], is_load = false "
               "is_up = false is_pre = true is_reg = false");

    free(arm_state);
    
    //is_load = false is_up = false is_pre = false is_reg = true
    trans_t trans_ins5 = { .offset  = { .shift_reg = { .Rm = 1, 
                                                       .type = LSL, 
                                                       .val = 1 } }, //offset = 22
                           .Rd      = 2, 
                           .Rn      = 3, 
                           .is_load = false, // store 0
                           .is_up   = false, // sub 
                           .is_pre  = false, // post 
                           .is_reg  = true,  // reg
                           01, }; 
    arm_state = init_state(); 
    arm_state->reg[1] = 0x0000000B;  // Rm 
    arm_state->reg[2] = 0x00000ABC;  //Rd 
    arm_state->reg[3] = 52;          // Rn

    execute_trans(trans_ins5, arm_state);
    
    test_int_v(arm_state->reg[2], 30, "post indexing, base register value is changed");

    result = 0;
    load(52, arm_state->memory, result);
    test_int_v(result, 0x00000ABC, 
               "STR Rd, [Rn], #12"
               "is_load = false is_up = false is_pre = false is_reg = true");

    free(arm_state);
    }
    
    end_all_tests();

    return 0;
} 

    