#include "../utils/types_and_macros.h"
#include "execute_DP.h"
#include "../utils/tools.h"
#include "../execute/execute_helper.h"

void execute_DP(instruction_t *decoded, ArmState arm_state)
{
  bitfield *reg = arm_state->reg;
  data_process_t data_ins = decoded->u.data_process;
  uint32_t result = 0;
  uint32_t Rn = to_int(reg[data_ins.Rn]);

  bool newFlagC = new_carry(reg, data_ins.operand2, data_ins.I);
  uint32_t operand2 = shift_imm_handle(reg, data_ins.operand2, data_ins.I);

  /*********** bugs here ******************/
  // if save result
  {
    result = compute_result(data_ins.opcode, operand2, Rn);
    reg[data_ins.Rd] = to_bf(result);
  }

  if (data_ins.S)
  {
    arm_state->flagN = get_bit(result, 31);
    arm_state->flagZ = (!result) ? 1 : 0;
    arm_state->flagC = newFlagC;
  }
}

bool new_carry(bitfield *reg, shift_or_imm_t shift_or_imm, bool is_imm)
{

  if (is_imm) // OP2 is an immediate value.
  {
    rotate_t op2_imm = shift_or_imm.imm_val;
    int rotation_amount = 2 * to_int(reg[op2_imm.rot_amt]);
    uint32_t Imm = to_int(reg[op2_imm.rot_val]);

    return get_bit(Imm, rotation_amount - 1);
  }
  else //OP2 is a register.
  {
    shift_reg_t op2_reg = shift_or_imm.shift_reg;
    int shift_val = to_int(reg[op2_reg.shift.integer]);
    uint32_t Rm = to_int(reg[op2_reg.Rm]);
    shift_type type = to_int(reg[op2_reg.shift.integer]);

    return
        type == LSL ? get_bit(Rm, 32 - shift_val) : get_bit(Rm, shift_val - 1);
  }
}

uint32_t compute_result(opcode_type opcode, uint32_t operand2, uint32_t Rn)
{
  switch (opcode)
  {
  case AND:
    return (Rn && operand2);
  case EOR:
    return (Rn ^ operand2);
  case SUB:
    return (Rn - operand2);
  case RSB:
    return (operand2 - Rn);
  case ADD:
    return (Rn + operand2);
  case TST:
    /*** not sure what to do*****/
    (Rn && operand2); //result not written
  case TEQ:
    (Rn ^ operand2); //result not written
  case CMP:
    (Rn - operand2); //result not written
  case ORR:
    return (Rn || operand2);
  case MOV:
    return (operand2);
  default:
    break;
  }
}
