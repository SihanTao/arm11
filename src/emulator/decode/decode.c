#include <stdlib.h>

#include "../../global_utils/types_and_macros.h"

#include "decode.h"

#include "../../global_utils/tools.h"

static reg_or_imm_t  reg_or_imm_helper(bool is_imm, bitfield fetched);
static instruction_t decode_branch(bitfield fetched);
static instruction_t decode_dp(bitfield fetched);
static instruction_t decode_mul(bitfield fetched);
static instruction_t decode_trans(bitfield fetched);

instruction_t decode(bitfield fetched)
{
  uint32_t fetched_val = fetched;
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
  else if (get_bit_range(fetched_val, 26, 27) == 0)
  {
    return decode_dp(fetched_val);
  }
  else
  {
    instruction_t result;
    result.tag = UNDEFINED;
    return result;
  }
}

reg_or_imm_t reg_or_imm_helper(bool is_imm, bitfield fetched)
{
  reg_or_imm_t result;
  if (is_imm)
  {
    result.rot_imm.imm    = get_bit_range(fetched, IMM_START, IMM_END);
    result.rot_imm.amount = get_bit_range(fetched, ROTATE_START, ROTATE_END);
    return result;
  }
  else
  {
    result.shift_reg.Rm   = get_bit_range(fetched, RM_START, RM_END);
    result.shift_reg.type = get_bit_range(fetched, SHIFT_TYPE_START, SHIFT_TYPE_END);
    result.shift_reg.val  = get_bit_range(fetched, SHIFT_CONST_START, SHIFT_CONST_END);
    return result;
  }
}

instruction_t decode_branch(bitfield fetched)
{
  instruction_t result;
  result.tag                = BRAN;
  result.cond   = get_bit_range(fetched, CONDITION_START, CONDITION_END);
  result.word.branch.offset = get_bit_range(fetched, BRANCH_OFFSET_START, BRANCH_OFFSET_END);
  return result;
}

instruction_t decode_dp(bitfield fetched)
{
  instruction_t result;

  result.tag                = PROC;
  result.cond     = get_bit_range(fetched, CONDITION_START, CONDITION_END);
  result.word.proc.is_imm    = get_bit(fetched, I_BIT);
  result.word.proc.opcode   = get_bit_range(fetched, OPCODE_START, OPCODE_END);
  result.word.proc.set_cond = get_bit(fetched, S_BIT);
  result.word.proc.Rn       = get_bit_range(fetched, DP_RN_START, DP_RN_END);
  result.word.proc.Rd       = get_bit_range(fetched, DP_RD_START, DP_RD_END);
  result.word.proc.operand2
      = reg_or_imm_helper(result.word.proc.is_imm, fetched);
  return result;
}

instruction_t decode_mul(bitfield fetched)
{
  instruction_t result;

  result.tag               = MUL;
  result.cond = get_bit_range(fetched, CONDITION_START, CONDITION_END);
  result.word.mul.is_mla = get_bit(fetched, ACCUMULATE_BIT);
  result.word.mul.set_cond = get_bit(fetched, SET_CONDITION_CODES_BIT);
  result.word.mul.Rd = get_bit_range(fetched, MUL_RD_START, MUL_RD_END);
  result.word.mul.Rn = get_bit_range(fetched, MUL_RN_START, MUL_RN_END);
  result.word.mul.Rs = get_bit_range(fetched, MUL_RS_START, MUL_RS_END);
  result.word.mul.Rm = get_bit_range(fetched, MUL_RM_START, MUL_RM_END);
  return result;
}

instruction_t decode_trans(bitfield fetched)
{
  instruction_t result;

  result.tag                = TRANS;
  result.cond    = get_bit_range(fetched, CONDITION_START, CONDITION_END);
  result.word.trans.is_reg   = get_bit(fetched, I_BIT);
  result.word.trans.is_pre  = get_bit(fetched, PRE_POST_INDEXING_BIT);
  result.word.trans.is_up   = get_bit(fetched, UP_BIT);
  result.word.trans.is_load = get_bit(fetched, LOAD_STORE_BIT);
  result.word.trans.Rn      = get_bit_range(fetched, TRANS_RN_START, TRANS_RN_END);
  result.word.trans.Rd      = get_bit_range(fetched, TRANS_RD_START, TRANS_RD_END);
  result.word.trans.offset
      = reg_or_imm_helper(!result.word.trans.is_reg, fetched);
  return result;
}
