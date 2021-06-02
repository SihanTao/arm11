#include "../utils/types_and_macros.h"
#include "execute_SDT.h"
#include "../utils/tools.h"
#include "execute_helper.h"

void execute_SDT(instruction_t *decoded, ArmState arm_state)
{
  uint32_t result;
  trans_t trans_ins = decoded->u.trans;
  bitfield *reg = arm_state->reg;

  uint32_t Rn = to_int(reg[trans_ins.Rn]);
  uint32_t offset = op2_carry(reg, trans_ins);

  //if U is set then offset is added to Rn. Otherwise the offset is subtracted from Rn.
  Rn = (trans_ins.U) ? Rn + offset : Rn - offset;

  //If L is set, the word is loaded from memory, otherwise the word is stored into memory.
  if (trans_ins.L)
  {
    result = Rn;
  }
  else
  {
    reg[trans_ins.Rd] = to_bf(Rn);
  }
  
  if (trans_ins.P) //pre-indexing, the offset is added/subtracted to the base register before transferring the data.
  {
    uint32_t newRn = (trans_ins.U) ? Rn + offset : Rn - offset;

    switch (trans_ins.P)
    {
    case LOAD:
      result = to_int(reg[trans_ins.Rn]) + to_int(reg[offset]);
      break;

    case STORE:
      reg[trans_ins.Rd] = to_bf(newRn);
      break;

    default:
      break;
    }
    Rn = (trans_ins.U) ? Rn + offset : Rn - offset;
  }
  else //(post-indexing, the offset is added/subtracted to the base register after transferring.
  {
    switch (trans_ins.P)
    {
    case LOAD:
      result = Rn;
      break;

    case STORE:
      reg[trans_ins.Rd] = to_bf(Rn);
      break;

    default:
      break;
    }
    Rn = (trans_ins.U) ? Rn + offset : Rn - offset;
  }
}

uint32_t op2_carry(bitfield * reg, trans_t trans_ins)
{ 

   if (trans_ins.I) //Offset is a register.
  {
    shift_reg_t offset_reg = trans_ins.offset.shift_reg;
    int shift_val = to_int(reg[offset_reg.shift.integer]);
    uint32_t Rm = to_int(reg[offset_reg.Rm]);
    shift_type type = to_int(reg[offset_reg.shift.integer]);

    return shift(Rm, shift_val, type);
  }
  else // Offset is an immediate value.
  {
    rotate_t offset_imm = trans_ins.offset.imm_offset;
    int rotation_amount = 2 * to_int(reg[offset_imm.rot_amt]);
    uint32_t Imm = to_int(reg[offset_imm.rot_val]);

    return rotate(rotation_amount, Imm);
  }
}  
