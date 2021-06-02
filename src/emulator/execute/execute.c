#include <stdlib.h>
#include <math.h>

#include "../utils/types_and_macros.h"

#include "execute.h"

#include "../utils/tools.h"
#include "../execute/execute_DP.h"

bool execute(instruction_t *decoded, ArmState arm_state)
{
  if (test_instruction_cond(decoded, arm_state))
  {
    switch (decoded->tag)
    {
    case DATA_PROCESS:
      execute_DP(decoded, arm_state);
      return CONTINUE;
    case MUL:
      execute_MUL(decoded, arm_state);
      return CONTINUE;
    case TRANS:
      execute_SDT(decoded, arm_state);
      return CONTINUE;
    case BRANCH:
      execute_BRANCH(decoded, arm_state);
      return CONTINUE;
    case ZERO:
      return EXIT;
    case UNDEFINED:
      return ERROR;
    default:
      return ERROR;
    }
  }
}

void execute_MUL(instruction_t *decoded, ArmState arm_state)
{
  // pre: PC is not used as operand or desination register
  //      Rd will not be the same as Rm
  bitfield *reg = arm_state->reg;
  mul_t mul_ins = decoded->u.mul;
  uint32_t Rm = to_int(reg[mul_ins.Rm]);
  uint32_t Rs = to_int(reg[mul_ins.Rs]);
  uint32_t Rn = to_int(reg[mul_ins.Rn]);
  uint32_t result = Rm * Rs;

  // the accumulate bit is set
  if (mul_ins.A)
  {
    result += Rn;
  }
  // Save the result
  reg[mul_ins.Rd] = to_bf(result);

  // If the S bit is set, we need to update the CPSR
  if (mul_ins.S)
  {
    arm_state->flagN = get_bit(result, 31);
    arm_state->flagZ = result == 0;
  }
}

void execute_SDT(instruction_t *decoded, ArmState arm_state)
{
  uint32_t result;
  trans_t trans_ins = decoded->u.trans;
  bitfield *reg = arm_state->reg;

  if (trans_ins.I) // offset is a register.
  {
    int shift_val = to_int(reg[trans_ins.offset.shift_reg.shift.integer]);
    uint32_t rm = to_int(reg[trans_ins.offset.shift_reg.Rm]);
    trans_ins.offset.offset_value

        switch (to_int(reg[trans_ins.offset.shift_reg.shift.ShiftT]))
    {
    case LSL: //logical left
    {
      reg[trans_ins.offset.offset_value] = to_bf(rm >> shift_val);
      break;
    }
    case LSR: //logical right
    {
      reg[trans_ins.offset.offset_value] = to_bf(rm << shift_val);
      break;
    }
    case ASR: //arithmetic right
    {
      uint32_t after_shift = rm << shift_val;
      int sign_bit = get_bit(rm, 31);
      uint32_t mask = 0;
      for (int i = 31; i >= 32 - shift_val; i--)
      {
        mask += pow(sign_bit, i);
      }
      reg[trans_ins.offset.offset_value] = to_bf(after_shift | mask);
      break;
    }
    case ROR: //rotate right
    {
      int af_rot_val = 0;
      for (int i = 0; i < shift_val; i++)
      {
        af_rot_val += pow(get_bit(rm, i), (31 - i));
      }
      reg[trans_ins.offset.offset_value] = to_bf(af_rot_val + (rm << shift_val));
      break;
    }
    default:
      break;
    }
  }
  else //Offset is an immediate offset.
  {
    int rotation_amount = 2 * to_int(reg[trans_ins.offset.imm_offset.rot_amt]);
    uint32_t Imm = to_int(reg[trans_ins.offset.imm_offset.rot_val]);
    int af_rot_val = 0;
    for (int i = 0; i < rotation_amount; i++)
    {
      af_rot_val += pow(get_bit(Imm, i), (31 - i));
    }
    reg[trans_ins.offset.imm_offset.rot_val] = to_bf(af_rot_val + (Imm << rotation_amount));
  }

  uint32_t Rn = to_int(reg[trans_ins.Rn]);
  uint32_t offset = to_int(reg[trans_ins.offset.offset_value]);

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
      result = to_int(reg[trans_ins.Rn]) + to_int(reg[trans_ins.offset.offset_value]);
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

void execute_BRANCH(instruction_t *decoded, ArmState arm_state)
{
  uint32_t offset = to_int(arm_state->reg[decoded->u.branch.offset]);
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
