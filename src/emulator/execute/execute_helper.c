#include <stdio.h>

#include "../../global_utils/types_and_macros.h"

#include "execute_helper.h"

#include "../../global_utils/tools.h"
#include "../utils/load_store.h"

/*!
 *
 * @param val
 * @return true if val is negative, false if val is positive
 */
bool is_neg(uint32_t val)
{
  return get_bit(val, SIGN_BIT);
}

/*!
 * users should not call this function by default
 * @param target
 * @param rotate_amount should be in range [0, 31], rotation amount is not
 * doubled in this function
 * @return the result after rotate
 */
value_carry_t rotate(uint32_t target, int rotate_amount)
{
  if (rotate_amount < 0 || rotate_amount >= NUM_OF_BIT_IN_WORD)
  {
    perror("Error! rotate_amount should in between 0 - 31\n");
    exit(EXIT_FAILURE);
  }

  value_carry_t result;
  if (rotate_amount == 0)
  {
    result.value = target;
    result.carry = 0;
    return result;
  }

  result.value = (target << (NUM_OF_BIT_IN_WORD - rotate_amount))
                 | (target >> rotate_amount);
  result.carry = get_bit(target, rotate_amount - 1);
  return result;
}

/*!
 * users should not call this function by default
 * @param target
 * @param shift_amount shift amount should be in range [0, 31]
 * @param type : one of LSL, LSR, ASR, ROR
 * @return the result after the shift
 */
value_carry_t shift(uint32_t target, int shift_amount, shift_type type)
{
  if (shift_amount < 0 || shift_amount >= NUM_OF_BIT_IN_WORD)
  {
    perror("shift_amount should in between 0 - 31");
    exit(EXIT_FAILURE);
  }

  value_carry_t result;
  if (shift_amount == 0)
  {
    result.value = target;
    result.carry = 0;
    return result;
  }

  // except LSL, other carry is defined as shift_amount - 1 bit of target
  result.carry = get_bit(target, shift_amount - 1);

  switch (type)
  {
  case LSL: // logical shift left
  {
    result.value = target << shift_amount;
    result.carry = get_bit(target, NUM_OF_BIT_IN_WORD - shift_amount);
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
    uint32_t padmask = get_bit(target, SIGN_BIT)
                           ? ALL_ONE << (NUM_OF_BIT_IN_WORD - shift_amount)
                           : 0;
    result.value     = target >> shift_amount | padmask;
    return result;
  }
  case ROR: // rotate right
  {
    return rotate(target, shift_amount);
  }
  default:
    perror("Internal error! unknown shift type!");
    exit(EXIT_FAILURE);
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
  bool Z = arm_state->zero;
  // cond is at the same position in all cases
  switch (instruction.cond)
  {
  case EQ:
    return (Z);
  case NE:
    return (!Z);
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
    perror("Internal error! unknown cond type!");
    exit(EXIT_FAILURE);
  }
}

/*!
 * Handle different cases according to the reg_imm
 * @param reg an array that each element represents word in registers
 * @param reg_imm
 * @param is_imm
 * @param value_out output param, the output value of the result
 * @param carry output param, if carry is setted to NULL,
 * this function will not change carry
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
    result = shift(reg[reg_imm.shift_reg.Rm], reg_imm.shift_reg.val,
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
void execute_proc(proc_t instruction, ArmState arm_state)
{
  bitfield *reg    = arm_state->reg;
  uint32_t  Rn_val = reg[instruction.Rn];
  uint32_t  operand2;
  bool      new_flag_c;

  // compute operand2, carry out is set to shift barrel carry out
  reg_imm_handle(reg, instruction.operand2, instruction.is_imm, &operand2,
                 &new_flag_c);

  uint32_t result
      = dp_carried_result(instruction.opcode, Rn_val, operand2, &new_flag_c);

  // update the result if opcode is NOT (TST, TEQ or CMP)
  if (instruction.opcode != TST && instruction.opcode != TEQ
      && instruction.opcode != CMP)
  {
    reg[instruction.Rd] = result;
  }

  // if set_cond bit is set, change the CPRS flags
  if (instruction.set_cond)
  {
    arm_state->neg
        = get_bit(result, SIGN_BIT); // TODO : make it a macro SIGN_BIT
    arm_state->zero  = result == 0;
    arm_state->carry = new_flag_c;
  }
}

/*!
 * find the result for data processing instruction
 * @param opcode
 * @param operand1
 * @param operand2
 * @param new_flag_c 	is a output param, it should accept a carry bit from
 * shift barrel, and it will update this flag_c according to opcode type i.e.
 * keep the value if it is a logical operation and change it to arithmetic
 * carry out if it is a arithmetic carry out
 * @return
 */
uint32_t dp_carried_result(pd_opcode_type opcode, uint32_t operand1,
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

/*!
 *
 * @param instruction an single data transfer instruction
 * @param arm_state current state of the arm machine
 */
void execute_trans(trans_t instruction, ArmState arm_state)
{
  bitfield *reg = arm_state->reg;
  uint32_t  Rn_val
      = instruction.Rn == PC ? arm_state->pc + 8 : reg[instruction.Rn];

  uint32_t offset;

  // if i bit is set to 0, is immediate value else is shifted register
  reg_imm_handle(reg, instruction.offset, !instruction.is_reg, &offset, NULL);
  // if is_up is set then offset is added to Rn. Otherwise subtracted from Rn.
  size_t address_with_offset
      = (instruction.is_up) ? Rn_val + offset : Rn_val - offset;
  size_t address;

  if (instruction.is_pre)
  {
    // pre-indexing does not change the contents of the base register
    address = address_with_offset;
  }
  else // is post indexing
  {
    // post-indexing changes the contents of the base register by the offset.
    address             = Rn_val;
    reg[instruction.Rn] = address_with_offset;
  }

  if (instruction.is_load)
  {
    load(address, arm_state->memory, &reg[instruction.Rd]);
  }
  else // is store
  {
    store(reg[instruction.Rd], address, arm_state->memory);
  }
}

/*!
 *
 * @param instruction an multiply instruction
 * @param arm_state current state of the arm machine
 */
void execute_mul(mul_t instruction, ArmState arm_state)
{
  // pre: PC is not used as operand or destination register
  //      Rd will not be the same as Rm
  bitfield *reg    = arm_state->reg;
  uint32_t  Rm_val = reg[instruction.Rm];
  uint32_t  Rs_val = reg[instruction.Rs];
  uint32_t  Rn_val = reg[instruction.Rn];
  uint32_t  result = Rm_val * Rs_val;

  // the accumulate bit is set
  if (instruction.is_mla)
  {
    result += Rn_val;
  }

  // Save the result
  reg[instruction.Rd] = result;

  // If the set_cond bit is set, we need to update the CPSR
  if (instruction.set_cond)
  {
    arm_state->neg  = get_bit(result, SIGN_BIT);
    arm_state->zero = result == 0;
  }
}

/*!
 *
 * @param instruction a branch instruction
 * @param arm_state current state of the arm machine
 */
void execute_bran(branch_t instruction, ArmState arm_state)
{
  // offset can be negative or positive
  int offset   = instruction.offset;
  int sign_bit = get_bit(offset, OFFSET_BIT);
  offset <<= 2;

  // if is negative padding 1s to left
  if (sign_bit == 1)
  {
    offset |= BRANCH_PAD_MASK;
  }

  arm_state->pc += offset;
}