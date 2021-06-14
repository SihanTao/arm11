#include <stdlib.h>
#include "../../global_utils/types_and_macros.h"
#include "decode.h"

#include "../../global_utils/tools.h"

<<<<<<< HEAD
static reg_or_imm_t  reg_or_imm_helper(bool is_imm, bitfield fetched);
static instruction_t decode_branch(bitfield fetched);
static instruction_t decode_dp(bitfield fetched);
static instruction_t decode_mul(bitfield fetched);
static instruction_t decode_trans(bitfield fetched);
=======

static reg_or_imm_t reg_or_imm_helper(bool is_imm, uint32_t fetched);
static instruction_t decode_branch(uint32_t fetched);
static instruction_t decode_dp(uint32_t fetched);
static instruction_t decode_mul(uint32_t fetched);
static instruction_t decode_trans(uint32_t fetched);
>>>>>>> 15416429fda8bc9b667cffd2c07379d125447771

/*!
 *
 * @param fetched : a fetched instruction in the form of bitfield from the memory fo the arm machine
 * @return : a decoded instruction in the form of instruction_t
 */

instruction_t decode(bitfield fetched)
{
<<<<<<< HEAD
  uint32_t fetched_val = fetched;
=======
  uint32_t fetched_val = to_int(fetched);
  printf("fetched_val :>> %u\n,", fetched_val); //DELETE_MARK
  printf("get_bit_range(fetched_val, 28, 31) :>> %x\n,", get_bit_range(fetched_val, 28, 31)); //DELETE_MARK
>>>>>>> 15416429fda8bc9b667cffd2c07379d125447771
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

<<<<<<< HEAD
/*!
 * This function is used to calculate the offset in single data transfer instruction
 * and the operand in data processing instruction
 * @param is_imm : true if the instruction contains a immediate value, false if it contains a shifted register
 * @param fetched : a fetched instruction
 * @return the processed result contains the information of the immediate value and the shifted register
 */
reg_or_imm_t reg_or_imm_helper(bool is_imm, uint32_t fetched)
=======
reg_or_imm_t reg_or_imm_helper(bool is_imm, bitfield fetched)
>>>>>>> code_generate_dp
{
  reg_or_imm_t result;
  if (is_imm)
  {
<<<<<<< HEAD
    result.rot_imm.imm    = get_bit_range(fetched, 0, 7);
    result.rot_imm.amount = get_bit_range(fetched, 8, 11);
=======
    result.rot_imm.imm = get_bit_range(fetched, IMM_START, IMM_END);
    result.rot_imm.amount = get_bit_range(fetched, ROTATE_START, ROTATE_END);
>>>>>>> 15416429fda8bc9b667cffd2c07379d125447771
    return result;
  }
  else
  {
<<<<<<< HEAD
    result.shift_reg.Rm   = get_bit_range(fetched, 0, 3);
    result.shift_reg.type = get_bit_range(fetched, 5, 6);
    result.shift_reg.val  = get_bit_range(fetched, 7, 11);
=======
    result.shift_reg.Rm = get_bit_range(fetched, RM_START, RM_END);
    result.shift_reg.type = get_bit_range(fetched, SHIFT_TYPE_START, SHIFT_TYPE_END);
    result.shift_reg.val = get_bit_range(fetched, SHIFT_CONST_START, SHIFT_CONST_END);
>>>>>>> 15416429fda8bc9b667cffd2c07379d125447771
    return result;
  }
}

<<<<<<< HEAD
/*!
 *
 * @param fetched: a fetched instruction which should be a single data transfer
 * @return a branch instruction
 */
instruction_t decode_branch(uint32_t fetched)
=======
instruction_t decode_branch(bitfield fetched)
>>>>>>> code_generate_dp
{
  instruction_t result;
<<<<<<< HEAD
  result.tag                = BRAN;
  result.cond               = get_bit_range(fetched, 28, 31);
  result.word.branch.offset = get_bit_range(fetched, 0, 23);
=======
  result.tag = BRANCH;
  result.word.branch.cond   = get_bit_range(fetched, CONDITION_START, CONDITION_END);
  result.word.branch.offset = get_bit_range(fetched, BRANCH_OFFSET_START, BRANCH_OFFSET_END);
>>>>>>> 15416429fda8bc9b667cffd2c07379d125447771
  return result;
}

<<<<<<< HEAD
/*!
 *
 * @param fetched : a fetched instruction which should be a data processing instruction
 * @return a processed data processing instruction
 */
instruction_t decode_dp(uint32_t fetched)
=======
instruction_t decode_dp(bitfield fetched)
>>>>>>> code_generate_dp
{
  instruction_t result;

<<<<<<< HEAD
  result.tag                = PROC;
  result.cond               = get_bit_range(fetched, 28, 31);
  result.word.proc.iFlag    = get_bit(fetched, 25);
  result.word.proc.opcode   = get_bit_range(fetched, 21, 24);
  result.word.proc.set_cond = get_bit(fetched, 20);
  result.word.proc.Rn       = get_bit_range(fetched, 16, 19);
  result.word.proc.Rd       = get_bit_range(fetched, 12, 15);
  result.word.proc.operand2
      = reg_or_imm_helper(result.word.proc.iFlag, fetched);
=======
  result.tag = DATA_PROCESS;
  result.word.proc.cond = get_bit_range(fetched, CONDITION_START, CONDITION_END);
  result.word.proc.iFlag = get_bit(fetched, I_BIT);
  result.word.proc.opcode = get_bit_range(fetched, OPCODE_START, OPCODE_END);
  result.word.proc.set_cond = get_bit(fetched, SET_CONDITION_CODES_BIT);
  result.word.proc.Rn = get_bit_range(fetched, DP_RN_START, DP_RN_END);
  result.word.proc.Rd = get_bit_range(fetched, DP_RD_START, DP_RD_END);
  result.word.proc.operand2 = reg_or_imm_helper(result.word.proc.iFlag, fetched);
>>>>>>> 15416429fda8bc9b667cffd2c07379d125447771
  return result;
}

<<<<<<< HEAD
/*!
 *
 * @param fetched : a fetched instruction which should be a multiply instruction
 * @return a processed multiply instruction
 */
instruction_t decode_mul(uint32_t fetched)
=======
instruction_t decode_mul(bitfield fetched)
>>>>>>> code_generate_dp
{
  instruction_t result;

<<<<<<< HEAD
  result.tag               = MUL;
  result.cond              = get_bit_range(fetched, 28, 31);
  result.word.mul.acc      = get_bit(fetched, 21);
  result.word.mul.set_cond = get_bit(fetched, 20);
  result.word.mul.Rd       = get_bit_range(fetched, 16, 19);
  result.word.mul.Rn       = get_bit_range(fetched, 12, 15);
  result.word.mul.Rs       = get_bit_range(fetched, 8, 11);
  result.word.mul.Rm       = get_bit_range(fetched, 0, 3);
=======
  result.tag = MUL;
  result.word.mul.cond = get_bit_range(fetched, CONDITION_START, CONDITION_END);
  result.word.mul.acc = get_bit(fetched, ACCUMULATE_BIT);
  result.word.mul.set_cond = get_bit(fetched, SET_CONDITION_CODES_BIT);
  result.word.mul.Rd = get_bit_range(fetched, MUL_RD_START, MUL_RN_END);
  result.word.mul.Rn = get_bit_range(fetched, MUL_RN_START, MUL_RN_END);
  result.word.mul.Rs = get_bit_range(fetched, MUL_RS_START, MUL_RS_END);
  result.word.mul.Rm = get_bit_range(fetched, MUL_RM_START, MUL_RM_END);
>>>>>>> 15416429fda8bc9b667cffd2c07379d125447771
  return result;
}

<<<<<<< HEAD
/*!
 *
 * @param fetched : a fetched instruction which should be a single data transfer instruction
 * @return a processed single transfer instruction
 */
instruction_t decode_trans(uint32_t fetched)
=======
instruction_t decode_trans(bitfield fetched)
>>>>>>> code_generate_dp
{
  instruction_t result;

<<<<<<< HEAD
  result.tag                = TRANS;
  result.cond               = get_bit_range(fetched, 28, 31);
  result.word.trans.iFlag   = get_bit(fetched, 25);
  result.word.trans.is_pre  = get_bit(fetched, 24);
  result.word.trans.is_up   = get_bit(fetched, 23);
  result.word.trans.is_load = get_bit(fetched, 20);
  result.word.trans.Rn      = get_bit_range(fetched, 16, 19);
  result.word.trans.Rd      = get_bit_range(fetched, 12, 15);
  result.word.trans.offset  = reg_or_imm_helper(!result.word.trans.iFlag, fetched);
=======
  result.tag = TRANS;
  result.word.trans.cond = get_bit_range(fetched, CONDITION_START, CONDITION_END);
  result.word.trans.iFlag = get_bit(fetched, I_BIT);
  result.word.trans.is_pre = get_bit(fetched, PRE_POST_INDEXING_BIT);
  result.word.trans.is_up = get_bit(fetched, UP_BIT);
  result.word.trans.is_load = get_bit(fetched, LOAD_STORE_BIT);
  result.word.trans.Rn = get_bit_range(fetched, TRANS_RN_START, TRANS_RN_END);
  result.word.trans.Rd = get_bit_range(fetched, TRANS_RD_START, TRANS_RD_END);
  result.word.trans.offset = reg_or_imm_helper(!result.word.trans.iFlag, fetched);
>>>>>>> 15416429fda8bc9b667cffd2c07379d125447771
  return result;
}
