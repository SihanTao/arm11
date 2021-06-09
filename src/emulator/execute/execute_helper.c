#include <stdio.h>

#include "../utils/types_and_macros.h"

#include "execute_helper.h"

#include "../utils/tools.h"

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

  // execept LSL, other carry is defined as shift_amount - 1 bit of target
  result.carry = get_bit(target, shift_amount - 1);

  switch (type)
  {
  case LSL: // logical shift left
  {
    result.value = target << shift_amount;
    result.carry = get_bit(target, WORD_SIZE - shift_amount);
    return result;
  }
  case LSR: // logical shift right
  {
    result.value = target >> shift_amount;
    return result;
  }
  case ASR: // arithmetic shift right
  {
    // arith_right, the most significant nth bits in result
    // are the same as 31th bit in t
    uint32_t head
        = get_bit(target, 31) ? ALL_ONE << (WORD_SIZE - shift_amount) : 0;
    result.value = target >> shift_amount | head;
    return result;
  }
  case ROR: // rotate right
  {
    return rotate(target, shift_amount);
  }
  default:
    perror("Internal error! unknown shift type!");
    exit(EXIT_FAILURE);
    ;
  }
}

bool test_instruction_cond(instruction_t instruction, ArmState arm_state)
{
  bool N = arm_state->neg;
  bool V = arm_state->ovflw;
  // cond is at the same position in all cases
  switch (instruction.word.proc.cond)
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
    perror("Internal error! unknown cond type!");
    exit(EXIT_FAILURE);
  }
}

/*
 * dispatcher for shift and rotate
 * value_out and carry are the output params
 * if carry is setted to NULL, this function will not change carry
 */
void reg_imm_handle(bitfield *reg, reg_or_imm_t reg_imm, bool is_imm,
                    uint32_t *value_out, bool *carry)
{
  value_carry_t result;
  if (is_imm)
  {
    result = shift(to_int(reg[reg_imm.shift_reg.Rm]), reg_imm.shift_reg.val,
                   reg_imm.shift_reg.type);
  }
  else
  {
    result = rotate(reg_imm.rot_imm.imm, 2 * reg_imm.rot_imm.amount);
  }

  *value_out = result.value;
  if (carry != NULL)
  {
    *carry = result.carry;
  }
}
