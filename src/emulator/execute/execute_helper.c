#include <math.h>

#include "../utils/types_and_macros.h"
#include "execute_helper.h"
#include "../utils/tools.h"

uint32_t rotate(int rotation_amount, uint32_t content)
{
  int af_rot_val = 0;
  for (int i = 0; i < rotation_amount; i++)
  {
    if (get_bit(content, i) == 1)
    {
      af_rot_val += pow(2, (31 - i));
    }
  }
  return af_rot_val + (content >> rotation_amount);
}

uint32_t shift(uint32_t Rm, int shift_val, shift_type type)
{
    switch (type)
  {
  case LSL:
  {
    return Rm << shift_val;
  }
  case LSR:
  {
    return Rm >> shift_val;
  }
  case ASR:
  {
    return arith_right(shift_val, Rm);
  }
  case ROR:
  {
    return rotate(shift_val, Rm);
  }
  default:
    break;
  }
}

uint32_t arith_right(uint32_t shift_val, uint32_t Rm)
{
  uint32_t after_shift = Rm >> shift_val;
  uint32_t mask = 0;
  bool sign_bit = get_bit(Rm, 31);
  if (sign_bit)
  {
    for (int i = 31; i >= 32 - shift_val; i--)
    {
      mask += pow(2, i);
    }
  }
  return after_shift | mask;
}

bool test_instruction_cond(instruction_t instruction, ArmState arm_state)
{
  bool N = arm_state->flagN;
  bool Z = arm_state->flagZ;
  bool C = arm_state->flagC;
  bool V = arm_state->flagV;
  switch (instruction.word.dp.cond) // cond is at the same position in all cases
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

uint32_t shift_imm_handle(bitfield * reg, reg_or_imm_t shift_or_imm, bool is_imm)
{

   if (is_imm) //Offset is a register.
  {
    shift_reg_t offset_reg = shift_or_imm.shift_reg;
    int shift_val = to_int(reg[offset_reg.shift.val]);
    uint32_t Rm = to_int(reg[offset_reg.Rm]);
    shift_type type = to_int(reg[offset_reg.shift.val]);

    return shift(Rm, shift_val, type);
  }
  else // Offset is an immediate value.
  {
    rotate_t offset_imm = shift_or_imm.imm_val;
    int rotation_amount = 2 * to_int(reg[offset_imm.amount]);
    uint32_t Imm = to_int(reg[offset_imm.value]);

    return rotate(rotation_amount, Imm);
  }
}
