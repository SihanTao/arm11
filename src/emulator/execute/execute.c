#include <stdlib.h>
#include <math.h>

#include "../utils/types_and_macros.h"
#include "../utils/tools.h"

#include "execute.h"

// TODO Here is an spelling error : decode -> decoded
// I will solve this after fix all other problems --Tony
bool execute(instruction_t *decoded, ArmState armstate)
{
  if (test_instruction_cond(decoded, armstate))
  {
    switch (decoded->tag)
    {
    case DATA_PROCESS:
      execute_DP(decoded, armstate);
      break;
    case MUL:
      execute_MUL(decoded, armstate);
      break;
    case TRANS:
      execute_SDT(decoded, armstate);
      break;
    case BRANCH:
      execute_BRANCH(decoded, armstate);
      break;
    case ZERO:
      execute_ZERO(decoded, armstate);
      // Intentionally no break here
    case UNDEFINED:
      // should handle raise an exception or something, I will try to figure out
      // Intentionally no break here
    default:
      return EXIT_FAILURE;
      // TODO: This is problematic!
      // This program should return either #EXIT or #CONTINUE
    }
  }
}

void execute_MUL(instruction_t *decoded, ArmState armstate)
{
  // pre: PC is not used as operand or desination register
  //      Rd will not be the same as Rm
  bitfield *reg = armstate->reg;
  mul mul_ins = decoded->u.mul;
  bitfield Rm = reg[mul_ins.Rm];
  bitfield Rs = reg[mul_ins.Rs];
  bitfield Rn = reg[mul_ins.Rn];

  uint32_t result = to_int(Rm) * to_int(Rs);

  // the accumulate bit is set
  if (mul_ins.A)
  {
    result += to_int(Rn);
  }
  // Save the result
  reg[mul_ins.Rd] = to_bf(result);

  // If the S bit is set, we need to update the CPSR
  if (mul_ins.S)
  {
    armstate->flagN = get_bit(result, 31);
    armstate->flagZ = result == 0;
  }
}

void execute_DP(instruction_t *decoded, ArmState armstate)
{
  uint32_t result;

  int Change_FlagC = 0; // the carry out bit need to be store in FlagC.
  data_process data_ins = decoded->u.data_process;
  bitfield * reg = armstate->reg;

  if (data_ins.I) // OP2 is an immediate value.
  {
    int rotation_amount = 2 * to_int(reg[data_ins.operand2.Iv.Rotate]);
    uint32_t Imm = to_int(reg[data_ins.operand2.Iv.Imm]);
    // TODO : extract this as a separate function.
    int af_rot_val = 0;
    for (int i = 0; i < rotation_amount; i++)
    {
      af_rot_val += pow(get_bit(Imm, i), (31 - i));
    }
    // TODO : I don't think this will work, since Imm has length 8
    reg[data_ins.operand2.Iv.Imm] = to_bf(af_rot_val + (Imm << rotation_amount));
    Change_FlagC = get_bit(Imm, rotation_amount - 1);
  }
  else //OP2 is a register.
  {
    int shift_val = to_int(reg[data_ins.operand2.Register.Shift.Integer]);
    uint32_t Rm = to_int(reg[data_ins.operand2.Register.Rm]);

    switch (to_int(reg[data_ins.operand2.Register.Shift.ShiftT]))
    {
    case LOGICAL_LEFT: //logical left
    {
      reg[data_ins.operand2.op2] = to_bf(Rm >> shift_val);
      Change_FlagC = get_bit(Rm, 32 - shift_val);
      break;
    }
    case LOGICAL_RIGHT: //logical right
    {
      reg[data_ins.operand2.op2] = to_bf(Rm << shift_val);
      Change_FlagC = get_bit(Rm, shift_val - 1);
      break;
    }
    case ARITH_RIGHT: //arithmetic right
    {
      uint32_t after_shift = Rm << shift_val;
      int sign_bit = get_bit(Rm, 31);
      uint32_t mask = 0;
      for (int i = 31; i >= 32 - shift_val; i--)
      {
        mask += pow(sign_bit, i);
      }
      reg[data_ins.operand2.op2] = to_bf(after_shift | mask);
      Change_FlagC = get_bit(Rm, shift_val - 1);
      break;
    }
    case ROTATE_RIGHT: //rotate right
    {
      int af_rot_val = 0;
      for (int i = 0; i < shift_val; i++)
      {
        af_rot_val += pow(get_bit(Rm, i), (31 - i));
      }
      reg[data_ins.operand2.op2] = to_bf(af_rot_val + (Rm << shift_val));
      Change_FlagC = get_bit(Rm, shift_val - 1);
      break;
    }
    default:
      break;
    }
  }

  //compute the result
  uint32_t Rn_value = to_int(reg[data_ins.Rn]);
  uint32_t operand2 = to_int(reg[data_ins.operand2.op2]);
  switch (data_ins.OpCode)
  {
  case AND:
    result = (Rn_value && operand2);
  case EOR:
    result = (Rn_value ^ operand2);
  case SUB:
    result = (Rn_value - operand2);
  case RSB:
    result = (operand2 - Rn_value);
  case ADD:
    result = (Rn_value + operand2);
  case TST:
    (Rn_value && operand2); //result not written
  case TEQ:
    (Rn_value ^ operand2); //result not written
  case CMP:
    (Rn_value - operand2); //result not written
  case ORR:
    result = (Rn_value || operand2);
  case MOV:
    result = (operand2);
  default:
    break;
  }

  //save the result.
  reg[data_ins.Rd] = to_bf(result);

  //if S is set then CPSR should be update.
  if (data_ins.S)
  {
    armstate->flagN = get_bit(result, 31);
    armstate->flagZ = (!result) ? 1 : 0;
    armstate->flagC = Change_FlagC;
  }
}

void execute_DP_Im(void);

void execute_DP_NIm(void);

void execute_SDT(instruction_t *decoded, ArmState armstate)
{
  uint32_t result;
  trans trans_ins = decoded->u.trans;
  bitfield * reg = armstate->reg;

  if (trans_ins.I) // offset is a register.
  {
    int shift_val = to_int(reg[trans_ins.offset.Register.Shift.Integer]);
    uint32_t rm = to_int(reg[trans_ins.offset.Register.Rm]);

    switch (to_int(reg[trans_ins.offset.Register.Shift.ShiftT]))
    {
    case LOGICAL_LEFT: //logical left
    {
      reg[trans_ins.offset.offset_value] = to_bf(rm >> shift_val);
      break;
    }
    case LOGICAL_RIGHT: //logical right
    {
      reg[trans_ins.offset.offset_value] = to_bf(rm << shift_val);
      break;
    }
    case ARITH_RIGHT: //arithmetic right
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
    case ROTATE_RIGHT: //rotate right
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
    int rotation_amount = 2 * to_int(reg[trans_ins.offset.Io.Rotate]);
    uint32_t Imm = to_int(reg[trans_ins.offset.Io.Imm]);
    int af_rot_val = 0;
    for (int i = 0; i < rotation_amount; i++)
    {
      af_rot_val += pow(get_bit(Imm, i), (31 - i));
    }
    reg[trans_ins.offset.Io.Imm] = to_bf(af_rot_val + (Imm << rotation_amount));
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
    case load:
      result = to_int(reg[trans_ins.Rn]) + to_int(reg[trans_ins.offset.offset_value]);
      break;

    case store:
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
    case load:
      result = Rn;
      break;

    case store:
      reg[trans_ins.Rd] = to_bf(Rn);
      break;

    default:
      break;
    }
    Rn = (trans_ins.U) ? Rn + offset : Rn - offset;
  }
}

void execute_BRANCH(instruction_t *decoded, ArmState armstate)
{
  uint32_t offset = to_int(armstate->reg[decoded->u.branch.offset]);
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
  armstate->pc += extended;
}

bool test_instruction_cond(instruction_t *instruction, ArmState armstate)
{
  bool N = armstate->flagN;
  bool Z = armstate->flagZ;
  bool C = armstate->flagC;
  bool V = armstate->flagV;
  switch (instruction->u.data_process.cond) // cond is at the same position in all cases
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
