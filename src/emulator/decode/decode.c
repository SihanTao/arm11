#include <stdlib.h>
#include "../../global_utils/types_and_macros.h"
#include "decode.h"
#include "../../global_utils/tools.h"


static reg_or_imm_t reg_or_imm_helper(bool is_imm, uint32_t fetched);
static instruction_t decode_branch(uint32_t fetched);
static instruction_t decode_dp(uint32_t fetched);
static instruction_t decode_mul(uint32_t fetched);
static instruction_t decode_trans(uint32_t fetched);

/*!
 *
 * @param fetched : a fetched instruction in the form of bitfield from the memory fo the arm machine
 * @return : a decoded instruction in the form of instruction_t
 */

instruction_t decode(bitfield fetched)
{
  uint32_t fetched_val = to_int(fetched);
  printf("fetched_val :>> %u\n,", fetched_val); //DELETE_MARK
  printf("get_bit_range(fetched_val, 28, 31) :>> %x\n,", get_bit_range(fetched_val, 28, 31)); //DELETE_MARK
  if (fetched_val == 0)
  {
    instruction_t result;
    result.tag = ZERO;
    return result;
  }
  if (get_bit(fetched_val, 27))
  {
    return decode_branch(fetched_val);
  }
  else if (get_bit(fetched_val, 26))
  {
    return decode_trans(fetched_val);
  }
  else if (get_bit_range(fetched_val, 22, 27) == 0
           && get_bit_range(fetched_val, 4, 7) == 0x9)
  {
    return decode_mul(fetched_val);
  }
  else
  {
    return decode_dp(fetched_val);
  }
}

/*!
 * This function is used to calculate the offset in single data transfer instruction
 * and the operand in data processing instruction
 * @param is_imm : true if the instruction contains a immediate value, false if it contains a shifted register
 * @param fetched : a fetched instruction
 * @return the processed result contains the information of the immediate value and the shifted register
 */
reg_or_imm_t reg_or_imm_helper(bool is_imm, uint32_t fetched)
{
  reg_or_imm_t result;
  if (is_imm)
  {
    result.rot_imm.imm = get_bit_range(fetched, IMM_START, IMM_END);
    result.rot_imm.amount = get_bit_range(fetched, ROTATE_START, ROTATE_END);
    return result;
  } else
  {
    result.shift_reg.Rm = get_bit_range(fetched, RM_START, RM_END);
    result.shift_reg.type = get_bit_range(fetched, SHIFT_TYPE_START, SHIFT_TYPE_END);
    result.shift_reg.val = get_bit_range(fetched, SHIFT_CONST_START, SHIFT_CONST_END);
    return result;
  }
}

/*!
 *
 * @param fetched: a fetched instruction which should be a single data transfer
 * @return a branch instruction
 */
instruction_t decode_branch(uint32_t fetched)
{
  instruction_t result;
  result.tag = BRANCH;
  result.word.branch.cond   = get_bit_range(fetched, CONDITION_START, CONDITION_END);
  result.word.branch.offset = get_bit_range(fetched, BRANCH_OFFSET_START, BRANCH_OFFSET_END);
  return result;
}

/*!
 *
 * @param fetched : a fetched instruction which should be a data processing instruction
 * @return a processed data processing instruction
 */
instruction_t decode_dp(uint32_t fetched)
{
  instruction_t result;

  result.tag = DATA_PROCESS;
  result.word.proc.cond = get_bit_range(fetched, CONDITION_START, CONDITION_END);
  result.word.proc.iFlag = get_bit(fetched, I_BIT);
  result.word.proc.opcode = get_bit_range(fetched, OPCODE_START, OPCODE_END);
  result.word.proc.set_cond = get_bit(fetched, S_BIT);
  result.word.proc.Rn = get_bit_range(fetched, DP_RN_START, DP_RN_END);
  result.word.proc.Rd = get_bit_range(fetched, DP_RD_START, DP_RD_END);
  result.word.proc.operand2 = reg_or_imm_helper(result.word.proc.iFlag, fetched);
  return result;
}

/*!
 * TODO
 * @param fetched
 * @return
 */
instruction_t decode_mul(uint32_t fetched)
{
  instruction_t result;

  result.tag = MUL;
  result.word.mul.cond = get_bit_range(fetched, 28, 31);
  result.word.mul.acc = get_bit(fetched, 21);
  result.word.mul.set_cond = get_bit(fetched, 20);
  result.word.mul.Rd = get_bit_range(fetched, 16, 19);
  result.word.mul.Rn = get_bit_range(fetched, 12, 15);
  result.word.mul.Rs = get_bit_range(fetched, 8, 11);
  result.word.mul.Rm = get_bit_range(fetched, 0, 3);
  return result;
}

/*!
 * TODO
 * @param fetched
 * @return
 */
instruction_t decode_trans(uint32_t fetched)
{
  instruction_t result;

  result.tag = TRANS;
  result.word.trans.cond = get_bit_range(fetched, 28, 31);
  result.word.trans.iFlag = get_bit(fetched, 25);
  result.word.trans.is_pre = get_bit(fetched, 24);
  result.word.trans.is_up = get_bit(fetched, 23);
  result.word.trans.is_load = get_bit(fetched, 20);
  result.word.trans.Rn = get_bit_range(fetched, 16, 19);
  result.word.trans.Rd = get_bit_range(fetched, 12, 15);
  result.word.trans.offset = reg_or_imm_helper(!result.word.trans.iFlag, fetched);
  return result;
}
