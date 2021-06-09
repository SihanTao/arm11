#include "execute_SDT.h"
#include "../execute/execute_helper.h"
#include "../utils/tools.h"
#include "../utils/types_and_macros.h"

void execute_TRANS (trans_t instruction, ArmState arm_state)
{
  // uint32_t result;
  // bitfield *reg = arm_state->reg;

  // uint32_t Rn = to_int(reg[instruction.Rn]);
  // uint32_t offset = reg_imm_handle(reg, instruction.offset,
  // instruction.is_imm);

  // //if is_up is set then offset is added to Rn. Otherwise the offset is
  // subtracted from Rn. Rn = (instruction.is_up) ? Rn + offset : Rn - offset;

  // //If is_load is set, the word is loaded from memory, otherwise the word is
  // stored into memory. if (instruction.is_load)
  // {
  //   result = Rn;
  // }
  // else
  // {
  //   reg[instruction.Rd] = to_bf(Rn);
  // }

  // if (instruction.is_pre) //pre-indexing, the offset is added/subtracted to
  // the base register before transferring the data.
  // {
  //   uint32_t newRn = (instruction.is_up) ? Rn + offset : Rn - offset;

  //   switch (instruction.is_pre)
  //   {
  //   case LOAD:
  //     result = to_int(reg[instruction.Rn]) + to_int(reg[offset]);
  //     break;

  //   case STORE:
  //     reg[instruction.Rd] = to_bf(newRn);
  //     break;

  //   default:
  //     break;
  //   }
  //   Rn = (instruction.is_up) ? Rn + offset : Rn - offset;
  // }
  // else //(post-indexing, the offset is added/subtracted to the base register
  // after transferring.
  // {
  //   switch (instruction.is_pre)
  //   {
  //   case LOAD:
  //     result = Rn;
  //     break;

  //   case STORE:
  //     reg[instruction.Rd] = to_bf(Rn);
  //     break;

  //   default:
  //     break;
  //   }
  //   Rn = (instruction.is_up) ? Rn + offset : Rn - offset;
  // }
}
