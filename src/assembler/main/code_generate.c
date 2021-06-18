#include <stdio.h>

#include "../../global_utils/types_and_macros.h"
#include "../../global_utils/tools.h"

#include "../../parsec/ast.h"
#include "../../parsec/char_stream.h"
#include "../../parsec/parsec.h"

#include "../data_structure/token_stream.h"
#include "../data_structure/symbol_table.h"

#include "../combinators/line.h"

#include "code_generate.h"

/*!
 *
 * @param is_imm
 * @param reg_or_imm
 * @param target
 * @return : determine whether it is a register or an immediate value, and
 *           return the result
 */
void reg_imm_helper(bool is_imm, reg_or_imm_t reg_or_imm, uint32_t *target)
{
  if (is_imm)
  {
    set_bit_range(target, reg_or_imm.rot_imm.imm, IMM_START, IMM_END);
    set_bit_range(target, reg_or_imm.rot_imm.amount, ROTATE_START, ROTATE_END);
  }
  else
  {
    set_bit_range(target, reg_or_imm.shift_reg.Rm, RM_START, RM_END);
    set_bit_range(target, reg_or_imm.shift_reg.type, SHIFT_TYPE_START, SHIFT_TYPE_END);
    set_bit_range(target, reg_or_imm.shift_reg.val, SHIFT_CONST_START, SHIFT_CONST_END);
  }
}

/*!
 * TODO
 * @param file_name the file name to output
 * @param token_stream
 * @param symbol_table
 * @param end_address the address of last instruction
 */
void code_generate(char *file_name, TokenStream token_stream,
                   SymbolTable symbol_table, int *end_address)
{
  uint32_t      binary_code;
  instruction_t ins;
  AST           ast;
  int           address;
  FILE *        f_handle = fopen(file_name, "wb");
  if (!f_handle)
  {
    perror("cannot open file");
    exit(EXIT_FAILURE);
  }

  for (token_t *cur_token = token_stream->head; cur_token != NULL;
       cur_token          = cur_token->next)
  {
    if (cur_token->ast == NULL)
    {
      fwrite(&cur_token->imm_val, sizeof(binary_code), 1, f_handle);
      continue;
    }

    ast     = cur_token->ast;
    address = cur_token->address;
    ins = e_instruction(ast, address, token_stream, symbol_table, end_address);
    binary_code = to_bcode(ins);

    fwrite(&binary_code, sizeof(binary_code), 1, f_handle);
  }

  fclose(f_handle);
}

uint32_t to_bcode(instruction_t ins)
{
  switch (ins.tag)
  {
  case ZERO:
  {
    return 0;
  }
  case BRAN:
  {
    return encode_BRANCH(ins);
  }
  case MUL:
  {
    return encode_MUL(ins);
  }
  case PROC:
  {
    return encode_DP(ins);
  }
  case TRANS:
  {
    return encode_TRANS(ins);
  }
  }
}

uint32_t encode_DP(instruction_t instruction)
{
  uint32_t result = 0;
  set_bit_range(&result, instruction.cond, CONDITION_START, CONDITION_END);
  proc_t proc = instruction.word.proc;
  set_bit(&result, proc.is_imm, I_BIT);
  set_bit_range(&result, proc.opcode, OPCODE_START, OPCODE_END);
  set_bit(&result, proc.set_cond, SET_CONDITION_CODES_BIT);
  set_bit_range(&result, proc.Rn, DP_RN_START, DP_RN_END);
  set_bit_range(&result, proc.Rd, DP_RD_START, DP_RD_END);
  reg_imm_helper(proc.is_imm, proc.operand2, &result);
  return result;
}

uint32_t encode_MUL(instruction_t instruction)
{
  uint32_t result = 0;
  set_bit_range(&result, instruction.cond, CONDITION_START, CONDITION_END);
  set_bit_range(&result, 0x9, 4, 7); // 0B1001

  mul_t mul = instruction.word.mul;
  set_bit(&result, mul.is_mla, ACCUMULATE_BIT);
  set_bit(&result, mul.set_cond, SET_CONDITION_CODES_BIT);
  set_bit_range(&result, mul.Rd, MUL_RD_START, MUL_RD_END);
  set_bit_range(&result, mul.Rn, MUL_RM_START, MUL_RN_END);
  set_bit_range(&result, mul.Rs, MUL_RS_START, MUL_RS_END);
  set_bit_range(&result, mul.Rm, MUL_RM_START, MUL_RM_END);
  return result;
}

uint32_t encode_TRANS(instruction_t instruction)
{
  uint32_t result = 0;
  set_bit_range(&result, instruction.cond, 28, 31);
  set_bit_range(&result, 0x1, 26,27);

  trans_t trans = instruction.word.trans;
  set_bit(&result, trans.is_reg, 25);
  set_bit(&result, trans.is_pre, 24);
  set_bit(&result, trans.is_up, 23);
  set_bit(&result, trans.is_load, 20);
  set_bit_range(&result, trans.Rn, 16, 19);
  set_bit_range(&result, trans.Rd, 12, 15);
  reg_imm_helper(!trans.is_reg, trans.offset, &result);
  return result;
}

uint32_t encode_BRANCH(instruction_t instruction)
{
  uint32_t result = 0;
  branch_t bran = instruction.word.branch;
  set_bit_range(&result, bran.offset, 0, 23);
  set_bit_range(&result, instruction.cond, 28, 31);
  set_bit_range(&result,0xa,24,27);

  return result;
}
