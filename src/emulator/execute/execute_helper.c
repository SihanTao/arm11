#include <math.h>

#include "../utils/types_and_macros.h"
#include "execute_helper.h"
#include "../utils/tools.h"

uint32_t rotate(uint32_t target, int rotate_amount)
{
  int result = 0;
  for (int i = 0; i < rotate_amount; i++)
  {
    if (get_bit(target, i) == 1)
    {
      result += pow(2, (31 - i));
    }
  }
  return result + (target >> rotate_amount);
}

uint32_t shift(uint32_t target, int shift_amount, shift_type type)
{
  switch (type)
  {
  case LSL:
  {
    return target << shift_amount;
  }
  case LSR:
  {
    return target >> shift_amount;
  }
  case ASR:
  {
    return arith_right(shift_amount, target);
  }
  case ROR:
  {
    return rotate(shift_amount, target);
  }
  default:
    break;
  }
}


// TODO : not sure if this is correct or not
uint32_t arith_right(uint32_t shift_amount, uint32_t target)
{
  uint32_t result = target >> shift_amount;
  uint32_t mask = 0;
  bool sign_bit = get_bit(target, 31);
  if (sign_bit)
  {
    for (int i = 31; i >= 32 - shift_amount; i--)
    {
      mask += pow(2, i);
    }
  }
  return result | mask;
}

bool test_instruction_cond(instruction_t instruction, ArmState arm_state)
{
  bool N = arm_state->neg;
  bool Z = arm_state->zero;
  bool C = arm_state->carry;
  bool V = arm_state->ovflw;
  switch (instruction.word.proc.cond) // cond is at the same position in all cases
  {
  case EQ:
    return (N);
  case NE:
    return (!N);
  case GE:
    return (N == V);
  case LT:
    return (N != V);
  case GT:
    return (!N && N == V);
  case LE:
    return (N || N != V);
  case AL:
    return true;
  default:
    break;
  }
}

uint32_t reg_imm_handle(bitfield * reg, reg_or_imm_t reg_imm, bool is_imm)
{
  if (is_imm)
  {
    return shift(
      to_int(reg[reg_imm.shift_reg.Rm]),
      reg_imm.shift_reg.val,
      reg_imm.shift_reg.type
    );
  }
  else
  {
    return rotate(
      2 * reg_imm.rot_imm.amount,
      reg_imm.rot_imm.imm
    );
  }
}
