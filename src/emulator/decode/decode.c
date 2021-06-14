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
 * TODO
 * @param is_imm
 * @param fetched
 * @return
 */
reg_or_imm_t reg_or_imm_helper(bool is_imm, uint32_t fetched)
{
  reg_or_imm_t result;
  if (is_imm)
  {
    result.rot_imm.imm = get_bit_range(fetched, 0, 7);
    result.rot_imm.amount = get_bit_range(fetched, 8, 11);
    return result;
  } else
  {
    result.shift_reg.Rm = get_bit_range(fetched, 0, 3);
    result.shift_reg.type = get_bit_range(fetched, 5, 6);
    result.shift_reg.val = get_bit_range(fetched, 7, 11);
    return result;
  }
}

/*!
 * TODO
 * @param fetched
 * @return
 */
instruction_t decode_branch(uint32_t fetched)
{
  instruction_t result;
  result.tag = BRANCH;
  result.word.branch.cond   = get_bit_range(fetched, 28, 31);
  result.word.branch.offset = get_bit_range(fetched, 0, 23);
  return result;
}

/*!
 * TODO
 * @param fetched
 * @return
 */
instruction_t decode_dp(uint32_t fetched)
{
  instruction_t result;

  result.tag = DATA_PROCESS;
  result.word.proc.cond = get_bit_range(fetched, 28, 31);
  result.word.proc.iFlag = get_bit(fetched, 25);
  result.word.proc.opcode = get_bit_range(fetched, 21, 24);
  result.word.proc.set_cond = get_bit(fetched, 20);
  result.word.proc.Rn = get_bit_range(fetched, 16, 19);
  result.word.proc.Rd = get_bit_range(fetched, 12, 15);
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
