#include <stdint.h>
#include <stdbool.h>

#include "../data_structure/token_stream.h"
#include "../../global_utils//tools.h"
#include "../../global_utils/types_and_macros.h"


uint32_t to_bcode_mov(Token cur_token)
{
  proc_t intermidiate_rep;

// retrive information from cur_token
// initialize intermidiate_rep
// call encode_DP
  
//intermidiate_rep.tag = DATA_PROCESS;
//intermidiate_rep.word.proc.cond = ?;
// intermidiate_rep.word.proc.iFlag ;
// intermidiate_rep.word.proc.opcode;
// intermidiate_rep.word.proc.operand2;
// intermidiate_rep.word.proc.Rd;
// intermidiate_rep.word.proc.Rn;
// intermidiate_rep.word.proc.set_cond;

  return encode_DP(intermidiate_rep);
}

void reg_imm_helper(bool is_imm, reg_or_imm_t reg_or_imm, uint32_t *target)
{
  if (is_imm) 
  {
    set_bit_range(target, reg_or_imm.rot_imm.imm, 0, 7);
    set_bit_range(target, reg_or_imm.rot_imm.amount, 8, 11);
  } else 
  {
    set_bit_range(target, reg_or_imm.shift_reg.Rm, 0, 3);
    set_bit_range(target, reg_or_imm.shift_reg.type, 5, 6);
    set_bit_range(target, reg_or_imm.shift_reg.val, 7, 11);
  }
}

uint32_t encode_DP(proc_t instruction)
{
  uint32_t result = 0;
  set_bit_range(&result, instruction.cond, 28, 31);
  set_bit(&result, instruction.iFlag, 25);
  set_bit_range(&result, instruction.opcode, 21, 24);
  set_bit(&result, instruction.set_cond, 20);
  set_bit_range(&result, instruction.Rn, 16, 19);
  set_bit_range(&result, instruction.Rd, 12, 15);
  reg_imm_helper(instruction.iFlag, instruction.operand2, result);
}

uint32_t encode_MUL(mul_t instruction)
{
  uint32_t result = 0;
  set_bit_range(&result, instruction.cond, 28, 31);
  set_bit(&result, instruction.acc, 21);
  set_bit(&result, instruction.set_cond, 20);
  set_bit_range(&result, instruction.Rd, 16, 19);
  set_bit_range(&result, instruction.Rn, 12, 15);
  set_bit_range(&result, instruction.Rs, 8, 11);
  set_bit_range(&result, instruction.Rm, 0, 3);
}

uint32_t encode_TRANS(trans_t instruction)
{
  uint32_t result = 0;
  set_bit_range(&result, instruction.cond, 28, 31);
  set_bit(&result, instruction.iFlag, 25);
  set_bit(&result, instruction.is_pre, 24);
  set_bit(&result, instruction.is_up, 23);
  set_bit(&result, instruction.is_load, 20);
  set_bit_range(&result, instruction.Rn, 16, 19);
  set_bit_range(&result, instruction.Rd, 12, 15);
  reg_imm_helper(!instruction.iFlag, instruction.offset, result);
}

uint32_t encode_BRANCH(branch_t instruction)
{
  uint32_t result = 0;
  set_bit_range(&result, instruction.cond, 28, 31);
  set_bit_range(&result, instruction.offset, 0, 23);
}

