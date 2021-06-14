#include <stdio.h>

#include "../../global_utils/types_and_macros.h"

#include "execute_helper.h"

#include "../../global_utils/tools.h"
#include "../utils/load_store.h"

static uint32_t dp_carried_result(pd_opcode_type opcode, uint32_t Rn,
                                  uint32_t operand2, bool *new_flag_c);
/*!
 *
 * @param val
 * @return true if val is negative, false if val is positive
 */
bool is_neg(uint32_t val) { return get_bit(val, 31); }

/*!
 *
 * @param target
 * @param rotate_amount
 * @return the result after rotate
 */
value_carry_t rotate(uint32_t target, int rotate_amount)
{
  value_carry_t result;
  if (rotate_amount == 0)
  {
    result.value = target;
    result.carry = 0;
    return result;
  }

  result.value
      = (target << (WORD_SIZE - rotate_amount)) | (target >> rotate_amount);
  result.carry = get_bit(target, rotate_amount - 1);
  return result;
}

/*!
 *
 * @param target
 * @param shift_amount
 * @param type : one of LSL, LSR, ASR, ROR
 * @return the result after the shift
 */
value_carry_t shift(uint32_t target, int shift_amount, shift_type type)
{
  if (shift_amount < 1 || shift_amount >= WORD_SIZE)
  {
    perror("shift_amount should in between 0 - 31");
    exit(EXIT_FAILURE);
  }

  value_carry_t result;

  // except LSL, other carry is defined as shift_amount - 1 bit of target
  result.carry = get_bit(target, shift_amount - 1);

  switch (type)
  {
  case LSL: // logical shift left
  {
    result.value = target << shift_amount;
    result.carry = get_bit(target, WORD_SIZE - shift_amount);
    return result;
  }
  case LSR: // logical shift right
  {
    result.value = target >> shift_amount;
    return result;
  }
  case ASR: // arithmetic shift right
  {
    // padding the 31th bit to result value
    uint32_t padmask
        = get_bit(target, 31) ? ALL_ONE << (WORD_SIZE - shift_amount) : 0;
    result.value = target >> shift_amount | padmask;
    return result;
  }
  case ROR: // rotate right
  {
    return rotate(target, shift_amount);
  }
  default:
    perror("Internal error! unknown shift type!");
    exit(EXIT_FAILURE);
    ;
  }
}

/*!
 *
 * @param instruction
 * @param arm_state
 * @return a bool value which depends on the condition code of the instruction
 */
bool test_instruction_cond(instruction_t instruction, ArmState arm_state)
{
  bool N = arm_state->neg;
  bool V = arm_state->ovflw;
  // cond is at the same position in all cases
  switch (instruction.word.proc.cond)
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
    return true;
    // perror("Internal error! unknown cond type!");
    // exit(EXIT_FAILURE);
  }
}

/*!
 * Handle different cases according to the reg_imm
 * @param reg : an array that each element represents word in registers
 * @param reg_imm : stores either an immediate value or a shifted register
 * @param is_imm : true if reg_imm is an immediate value, false if reg_imm is a shifted register
 * @param value_out : the output value of the result
 * @param carry : the output carry of the result
 */
void reg_imm_handle(bitfield *reg, reg_or_imm_t reg_imm, bool is_imm,
                    uint32_t *value_out, bool *carry)
{
  value_carry_t result;

  if (is_imm) // is rotated immediate value
  {
    // arm only rotate value by a multiple of 2
    result = rotate(reg_imm.rot_imm.imm, 2 * reg_imm.rot_imm.amount);
  }
  else // is shifted register
  {
    result = shift(to_int(reg[reg_imm.shift_reg.Rm]), reg_imm.shift_reg.val,
                   reg_imm.shift_reg.type);
  }

  *value_out = result.value;
  if (carry != NULL)
  {
    *carry = result.carry;
  }
}

/*!
 * execute an an data processing instruction
 * @param instruction : an data processing instruction
 * @param arm_state : current state of the arm machine
 */
void execute_DP(proc_t instruction, ArmState arm_state)
{
  bitfield *reg = arm_state->reg;
  uint32_t  Rn  = to_int(reg[instruction.Rn]);
  uint32_t  operand2;
  bool      new_flag_c;

  // compute operand2, carry out is set to shift barrel carry out
  reg_imm_handle(reg, instruction.operand2, instruction.iFlag, &operand2,
                 &new_flag_c);

  uint32_t result
      = dp_carried_result(instruction.opcode, Rn, operand2, &new_flag_c);

  // update the result if opcode is NOT (TST, TEQ or CMP)
  if (instruction.opcode != TST && instruction.opcode != TEQ
      && instruction.opcode != CMP)
  {
    reg[instruction.Rd] = to_bf(result);
  }

  // if set_cond bit is set, change the CPRS flags
  if (instruction.set_cond)
  {
    arm_state->neg   = get_bit(result, 31); // TODO : make it a macro SIGN_BIT
    arm_state->zero  = result == 0;
    arm_state->carry = new_flag_c;
  }
}

/*
 * find the result for data processing instruction
 * `new_flag_c` is a output param, it should accept a carry bit from shift
 * barrel, and it will update this flag_c according to opcode type
 * i.e. keep the value if it is a logical operation and change it to
 * arithmetic carry out if it is a arithmetic carry out
 */
static uint32_t dp_carried_result(pd_opcode_type opcode, uint32_t operand1,
                                  uint32_t operand2, bool *new_flag_c)
{
  uint32_t result;
  switch (opcode)
  {
  // for logical operations, C is set to the carry out from any shift
  // operation i.e. by no change
  case AND:
    return operand1 & operand2;
  case EOR:
    return operand1 ^ operand2;
  case ORR:
    return operand1 | operand2;
  case TST:
    return operand1 & operand2;
  case TEQ:
    return operand1 ^ operand2;
  case MOV:
    return operand2;
  // In arithmetic operations (add, sub, rsb and cmp) the C bit will be set
  // to the carry out of the bit 31 of the ALU

  // TODO!!!!!!
  // there will be problems, that I am not sure how unsigned or signed add
  // and mul performed.
  case SUB:
    result      = operand1 - operand2;
    *new_flag_c = (is_neg(operand1) == is_neg(operand2)) != is_neg(result);
    return result;
  case RSB:
    result      = operand2 - operand1;
    *new_flag_c = (is_neg(operand1) == is_neg(operand2)) != is_neg(result);
    return result;
  case ADD:
    result      = operand1 + operand2;
    *new_flag_c = (is_neg(operand1) == is_neg(operand2)) != is_neg(result);
    return result;
  case CMP:
    result      = operand1 - operand2;
    *new_flag_c = (is_neg(operand1) == is_neg(operand2)) != is_neg(result);
    return result;
  default:
    break;
  }
}

void execute_TRANS(trans_t instruction, ArmState arm_state)
{
  if (instruction.Rn == 15) // pc
  {
    perror("Doesn't support to load or store PC! \n");
    exit(EXIT_FAILURE);
  }

  bitfield *reg = arm_state->reg;
  uint32_t  Rn  = to_int(reg[instruction.Rn]);
  uint32_t  offset;

  // if i bit is set to 0, is immediate value else is shifted register
  reg_imm_handle(reg, instruction.offset, !instruction.iFlag, &offset, NULL);
  // if is_up is set then offset is added to Rn. Otherwise subtracted from Rn.
  size_t address_with_offset = (instruction.is_up) ? Rn + offset : Rn - offset;
  size_t address;

  if (instruction.is_pre)
  {
    // pre-indexing does not change the contents of the base register
    address = address_with_offset;
  }
  else // is post indexing
  {
    // post-indexing changes the contents of the base register by the offset.
    address             = Rn;
    reg[instruction.Rn] = to_bf(address_with_offset);
  }

  if (instruction.is_load)
  {
    reg[instruction.Rd] = load(address, arm_state->memory);
  }
  else // is store
  {
    store(reg[instruction.Rd], address, arm_state->memory);
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
  offset <<= 2;

  // if is negative padding 1s to left
  if (sign_bit == 1)
  {
    offset |= BRANCH_PAD_MASK;
  }

  // PC is 8 bytes ahead of the instruction that is being executed
  arm_state->pc += offset - 8;
}