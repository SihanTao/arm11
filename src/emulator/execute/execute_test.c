#include <stdlib.h>
#include "../utils/types_and_macros.h"
#include "execute.h"
#include "../utils/tools.h"
#include "../utils/unit_test.h"

static ArmState init_state();

int main(void)
{
  ArmState arm_state = init_state();

  //add_test("Test for data processing execution");
  //{
  //}

  add_test("Test for Mul execution");
  {
    // S = true A = false
    instruction_t mul_ins1 =
        {.tag = MUL,
         .u.mul = {
             .Rm = 1,
             1001,
             .Rs = 2,
             .Rn = 3,
             .Rd = 4,
             .S = true,
             .A = false,
             0,
             .cond = 0}};

    arm_state->reg[1] = to_bf(0xFFFFFFFD); // Rm
    arm_state->reg[2] = to_bf(0xF);        // Rs
    arm_state->reg[4] = to_bf(0);          // Rd

    execute_MUL(&mul_ins1, arm_state);

    test_int_v(
        to_int(arm_state->reg[4]),
        0xEFFFFFFD3,
        "mul 0xFFFFFFFFD 0xF 0, S true A false");

    // N is set to bit 31 of the result
    test_true( arm_state->flagN == get_bit(0xFFFFFFD3, 31));

    // Z is set if and only if the result is zero.
    test_true(arm_state->flagZ == (to_int(arm_state->reg[4]) == 0));

    free(arm_state);

    // S = true A = flase condition is set
    instruction_t mul_ins2 =
        {.tag = MUL,
         .u.mul = {
             .Rm = 1,
             1001,
             .Rs = 2,
             .Rn = 3,
             .Rd = 4,
             .S = true,
             .A = false,
             01,
             .cond = 1}};
    arm_state = init_state();
    arm_state->reg[1] = to_bf(0xFFFFFFFD); // Rm
    arm_state->reg[2] = to_bf(0xF);        // Rs
    arm_state->reg[4] = to_bf(0);          // Rd

    execute_MUL(&mul_ins1, arm_state);

    test_int_v(
        to_int(arm_state->reg[4]),
        0xEFFFFFFD3,
        "mul 0xFFFFFFFFD 0xF 0, S true A false");

    // N is set to bit 31 of the result
    test_true( arm_state->flagN == get_bit(0xFFFFFFD3, 31));

    // Z is set if and only if the result is zero.
    test_true(arm_state->flagZ == (to_int(arm_state->reg[4]) == 0));

    free(arm_state);

    // S = true A = true
    instruction_t mul_ins3 =
        {.tag = MUL,
         .u.mul = {
             .Rm = 1,
             1001,
             .Rs = 2,
             .Rn = 3,
             .Rd = 4,
             .S = true,
             .A = true,
             0,
             .cond = 0}};
    arm_state = init_state();
    arm_state->reg[1] = to_bf(1); // Rm
    arm_state->reg[2] = to_bf(2); // Rs
    arm_state->reg[3] = to_bf(3); // Rn
    arm_state->reg[4] = to_bf(0); // Rd

    execute_MUL(&mul_ins1, arm_state);

    test_int_v(
        to_int(arm_state->reg[4]),
        0xEFFFFFFD3,
        "mul 1 2 3 4, S true A true");

    // N is set to bit 31 of the result
    test_true( arm_state->flagN == get_bit(0xFFFFFFD3, 31));

    // Z is set if and only if the result is zero.
    test_true(arm_state->flagZ == (to_int(arm_state->reg[4]) == 0));

    free(arm_state);

    // S = false A = true
    instruction_t mul_ins4 =
        {.tag = MUL,
         .u.mul = {
             .Rm = 1,
             1001,
             .Rs = 2,
             .Rn = 3,
             .Rd = 4,
             .S = false,
             .A = true,
             0,
             .cond = 0}};
    arm_state = init_state();
    arm_state->reg[1] = to_bf(1); // Rm
    arm_state->reg[2] = to_bf(2); // Rs
    arm_state->reg[3] = to_bf(3); // Rn
    arm_state->reg[4] = to_bf(0); // Rd

    execute_MUL(&mul_ins1, arm_state);

    test_int_v(
        to_int(arm_state->reg[4]),
        0xEFFFFFFD3,
        "mul 1 2 3 4, S false A true");

    free(arm_state);

    // S = false A = flase
    instruction_t mul_ins5 =
        {.tag = MUL,
         .u.mul = {
             .Rm = 1,
             1001,
             .Rs = 2,
             .Rn = 3,
             .Rd = 4,
             .S = false,
             .A = false,
             0,
             .cond = 0}};
    arm_state = init_state();
    arm_state->reg[1] = to_bf(0xFFFFFFFD); // Rm
    arm_state->reg[2] = to_bf(0xF);        // Rs
    arm_state->reg[4] = to_bf(0);          // Rd

    execute_MUL(&mul_ins1, arm_state);

    test_int_v(
        to_int(arm_state->reg[4]),
        0xEFFFFFFD3,
        "mul 0xFFFFFFFFD 0xF 0, S false A false");

    free(arm_state);
   }

  add_test("Test for Trans execution");
  {
    /* STR R0, [R1, #12]
     * LDR R0, [R1, #12]
     * STR R0, [R1], #12
     * LDR R0, [R1], #12
     */

    instruction_t trans_ins1 =
        {.tag = TRANS,
         .u.trans = {
             .offset = 1,
             .Rd = 2,
             .Rn = 3,
             .L = false,
             0,
             .U = false,
             .P = false,
             .I = false,
             01,
             .cond = 0}};
    arm_state = init_state();
    arm_state->reg[1] = to_bf(12); // offset
    arm_state->reg[2] = to_bf(0); // Rd
    arm_state->reg[3] = to_bf(1); // Rn

    execute_SDT(&trans_ins1, arm_state);

    test_int_v(
        to_int(arm_state->reg[2]),
        1,
        "STR R0, [R1, #12], all false");

    free(arm_state);

    //condition is set
    instruction_t trans_ins2 =
        {.tag = TRANS,
         .u.trans = {
             .offset = 1,
             .Rd = 2,
             .Rn = 3,
             .L = false,
             0,
             .U = false,
             .P = false,
             .I = false,
             01,
             .cond = 1}};
    arm_state = init_state();
    arm_state->reg[1] = to_bf(1); // offset
    arm_state->reg[2] = to_bf(0); // Rd
    arm_state->reg[3] = to_bf(2); // Rn

    execute_SDT(&trans_ins2, arm_state);

    test_int_v(
        to_int(arm_state->reg[2]),
        1,
        "STR R0, [R1, #12], all false");

    free(arm_state);

    //L = true U = false P = false I = false
    instruction_t trans_ins3 =
        {.tag = TRANS,
         .u.trans = {
             .offset = 1,
             .Rd = 2,
             .Rn = 3,
             .L = true,
             0,
             .U = false,
             .P = false,
             .I = false,
             01,
             .cond = 0}};
    arm_state = init_state();
    arm_state->reg[1] = to_bf(1); // offset
    arm_state->reg[2] = to_bf(0); // Rd
    arm_state->reg[3] = to_bf(2); // Rn

    execute_SDT(&trans_ins3, arm_state);

    test_int_v(
        to_int(arm_state->reg[2]),
        1,
        "LDR R0, [R1, #12], L = true U = false P = false I = false");

    free(arm_state);

    //L = false U = true P = false I = false
    instruction_t trans_ins4 =
        {.tag = TRANS,
         .u.trans = {
             .offset = 1,
             .Rd = 2,
             .Rn = 3,
             .L = false,
             0,
             .U = true,
             .P = false,
             .I = false,
             01,
             .cond = 0}};
    arm_state = init_state();
    arm_state->reg[1] = to_bf(1); // offset
    arm_state->reg[2] = to_bf(0); // Rd
    arm_state->reg[3] = to_bf(2); // Rn

    execute_SDT(&trans_ins4, arm_state);

    test_int_v(
        to_int(arm_state->reg[2]),
        1,
        "LDR R0, [R1, #12], L = false U = true P = false I = false");

    free(arm_state);

    //L = false U = false P = true I = false
    instruction_t trans_ins5 =
        {.tag = TRANS,
         .u.trans = {
             .offset = 1,
             .Rd = 2,
             .Rn = 3,
             .L = false,
             0,
             .U = false,
             .P = true,
             .I = false,
             01,
             .cond = 0}};
    arm_state = init_state();
    arm_state->reg[1] = to_bf(1); // offset
    arm_state->reg[2] = to_bf(0); // Rd
    arm_state->reg[3] = to_bf(2); // Rn

    execute_SDT(&trans_ins5, arm_state);

    test_int_v(
        to_int(arm_state->reg[2]),
        1,
        "STR R0, [R1], #12, L = false U = false P = true I = false");

    free(arm_state);

    //L = false U = false P = false I = true
    instruction_t trans_ins6 =
        {.tag = TRANS,
         .u.trans = {
             .offset = 1,
             .Rd = 2,
             .Rn = 3,
             .L = false,
             0,
             .U = false,
             .P = false,
             .I = true,
             01,
             .cond = 0}};
    arm_state = init_state();
    arm_state->reg[1] = to_bf(1); // offset
    arm_state->reg[2] = to_bf(0); // Rd
    arm_state->reg[3] = to_bf(2); // Rn

    execute_SDT(&trans_ins6, arm_state);

    test_int_v(
        to_int(arm_state->reg[2]),
        1,
        "STR R0, [R1], #12, L = false U = false P = false I = true");

    free(arm_state);

    //L = true U = true P = false I = false
    instruction_t trans_ins7 =
        {.tag = TRANS,
         .u.trans = {
             .offset = 1,
             .Rd = 2,
             .Rn = 3,
             .L = true,
             0,
             .U = true,
             .P = false,
             .I = false,
             01,
             .cond = 0}};
    arm_state = init_state();
    arm_state->reg[1] = to_bf(1); // offset
    arm_state->reg[2] = to_bf(0); // Rd
    arm_state->reg[3] = to_bf(2); // Rn

    execute_SDT(&trans_ins7, arm_state);

    test_int_v(
        to_int(arm_state->reg[2]),
        1,
        "LDR R0, [R1], #12, L = true U = true P = false I = false");

    free(arm_state);

    //L = true U = false P = true I = false
    instruction_t trans_ins8 =
        {.tag = TRANS,
         .u.trans = {
             .offset = 1,
             .Rd = 2,
             .Rn = 3,
             .L = true,
             0,
             .U = false,
             .P = true,
             .I = false,
             01,
             .cond = 0}};
    arm_state = init_state();
    arm_state->reg[1] = to_bf(1); // offset
    arm_state->reg[2] = to_bf(0); // Rd
    arm_state->reg[3] = to_bf(2); // Rn

    execute_SDT(&trans_ins8, arm_state);

    test_int_v(
        to_int(arm_state->reg[2]),
        1,
        "LDR R0, [R1], #12, L = true U = false P = true I = false");

    free(arm_state);

    //L = true U = false P = false I = true
    instruction_t trans_ins9 =
        {.tag = TRANS,
         .u.trans = {
             .offset = 1,
             .Rd = 2,
             .Rn = 3,
             .L = true,
             0,
             .U = false,
             .P = false,
             .I = true,
             01,
             .cond = 0}};
    arm_state = init_state();
    arm_state->reg[1] = to_bf(1); // offset
    arm_state->reg[2] = to_bf(0); // Rd
    arm_state->reg[3] = to_bf(2); // Rn

    execute_SDT(&trans_ins9, arm_state);

    test_int_v(
        to_int(arm_state->reg[2]),
        1,
        "LDR R0, [R1], #12, L = true U = false P = false I = true");

    free(arm_state);

  }

  add_test("Test for Branch execution");
  {
    instruction_t branch_ins1 =
        {.tag = BRANCH,
         .u.branch = {
             .offset = 1,
             1010,
             .cond = 0}};
    arm_state = init_state();
    execute_BRANCH(&branch_ins1, arm_state);

    //condition is set
    instruction_t branch_ins2 =
        {.tag = BRANCH,
         .u.branch = {
             .offset = 1,
             1010,
             .cond = 1}};
    arm_state = init_state();
    execute_BRANCH(&branch_ins2, arm_state);
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
  result->flagN = false;
  result->flagZ = false;
  result->flagC = false;
  result->flagV = false;

  return result;
}