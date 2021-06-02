#include "../utils/types_and_macros.h"
#include "execute_SDT.h"
#include "../utils/tools.h"
#include "../execute/execute_helper.h"

void execute_SDT(instruction_t *decoded, ArmState arm_state)
{
  uint32_t result;
  trans_t trans_ins = decoded->u.trans;
  bitfield *reg = arm_state->reg;

  uint32_t Rn = to_int(reg[trans_ins.Rn]);
  uint32_t offset = shift_imm_handle(reg, trans_ins.offset, trans_ins.I);

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


