#include <math.h>
#include <stdlib.h>

#include "../utils/types_and_macros.h"

#include "execute.h"

#include "../execute/execute_helper.h"
#include "../utils/tools.h"

static uint32_t dp_carried_result(proc_type opcode, uint32_t Rn,
                                  uint32_t operand2, bool *newFlagC);

exit_type execute(instruction_t decoded, ArmState arm_state)
{
  if (test_instruction_cond(decoded, arm_state))
  {
    switch (decoded.tag)
    {
    case DATA_PROCESS:
      execute_DP(decoded.word.proc, arm_state);
      return CONTINUE;
    case MUL:
      execute_MUL(decoded.word.mul, arm_state);
      return CONTINUE;
    case TRANS:
      execute_TRANS(decoded.word.trans, arm_state);
      return CONTINUE;
    case BRANCH:
      execute_BRANCH(decoded.word.branch, arm_state);
      return CONTINUE;
    case ZERO:
      return EXIT;
    default:
      return ERROR;
    }
  }

  return CONTINUE;
}

void execute_DP(proc_t instruction, ArmState arm_state)
{
  bitfield *reg = arm_state->reg;
  uint32_t  Rn  = to_int(reg[instruction.Rn]);
  uint32_t  operand2;
  bool      newFlagC;

  // compute operand2, carry out is set to shift barrel carry out
  reg_imm_handle(reg, instruction.operand2, instruction.is_imm, &operand2,
                 &newFlagC);

  uint32_t result
      = dp_carried_result(instruction.opcode, Rn, operand2, &newFlagC);

  // don't update the result if opcode is TST, TEQ or CMP
  if (instruction.opcode != TST && instruction.opcode != TEQ
      && instruction.opcode != CMP)
  {
    reg[instruction.Rd] = to_bf(result);
  }

  // if set_cond bit is set, change the CPRS flags
  if (instruction.set_cond)
  {
    arm_state->neg   = get_bit(result, 31);
    arm_state->zero  = result == 0;
    arm_state->carry = newFlagC;
  }
}

static uint32_t dp_carried_result(proc_type opcode, uint32_t Rn,
                                  uint32_t operand2, bool *newFlagC)
{
  switch (opcode)
  {
  // for logical operations, C is set to the carry out from any shift
  // operation i.e. by no change
  case AND:
    return Rn && operand2;
  case EOR:
    return Rn ^ operand2;
  case ORR:
    return Rn || operand2;
  case TST:
    return Rn && operand2;
  case TEQ:
    return Rn ^ operand2;
  case MOV:
    return operand2;
  // In arithmetic operations (add, sub, rsb and cmp) the C bit will be set
  // to the carry out of the bit 31 of the ALU

  // TODO!!!!!!
  // there will be problems, that I am not sure how unsigned or signed add
  // and mul performed.
  case SUB:
    *newFlagC = get_bit(Rn, 31) && get_bit(Rn, 31); // might be a bug here
    return Rn - operand2;
  case RSB:
    *newFlagC = 1;
    return operand2 - Rn;
  case ADD:
    *newFlagC = 1;
    return Rn + operand2;
  case CMP:
    *newFlagC = 1;
    return Rn - operand2; // result not written
  default:
    break;
  }
}

void execute_TRANS(trans_t instruction, ArmState arm_state)
{
  bitfield *reg = arm_state->reg;
  uint32_t  Rn  = to_int(reg[instruction.Rn]);
  uint32_t  offset;

  reg_imm_handle(reg, instruction.offset, instruction.is_imm, &offset, NULL);

  // if is_up is set then offset is added to Rn. Otherwise the offset is
  // subtracted from Rn.
  Rn = (instruction.is_up) ? Rn + offset : Rn - offset;

  // For the purposes of this exercise, pre- indexing will not change the value
  // of the base register, however post-indexing will change the contents of
  // the base register by the offset.
  if (!instruction.is_pre) // pre indexing
  {
    reg[instruction.Rn] = to_bf(Rn);
  }

  if (instruction.is_load) // load from memory
  {
    reg[instruction.Rd] = load(Rn, arm_state->memory);
  }
  else // store to memory
  {
    // poke(reg[instruction.Rd], Rn, arm_state->memory);
  }
}

void execute_MUL(mul_t instruction, ArmState arm_state)
{
  // pre: PC is not used as operand or destination register
  //      Rd will not be the same as Rm
  bitfield *reg    = arm_state->reg;
  uint32_t  Rm     = to_int(reg[instruction.Rm]);
  uint32_t  Rs     = to_int(reg[instruction.Rs]);
  uint32_t  Rn     = to_int(reg[instruction.Rn]);
  uint32_t  result = Rm * Rs;

  // the accumulate bit is set
  if (instruction.acc)
  {
    result += Rn;
  }

  // Save the result
  reg[instruction.Rd] = to_bf(result);

  // If the set_cond bit is set, we need to update the CPSR
  if (instruction.set_cond)
  {
    arm_state->neg  = get_bit(result, 31);
    arm_state->zero = result == 0;
  }
}

void execute_BRANCH(branch_t instruction, ArmState arm_state)
{
  uint32_t offset   = to_int(arm_state->reg[instruction.offset]);
  int      sign_bit = get_bit(offset, 24);
  uint32_t mask     = 0;
  uint32_t extended = 0;

  // The offset will take into account the effect of the pipeline (i.e. PC is 8
  // bytes ahead of the instruction that is being executed). I think it is
  // kinda hard to write test for this branch instruction. since it is closely
  // related to pc.

  offset <<= 2;

  if (sign_bit == 0)
  {
    mask     = ~(2 ^ 25 + 2 ^ 26);
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
