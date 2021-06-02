#include <stdlib.h>
#include <math.h>

#include "../utils/types_and_macros.h"
#include "../utils/tools.h"

#include "execute.h"

// TODO Here is an spelling error : decode -> decoded
// I will solve this after fix all other problems --Tony
bool execute(instruction_t *decoded, ArmState arm_state)
{
  if (test_instruction_cond(decoded, arm_state))
  {
    switch (decoded->tag)
    {
    case DATA_PROCESS:
      execute_DP(decoded, arm_state);
      break;
    case MUL:
      execute_MUL(decoded, arm_state);
      break;
    case TRANS:
      execute_SDT(decoded, arm_state);
      break;
    case BRANCH:
      execute_BRANCH(decoded, arm_state);
      break;
    case ZERO:
      return EXIT;
    case UNDEFINED:
      return ERROR;
    default:
      return ERROR;
    }
  }
  return CONTINUE;
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

// begin of execute_DP.
void execute_DP(instruction_t *decoded, ArmState arm_state)
{
  uint32_t result;
  data_process_t data_ins = decoded->u.data_process;
  bitfield *reg = arm_state->reg;
  uint32_t operand2 = 0;

  if (data_ins.I) // OP2 is an immediate value.
  {
    uint32_t operand2 = execute_DP_Im(reg, data_ins);
  }
  else //OP2 is a register.
  {
    uint32_t operand2 = execute_DP_NIm(reg, data_ins);
  }

  //compute the result
  uint32_t Rn_value = to_int(reg[data_ins.Rn]);
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
    arm_state->flagN = get_bit(result, 31);
    arm_state->flagZ = (!result) ? 1 : 0;
    arm_state->flagC = Change_FlagC;
  }
}

bool Change_FlagC = false; // the carry out bit need to be store in FlagC.

uint32_t rotate(int rotation_amout, uint32_t content) // function for rotate.
{
    int af_rot_val = 0;
    for (int i = 0; i < rotation_amout; i++)
      {
        af_rot_val += pow(get_bit(content, i), (31 - i));
      }
    return af_rot_val + (content >> rotation_amout);
}

uint32_t execute_DP_Im(bitfield *reg, data_process_t data_ins)
{
  int rotation_amount = 2 * to_int(reg[data_ins.operand2.Iv.Rotate]);
  uint32_t Imm = to_int(reg[data_ins.operand2.Iv.Imm]);
  Change_FlagC = get_bit(Imm, rotation_amount - 1);
  return rotate(rotation_amount, Imm);
}

uint32_t execute_DP_NIm(bitfield *reg, data_process_t data_ins)
{
  int shift_val = to_int(reg[data_ins.operand2.Register.Shift.Integer]);
  uint32_t Rm = to_int(reg[data_ins.operand2.Register.Rm]);

  switch (to_int(reg[data_ins.operand2.Register.Shift.ShiftT]))
  {
    case LOGICAL_LEFT: //logical left
    {
      Change_FlagC = get_bit(Rm, 32 - shift_val);
      return Rm << shift_val;
      break;
    }
    case LOGICAL_RIGHT: //logical right
    {
      Change_FlagC = get_bit(Rm, shift_val - 1);
      return Rm >> shift_val;
      break;
    }
    case ARITH_RIGHT: //arithmetic right
    {
      uint32_t after_shift = Rm >> shift_val;
      int sign_bit = get_bit(Rm, 31);
      uint32_t mask = 0;
      if (sign_bit == 1)
      {
        for (int i = 31; i >= 32 - shift_val; i--)
        {
          mask += pow(2, i);
        }
      }
      Change_FlagC = get_bit(Rm, shift_val - 1);
      return after_shift | mask;
      break;
    }
    case ROTATE_RIGHT: //rotate right
    {
      Change_FlagC = get_bit(Rm, shift_val - 1);
      return rotate(shift_val, Rm);
      break;
    }
    default:
      break;
  }
}
// end of execute_DP.

void execute_SDT(instruction_t *decoded, ArmState arm_state)
{
  uint32_t result;
  trans_t trans_ins = decoded->u.trans;
  bitfield *reg = arm_state->reg;

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

bool test_instruction_cond(instruction_t *instruction, ArmState arm_state)
{
  bool N = arm_state->flagN;
  bool Z = arm_state->flagZ;
  bool C = arm_state->flagC;
  bool V = arm_state->flagV;
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
