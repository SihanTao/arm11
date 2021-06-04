#include "../utils/types_and_macros.h"
#include "execute_DP.h"
#include "../utils/tools.h"
#include "../execute/execute_helper.h"

void execute_DP(proc_t instruction, ArmState arm_state)
{
  bitfield *reg = arm_state->reg;
  uint32_t result = 0;
  uint32_t Rn = to_int(reg[instruction.Rn]);
  bitfield Rd = reg[instruction.Rd];

  bool newFlagC = new_carry(reg, instruction.operand2, instruction.I);
  uint32_t operand2 = reg_imm_handle(reg, instruction.operand2, !instruction.I);

  /*********** bugs here ******************/
  // if save result
  {
    compute_result(instruction.opcode, operand2, Rn, Rd);
  }

  if (instruction.S)
  {
    arm_state->flagN = get_bit(result, 31);
    arm_state->flagZ = result == 0;
    arm_state->flagC = newFlagC;
  }
}

/* need to rewrite */
bool new_carry(bitfield *reg, reg_or_imm_t shift_or_imm, bool is_imm)
{

  // if (is_imm) // OP2 is an immediate value.
  // {
  //   rot_imm_t op2_imm = shift_or_imm.rot_imm;
  //   int rotation_amount = 2 * to_int(reg[op2_imm.amount]);
  //   uint32_t Imm = to_int(reg[op2_imm.imm]);

  //   return get_bit(Imm, rotation_amount - 1);
  // }
  // else //OP2 is a register.
  // {
  //   shift_reg_t op2_reg = shift_or_imm.shift_reg;
  //   int shift_val = to_int(reg[op2_reg.shift.val]);
  //   uint32_t Rm = to_int(reg[op2_reg.Rm]);
  //   shift_type type = to_int(reg[op2_reg.shift.val]);

  //   return
  //       type == LSL ? get_bit(Rm, 32 - shift_val) : get_bit(Rm, shift_val - 1);
  // }
  return false;
}

void compute_result(proc_type opcode, uint32_t operand2, uint32_t Rn, bitfield Rd)
{
  switch (opcode)
  {
  case AND:
    Rd = to_bf(Rn && operand2);
  case EOR:
    Rd = to_bf(Rn ^ operand2);
  case SUB:
    Rd = to_bf(Rn - operand2);
  case RSB:
    Rd = to_bf (operand2 - Rn);
  case ADD:
    Rd = to_bf (Rn + operand2);
  case TST:
    /*** not sure what to do*****/
    (Rn && operand2); //result not written
  case TEQ:
    (Rn ^ operand2); //result not written
  case CMP:
    (Rn - operand2); //result not written
  case ORR:
    Rd = to_bf(Rn || operand2);
  case MOV:
    Rd = to_bf(operand2);
  default:
    break;
  }
}
