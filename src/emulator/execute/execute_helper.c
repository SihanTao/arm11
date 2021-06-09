#include <stdio.h>

#include "../utils/tools.h"
#include "../utils/types_and_macros.h"
#include "execute_helper.h"

value_carry_t rotate(uint32_t target, int rotate_amount)
{
  value_carry_t result;
  result.value
      = (target << (WORD_SIZE - rotate_amount)) | (target >> rotate_amount);
  result.carry = get_bit(target, rotate_amount - 1);
  return result;
}

value_carry_t shift(uint32_t target, int shift_amount, shift_type type)
{
  if (shift_amount < 1 || shift_amount >= WORD_SIZE)
    {
      perror("shift_amount should in between 0 - 31");
      exit(EXIT_FAILURE);
    }

  value_carry_t result;
  result.carry = get_bit(target, shift_amount - 1);
  // execept LSL, other carry is defined as shift_amount - 1 bit of target

  switch (type)
    {
    case LSL:
      {
        result.value = target << shift_amount;
        result.carry = get_bit(target, WORD_SIZE - shift_amount);
        return result;
      }
    case LSR:
      {
        result.value = target >> shift_amount;
        return result;
      }
    case ASR:
      {
        // arith_right, the most significant nth bits in result
        // are the same as 31th bit in t
        uint32_t head
            = get_bit(target, 31) ? ALL_ONE << (WORD_SIZE - shift_amount) : 0;
        result.value = target >> shift_amount | head;
        return result;
      }
    case ROR:
      {
        return rotate(target, shift_amount);
      }
    default:
      break;
    }
}

bool test_instruction_cond(instruction_t instruction, ArmState arm_state)
{
  bool N = arm_state->neg;
  bool Z = arm_state->zero;
  bool C = arm_state->carry;
  bool V = arm_state->ovflw;
  switch (
      instruction.word.proc.cond) // cond is at the same position in all cases
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

value_carry_t reg_imm_handle(bitfield * reg, reg_or_imm_t reg_imm, bool is_imm)
{
  if (is_imm)
    {
      return shift(to_int(reg[reg_imm.shift_reg.Rm]), reg_imm.shift_reg.val,
                   reg_imm.shift_reg.type);
    }
  else
    {
      return rotate(reg_imm.rot_imm.imm, 2 * reg_imm.rot_imm.amount);
    }
}
