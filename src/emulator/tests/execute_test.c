// #include <stdlib.h>
// #include <math.h>
// #include "../utils/types_and_macros.h"
// #include "../execute/execute_helper.h"
// #include "../execute/execute_SDT.h"
// #include "../execute/execute_DP.h"
// #include "../execute/execute.h"
// #include "../utils/tools.h"
// #include "../utils/unit_test.h"

// static ArmState init_state();

// int main(void)
// {
//   ArmState arm_state = init_state();

//   add_test("Test for rotate");
//   {
//       test_int_v(rotate(1, 0x0000000B), 0x80000005, "1011 becomes 1000..0101");

//       test_int_v(rotate(2, 0x0000001A), 0x40000006, "11010 becomes 0100..0110");

//       test_int_v(rotate(3, 0x0000005D), 0xA000000B, "1011101 becomes 1010..1011");
//   }

//   add_test("Test for shift");
//   {
//       test_int_v(shift(0x0000000B, 1, LSL), 0x00000016, "1011 becomes 10110");

//       test_int_v(shift(0x0000001A, 2, LSR), 0x00000006, "11010 becomes 110");

//       test_int_v(shift(0xA000000B, 3, ASR), 0xF4000001, "1010..1011 becomes 1111010..0001");

//       test_int_v(shift(0x0000005D, 3, ROR), 0xA000000B, "1011101 becomes 1010..1011");
//   }

//   add_test("Test for arith_right");
//   {
//       test_int_v(arith_right(1, 0x0000001A), 0x0000000D, "..11010 becomes ..1101");

//       test_int_v(arith_right(2, 0x80000005), 0xE0000001, "1000..0101 becomes 1110..0001");

//       test_int_v(arith_right(3, 0xA000000B), 0xF4000001, "1010..1011 becomes 1111010..0001");
//   }

// //   add_test("Test for shift_imm_handle");
// //   {
// //       test_int_v(shift_imm_handle(1, 0x0000000B), 0x80000005, "1011 becomes 1101");

// //       test_int_v(shift_imm_handle(1, 0x0000000B), 0x80000005, "1011 becomes 1101");

// //       test_int_v(shift_imm_handle(1, 0x0000000B), 0x80000005, "1011 becomes 1101");
// //   }
// /*
//   add_test("Test for data processing execution");
//   {
//       instruction_t dp_ins1 =
//         {.tag = DATA_PROCESS,
//          .word.dp = {
//              .operand2 = {
//                  .shift_reg = {
//                      .Rm = ,
//                      .shift = {
//                          .val = ,
//                          .type = }}},
//              .Rd = 2,
//              .Rn = 3,
//              .S = false,//CPSR flags not update
//              .opcode = 4,
//              .I = false,//op2 is a register
//              0,
//              .cond = 0}};
//     arm_state = init_state();
//     arm_state->reg[1] = to_bf(0xFFFFFFFD); // Rm
//     arm_state->reg[2] = to_bf(0xF);        // Rd
//     arm_state->reg[3] = to_bf(0);          // Rn

//     execute_DP(&dp_ins1, arm_state);

//     test_int_v(
//         to_int(arm_state->reg[2]),
//         0xEFFFFFFD3,
//         "mul 0xFFFFFFFFD 0xF 0, S false I false");

//     free(arm_state);

//     //condition is set
//     instruction_t dp_ins2 =
//         {.tag = DATA_PROCESS,
//          .word.dp = {
//              .operand2 = {
//                  .op2 = 1,
//                  .Register = {
//                      .Rm = 1,
//                      .Shift = {
//                          .Integer = ,
//                          .ShiftT = }}},
//              .Rd = 2,
//              .Rn = 3,
//              .S = false,//CPSR flags not update
//              .opcode = 4,
//              .I = false,//op2 is a register
//              0,
//              .cond = 1}};
//     arm_state = init_state();
//     arm_state->reg[1] = to_bf(0xFFFFFFFD); // op2
//     arm_state->reg[2] = to_bf(0xF);        // Rd
//     arm_state->reg[3] = to_bf(0);          // Rn

//     execute_DP(&dp_ins2, arm_state);

//     test_int_v(
//         to_int(arm_state->reg[2]),
//         0xEFFFFFFD3,
//         "mul 0xFFFFFFFFD 0xF 0, S false I false");

//     free(arm_state);

//     //S = true I = false
//     instruction_t dp_ins3 =
//         {.tag = DATA_PROCESS,
//          .word.dp = {
//              .operand2 = {
//                  .op2 = 1,
//                  .Register = {
//                      .Rm = 1,
//                      .Shift = {
//                          .Integer = ,
//                          .ShiftT = }}},
//              .Rd = 2,
//              .Rn = 3,
//              .S = true, //CPSR flags update
//              .opcode = 4,
//              .I = false,//op2 is a register
//              0,
//              .cond = 0}};
//     arm_state = init_state();
//     arm_state->reg[1] = to_bf(0xFFFFFFFD); // op2
//     arm_state->reg[2] = to_bf(0xF);        // Rd
//     arm_state->reg[3] = to_bf(0);          // Rn

//     execute_DP(&dp_ins3, arm_state);

//     test_int_v(
//         to_int(arm_state->reg[2]),
//         0xEFFFFFFD3,
//         "mul 0xFFFFFFFFD 0xF 0, S true I false");

//     // C is set to the carry out from any shift operation
//     //test_true(arm_state->flagC == );

//     // N is set to bit 31 of the result
//     test_true(arm_state->flagN == get_bit(0xFFFFFFD3, 31));

//     // Z is set if and only if the result is zero.
//     test_true(arm_state->flagZ == (to_int(arm_state->reg[2]) == 0));

//     free(arm_state);


//     //S = false I = true
//     instruction_t dp_ins4 =
//         {.tag = DATA_PROCESS,
//          .word.dp = {
//              .operand2 = {
//                  .op2 = ,
//                  .Iv = {
//                      .Imm = ,
//                      .Rotate = }},
//              .Rd = 2,
//              .Rn = 3,
//              .S = false, //CPSR flags not update
//              .opcode = 4,
//              .I = true, //op2 is an immediate constant
//              0,
//              .cond = 0}};
//     arm_state = init_state();

//     arm_state->reg[2] = to_bf(0xF);        // Rd
//     arm_state->reg[3] = to_bf(0);          // Rn

//     execute_DP(&dp_ins4, arm_state);

//     test_int_v(
//         to_int(arm_state->reg[2]),
//         0xEFFFFFFD3,
//         "mul 0xFFFFFFFFD 0xF 0, S false I true");

//     free(arm_state);

//     //S = true I = true
//     instruction_t dp_ins5 =
//         {.tag = DATA_PROCESS,
//          .word.dp = {
//              .operand2 = {
//                  .op2 = ,
//                  .Iv = {
//                      .Imm = ,
//                      .Rotate = }},
//              .Rd = 2,
//              .Rn = 3,
//              .S = true, //CPSR flags update
//              .opcode = 4,
//              .I = true, //op2 is an immediate constant
//              0,
//              .cond = 0}};
//     arm_state = init_state();

//     arm_state->reg[2] = to_bf(0xF);        // Rd
//     arm_state->reg[3] = to_bf(0);          // Rn

//     execute_DP(&dp_ins5, arm_state);

//     test_int_v(
//         to_int(arm_state->reg[2]),
//         0xEFFFFFFD3,
//         "mul 0xFFFFFFFFD 0xF 0, S true I true");

//     // C is set to the carry out from any shift operation
//     //test_true(arm_state->flagC == );

//     // N is set to bit 31 of the result
//     test_true( arm_state->flagN == get_bit(0xFFFFFFD3, 31));

//     // Z is set if and only if the result is zero.
//     test_true(arm_state->flagZ == (to_int(arm_state->reg[2]) == 0));

//     free(arm_state);
//   }
//   */

//   add_test("Test for Mul execution");
//   {
//     // S = true A = false
//     instruction_t mul_ins1 =
//         {.tag = MUL,
//          .word.mul = {
//              .Rm = 1,
//              1001,
//              .Rs = 2,
//              .Rn = 3,
//              .Rd = 4,
//              .S = true,
//              .A = false,
//              0,
//              .cond = 0}};

//     arm_state->reg[1] = to_bf(0xFFFFFFFD); // Rm
//     arm_state->reg[2] = to_bf(0xF);        // Rs
//     arm_state->reg[4] = to_bf(0);          // Rd

//     execute_MUL(mul_ins1, arm_state);

//     test_int_v(
//         to_int(arm_state->reg[4]),
//         0xEFFFFFFD3,
//         "mul 0xFFFFFFFFD 0xF 0, S true A false");

//     // N is set to bit 31 of the result
//     test_true( arm_state->flagN == get_bit(0xFFFFFFD3, 31));

//     // Z is set if and only if the result is zero.
//     test_true(arm_state->flagZ == (to_int(arm_state->reg[4]) == 0));

//     free(arm_state);

//     // S = true A = flase condition is set
//     instruction_t mul_ins2 =
//         {.tag = MUL,
//          .word.mul = {
//              .Rm = 1,
//              1001,
//              .Rs = 2,
//              .Rn = 3,
//              .Rd = 4,
//              .S = true,
//              .A = false,
//              01,
//              .cond = 1}};
//     arm_state = init_state();
//     arm_state->reg[1] = to_bf(0xFFFFFFFD); // Rm
//     arm_state->reg[2] = to_bf(0xF);        // Rs
//     arm_state->reg[4] = to_bf(0);          // Rd

//     execute_MUL(&mul_ins2, arm_state);

//     test_int_v(
//         to_int(arm_state->reg[4]),
//         0xEFFFFFFD3,
//         "mul 0xFFFFFFFFD 0xF 0, S true A false");

//     // N is set to bit 31 of the result
//     test_true( arm_state->flagN == get_bit(0xFFFFFFD3, 31));

//     // Z is set if and only if the result is zero.
//     test_true(arm_state->flagZ == (to_int(arm_state->reg[4]) == 0));

//     free(arm_state);

//     // S = true A = true
//     instruction_t mul_ins3 =
//         {.tag = MUL,
//          .word.mul = {
//              .Rm = 1,
//              1001,
//              .Rs = 2,
//              .Rn = 3,
//              .Rd = 4,
//              .S = true,
//              .A = true,
//              0,
//              .cond = 0}};
//     arm_state = init_state();
//     arm_state->reg[1] = to_bf(1); // Rm
//     arm_state->reg[2] = to_bf(2); // Rs
//     arm_state->reg[3] = to_bf(3); // Rn
//     arm_state->reg[4] = to_bf(0); // Rd

//     execute_MUL(&mul_ins3, arm_state);

//     test_int_v(
//         to_int(arm_state->reg[4]),
//         0x00000005,
//         "mul 1 2 3 0, S true A true");

//     // N is set to bit 31 of the result
//     test_true( arm_state->flagN == get_bit(0x00000005, 31));

//     // Z is set if and only if the result is zero.
//     test_true(arm_state->flagZ == (to_int(arm_state->reg[4]) == 0));

//     free(arm_state);

//     // S = false A = true
//     instruction_t mul_ins4 =
//         {.tag = MUL,
//          .word.mul = {
//              .Rm = 1,
//              1001,
//              .Rs = 2,
//              .Rn = 3,
//              .Rd = 4,
//              .S = false,
//              .A = true,
//              0,
//              .cond = 0}};
//     arm_state = init_state();
//     arm_state->reg[1] = to_bf(1); // Rm
//     arm_state->reg[2] = to_bf(2); // Rs
//     arm_state->reg[3] = to_bf(3); // Rn
//     arm_state->reg[4] = to_bf(0); // Rd

//     execute_MUL(&mul_ins4, arm_state);

//     test_int_v(
//         to_int(arm_state->reg[4]),
//         0x00000005,
//         "mul 1 2 3 0, S false A true");

//     free(arm_state);

//     // S = false A = flase
//     instruction_t mul_ins5 =
//         {.tag = MUL,
//          .word.mul = {
//              .Rm = 1,
//              1001,
//              .Rs = 2,
//              .Rn = 3,
//              .Rd = 4,
//              .S = false,
//              .A = false,
//              0,
//              .cond = 0}};
//     arm_state = init_state();
//     arm_state->reg[1] = to_bf(0xFFFFFFFD); // Rm
//     arm_state->reg[2] = to_bf(0xF);        // Rs
//     arm_state->reg[4] = to_bf(0);          // Rd

//     execute_MUL(&mul_ins5, arm_state);

//     test_int_v(
//         to_int(arm_state->reg[4]),
//         0xEFFFFFFD3,
//         "mul 0xFFFFFFFFD 0xF 0, S false A false");

//     free(arm_state);
//    }

//   add_test("Test for Trans execution");
//   {
//     /* STR R0, [R1, #12]
//      * LDR R0, [R1, #12]
//      * STR R0, [R1], #12
//      * LDR R0, [R1], #12
//      */

//     instruction_t trans_ins1 =
//         {.tag = TRANS,
//          .word.trans = {
//             // .offset = {
//               //   .Io.Imm = ,
//               //   .Io.Rotate = ,
//               //   .offset_value = },
//              .Rd = 2,
//              .Rn = 3,
//              .L = false,//str
//              0,
//              .U = false,//sub
//              .P = false,//post
//              .I = false,//imm
//              01,
//              .cond = 0}};
//     arm_state = init_state();
//     arm_state->reg[1] = to_bf(2); // offset, 0000 0000 0010
//     arm_state->reg[2] = to_bf(0); // Rd
//     arm_state->reg[3] = to_bf(1); // Rn,

//     execute_TRANS(&trans_ins1, arm_state);

//     test_int_v(
//         to_int(arm_state->reg[2]),
//         1,
//         "STR R0, [R1], #12, all false");

//     free(arm_state);

//     //condition is set
//     instruction_t trans_ins2 =
//         {.tag = TRANS,
//          .word.trans = {
//              .offset = 1,
//              .Rd = 2,
//              .Rn = 3,
//              .L = false,//store
//              0,
//              .U = false,//sub
//              .P = false,//post
//              .I = false,//imm
//              01,
//              .cond = 1}};
//     arm_state = init_state();
//     arm_state->reg[1] = to_bf(1); // offset
//     arm_state->reg[2] = to_bf(0); // Rd
//     arm_state->reg[3] = to_bf(2); // Rn

//     execute_TRANS(&trans_ins2, arm_state);

//     test_int_v(
//         to_int(arm_state->reg[2]),
//         1,
//         "STR R0, [R1], #12, all false");

//     free(arm_state);

//     //L = true U = false P = false I = false
//     instruction_t trans_ins3 =
//         {.tag = TRANS,
//          .word.trans = {
//              .offset = 1,
//              .Rd = 2,
//              .Rn = 3,
//              .L = true, //load
//              0,
//              .U = false,//sub
//              .P = false,//post
//              .I = false,//imm
//              01,
//              .cond = 0}};
//     arm_state = init_state();
//     arm_state->reg[1] = to_bf(1); // offset
//     arm_state->reg[2] = to_bf(0); // Rd
//     arm_state->reg[3] = to_bf(2); // Rn

//     execute_TRANS(&trans_ins3, arm_state);

//     test_int_v(
//         to_int(arm_state->reg[2]),
//         1,
//         "LDR R0, [R1], #12, L = true U = false P = false I = false");

//     free(arm_state);

//     //L = false U = true P = false I = false
//     instruction_t trans_ins4 =
//         {.tag = TRANS,
//          .word.trans = {
//              .offset = 1,
//              .Rd = 2,
//              .Rn = 3,
//              .L = false,//store
//              0,
//              .U = true, //add
//              .P = false,//post
//              .I = false,//imm
//              01,
//              .cond = 0}};
//     arm_state = init_state();
//     arm_state->reg[1] = to_bf(1); // offset
//     arm_state->reg[2] = to_bf(0); // Rd
//     arm_state->reg[3] = to_bf(2); // Rn

//     execute_TRANS(&trans_ins4, arm_state);

//     test_int_v(
//         to_int(arm_state->reg[2]),
//         1,
//         "STR R0, [R1], #12, L = false U = true P = false I = false");

//     free(arm_state);

//     //L = false U = false P = true I = false
//     instruction_t trans_ins5 =
//         {.tag = TRANS,
//          .word.trans = {
//              .offset = 1,
//              .Rd = 2,
//              .Rn = 3,
//              .L = false,//store
//              0,
//              .U = false,//sub
//              .P = true, //pre
//              .I = false,//imm
//              01,
//              .cond = 0}};
//     arm_state = init_state();
//     arm_state->reg[1] = to_bf(1); // offset
//     arm_state->reg[2] = to_bf(0); // Rd
//     arm_state->reg[3] = to_bf(2); // Rn

//     execute_TRANS(&trans_ins5, arm_state);

//     test_int_v(
//         to_int(arm_state->reg[2]),
//         1,
//         "STR R0, [R1, #12], L = false U = false P = true I = false");

//     free(arm_state);

//     //L = false U = false P = false I = true
//     instruction_t trans_ins6 =
//         {.tag = TRANS,
//          .word.trans = {
//              .offset = 1,
//              .Rd = 2,
//              .Rn = 3,
//              .L = false,//store
//              0,
//              .U = false,//sub
//              .P = false,//post
//              .I = true, //reg
//              01,
//              .cond = 0}};
//     arm_state = init_state();
//     arm_state->reg[1] = to_bf(1); // offset
//     arm_state->reg[2] = to_bf(0); // Rd
//     arm_state->reg[3] = to_bf(2); // Rn

//     execute_TRANS(&trans_ins6, arm_state);

//     test_int_v(
//         to_int(arm_state->reg[2]),
//         1,
//         "STR R0, [R1], #12, L = false U = false P = false I = true");

//     free(arm_state);

//     //L = true U = true P = false I = false
//     instruction_t trans_ins7 =
//         {.tag = TRANS,
//          .word.trans = {
//              .offset = 1,
//              .Rd = 2,
//              .Rn = 3,
//              .L = true, //load
//              0,
//              .U = true, //add
//              .P = false,//post
//              .I = false,//imm
//              01,
//              .cond = 0}};
//     arm_state = init_state();
//     arm_state->reg[1] = to_bf(1); // offset
//     arm_state->reg[2] = to_bf(0); // Rd
//     arm_state->reg[3] = to_bf(2); // Rn

//     execute_TRANS(&trans_ins7, arm_state);

//     test_int_v(
//         to_int(arm_state->reg[2]),
//         1,
//         "LDR R0, [R1], #12, L = true U = true P = false I = false");

//     free(arm_state);

//     //L = true U = false P = true I = false
//     instruction_t trans_ins8 =
//         {.tag = TRANS,
//          .word.trans = {
//              .offset = 1,
//              .Rd = 2,
//              .Rn = 3,
//              .L = true, //load
//              0,
//              .U = false,//sub
//              .P = true, //pre
//              .I = false,//imm
//              01,
//              .cond = 0}};
//     arm_state = init_state();
//     arm_state->reg[1] = to_bf(1); // offset
//     arm_state->reg[2] = to_bf(0); // Rd
//     arm_state->reg[3] = to_bf(2); // Rn

//     execute_TRANS(&trans_ins8, arm_state);

//     test_int_v(
//         to_int(arm_state->reg[2]),
//         1,
//         "LDR R0, [R1, #12], L = true U = false P = true I = false");

//     free(arm_state);

//     //L = true U = false P = false I = true
//     instruction_t trans_ins9 =
//         {.tag = TRANS,
//          .word.trans = {
//              .offset = 1,
//              .Rd = 2,
//              .Rn = 3,
//              .L = true, //load
//              0,
//              .U = false,//sub
//              .P = false,//post
//              .I = true, //reg
//              01,
//              .cond = 0}};
//     arm_state = init_state();
//     arm_state->reg[1] = to_bf(1); // offset
//     arm_state->reg[2] = to_bf(0); // Rd
//     arm_state->reg[3] = to_bf(2); // Rn

//     execute_TRANS(&trans_ins9, arm_state);

//     test_int_v(
//         to_int(arm_state->reg[2]),
//         1,
//         "LDR R0, [R1], #12, L = true U = false P = false I = true");

//     free(arm_state);

//     //L = false U = true P = true I = false
//     instruction_t trans_ins10 =
//         {.tag = TRANS,
//          .word.trans = {
//              .offset = 1,
//              .Rd = 2,
//              .Rn = 3,
//              .L = false, //store
//              0,
//              .U = true,//add
//              .P = true,//pre
//              .I = false, //imm
//              01,
//              .cond = 0}};
//     arm_state = init_state();
//     arm_state->reg[1] = to_bf(1); // offset
//     arm_state->reg[2] = to_bf(0); // Rd
//     arm_state->reg[3] = to_bf(2); // Rn

//     execute_TRANS(&trans_ins10, arm_state);

//     test_int_v(
//         to_int(arm_state->reg[2]),
//         1,
//         "STR R0, [R1, #12], L = false U = true P = true I = false");

//     free(arm_state);

//     //L = false U = true P = false I = true
//     instruction_t trans_ins11 =
//         {.tag = TRANS,
//          .word.trans = {
//              .offset = 1,
//              .Rd = 2,
//              .Rn = 3,
//              .L = false, //store
//              0,
//              .U = true,//add
//              .P = false,//post
//              .I = true, //reg
//              01,
//              .cond = 0}};
//     arm_state = init_state();
//     arm_state->reg[1] = to_bf(1); // offset
//     arm_state->reg[2] = to_bf(0); // Rd
//     arm_state->reg[3] = to_bf(2); // Rn

//     execute_TRANS(&trans_ins11, arm_state);

//     test_int_v(
//         to_int(arm_state->reg[2]),
//         1,
//         "STR R0, [R1], #12, L = false U = true P = false I = true");

//     free(arm_state);

//     //L = false U = false P = true I = true
//     instruction_t trans_ins12 =
//         {.tag = TRANS,
//          .word.trans = {
//              .offset = 1,
//              .Rd = 2,
//              .Rn = 3,
//              .L = false, //store
//              0,
//              .U = false,//sub
//              .P = true,//pre
//              .I = true, //reg
//              01,
//              .cond = 0}};
//     arm_state = init_state();
//     arm_state->reg[1] = to_bf(1); // offset
//     arm_state->reg[2] = to_bf(0); // Rd
//     arm_state->reg[3] = to_bf(2); // Rn

//     execute_TRANS(&trans_ins12, arm_state);

//     test_int_v(
//         to_int(arm_state->reg[2]),
//         1,
//         "STR R0, [R1, #12], L = false U = false P = true I = true");

//     free(arm_state);

//     //L = true U = true P = true I = false
//     instruction_t trans_ins13 =
//         {.tag = TRANS,
//          .word.trans = {
//              .offset = 1,
//              .Rd = 2,
//              .Rn = 3,
//              .L = true, //load
//              0,
//              .U = true,//add
//              .P = true,//pre
//              .I = false, //reg
//              01,
//              .cond = 0}};
//     arm_state = init_state();
//     arm_state->reg[1] = to_bf(1); // offset
//     arm_state->reg[2] = to_bf(0); // Rd
//     arm_state->reg[3] = to_bf(2); // Rn

//     execute_TRANS(&trans_ins13, arm_state);

//     test_int_v(
//         to_int(arm_state->reg[2]),
//         1,
//         "LDR R0, [R1, #12], L = true U = true P = true I = false");

//     free(arm_state);

//     //L = true U = true P = false I = true
//     instruction_t trans_ins14 =
//         {.tag = TRANS,
//          .word.trans = {
//              .offset = 1,
//              .Rd = 2,
//              .Rn = 3,
//              .L = true, //load
//              0,
//              .U = true,//add
//              .P = false,//post
//              .I = true, //reg
//              01,
//              .cond = 0}};
//     arm_state = init_state();
//     arm_state->reg[1] = to_bf(1); // offset
//     arm_state->reg[2] = to_bf(0); // Rd
//     arm_state->reg[3] = to_bf(2); // Rn

//     execute_TRANS(&trans_ins14, arm_state);

//     test_int_v(
//         to_int(arm_state->reg[2]),
//         1,
//         "LDR R0, [R1], #12, L = true U = true P = false I = true");

//     free(arm_state);

//     //L = true U = false P = true I = true
//     instruction_t trans_ins15 =
//         {.tag = TRANS,
//          .word.trans = {
//              .offset = 1,
//              .Rd = 2,
//              .Rn = 3,
//              .L = true, //load
//              0,
//              .U = false,//sub
//              .P = true,//pre
//              .I = true, //reg
//              01,
//              .cond = 0}};
//     arm_state = init_state();
//     arm_state->reg[1] = to_bf(1); // offset
//     arm_state->reg[2] = to_bf(0); // Rd
//     arm_state->reg[3] = to_bf(2); // Rn

//     execute_TRANS(&trans_ins15, arm_state);

//     test_int_v(
//         to_int(arm_state->reg[2]),
//         1,
//         "LDR R0, [R1, #12], L = true U = false P = true I = true");

//     free(arm_state);

//     //L = false U = true P = true I = true
//     instruction_t trans_ins16 =
//         {.tag = TRANS,
//          .word.trans = {
//              .offset = 1,
//              .Rd = 2,
//              .Rn = 3,
//              .L = false, //store
//              0,
//              .U = true,//add
//              .P = true,//pre
//              .I = true, //reg
//              01,
//              .cond = 0}};
//     arm_state = init_state();
//     arm_state->reg[1] = to_bf(1); // offset
//     arm_state->reg[2] = to_bf(0); // Rd
//     arm_state->reg[3] = to_bf(2); // Rn

//     execute_TRANS(&trans_ins16, arm_state);

//     test_int_v(
//         to_int(arm_state->reg[2]),
//         1,
//         "STR R0, [R1, #12], L = false U = true P = true I = true");

//     free(arm_state);

//     //L = true U = true P = true I = true
//     instruction_t trans_ins17 =
//         {.tag = TRANS,
//          .word.trans = {
//              .offset = 1,
//              .Rd = 2,
//              .Rn = 3,
//              .L = true, //load
//              0,
//              .U = true,//add
//              .P = true,//pre
//              .I = true, //reg
//              01,
//              .cond = 0}};
//     arm_state = init_state();
//     arm_state->reg[1] = to_bf(1); // offset
//     arm_state->reg[2] = to_bf(0); // Rd
//     arm_state->reg[3] = to_bf(2); // Rn

//     execute_TRANS(&trans_ins17, arm_state);

//     test_int_v(
//         to_int(arm_state->reg[2]),
//         1,
//         "LDR R0, [R1, #12], L = true U = true P = true I = true");

//     free(arm_state);

//   }

//   add_test("Test for Branch execution");
//   {
//     instruction_t branch_ins1 =
//         {.tag = BRANCH,
//          .word.branch = {
//              .offset = 1,
//              1010,
//              .cond = 0}};
//     arm_state = init_state();
//     execute_BRANCH(&branch_ins1, arm_state);

//     //condition is set
//     instruction_t branch_ins2 =
//         {.tag = BRANCH,
//          .word.branch = {
//              .offset = 1,
//              1010,
//              .cond = 1}};
//     arm_state = init_state();
//     execute_BRANCH(&branch_ins2, arm_state);
//   }

//   end_all_tests();

//   return 0;
// }

// ArmState init_state()
// {
//   ArmState result = (ArmState)malloc(sizeof(arm_state_struct));
//   if (result == NULL)
//   {
//     return NULL;
//   }

//   result->reg = calloc(NUM_OF_REG, sizeof(bitfield));
//   result->memory = calloc(MAX_MEMORY_ADDRESS, sizeof(byte));
//   if (result->reg == NULL || result->memory == NULL)
//   {
//     return NULL;
//   }

//   result->pc = 0;
//   result->flagN = false;
//   result->flagZ = false;
//   result->flagC = false;
//   result->flagV = false;

//   return result;
// }