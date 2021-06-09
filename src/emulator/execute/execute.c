#include <stdlib.h>
#include <math.h>

#include "../utils/types_and_macros.h"

#include "execute.h"

#include "../utils/tools.h"
#include "../execute/execute_helper.h"
#include "../execute/execute_DP.h"
#include "../execute/execute_SDT.h"


exit_type execute(instruction_t decoded, ArmState arm_state)
{
  if (test_instruction_cond(decoded, arm_state))
  {
    switch (decoded.tag)
    {
    case DATA_PROCESS:
      execute_DP(decoded.word.proc, arm_state);
      return CONTINUE;
    case MUL:
      execute_MUL(decoded.word.mul, arm_state);
      return CONTINUE;
    case TRANS:
      execute_TRANS(decoded.word.trans, arm_state);
      return CONTINUE;
    case BRANCH:
      execute_BRANCH(decoded.word.branch, arm_state);
      return CONTINUE;
    case ZERO:
      return EXIT;
    default:
      return ERROR;
    }
  }
}

void execute_MUL(mul_t instruction, ArmState arm_state)
{
  // pre: PC is not used as operand or destination register
  //      Rd will not be the same as Rm
  bitfield *reg = arm_state->reg;
  uint32_t Rm = to_int(reg[instruction.Rm]);
  uint32_t Rs = to_int(reg[instruction.Rs]);
  uint32_t Rn = to_int(reg[instruction.Rn]);
  uint32_t result = Rm * Rs;

  // the accumulate bit is set
  if (instruction.acc)
  {
    result += Rn;
  }
  // Save the result
  reg[instruction.Rd] = to_bf(result);

  // If the set_cond bit is set, we need to update the CPSR
  if (instruction.set_cond)
  {
    arm_state->neg = get_bit(result, 31);
    arm_state->zero = result == 0;
  }
}

void execute_BRANCH(branch_t instruction, ArmState arm_state)
{
  uint32_t offset = to_int(arm_state->reg[instruction.offset]);
  int sign_bit = get_bit(offset, 24);
  uint32_t mask = 0;
  uint32_t extended = 0;

  // The offset will take into account the effect of the pipeline (i.e. PC is 8 bytes ahead of the
  // instruction that is being executed).
  // I think it is kinda hard to write test for this branch instruction.
  // since it is closely related to pc.

  offset <<= 2;

  if (sign_bit == 0)
  {
    mask = ~(2 ^ 25 + 2 ^ 26);
    extended = offset & mask;
  }
  else
  {
    for (int i = 31; i > 24; i--)
    {
      mask += 2 ^ i;
    }
    extended = offset | mask;
  }
  arm_state->pc += extended;
}


