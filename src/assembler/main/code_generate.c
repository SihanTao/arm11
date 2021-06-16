#include <stdio.h>

#include "../../global_utils/types_and_macros.h"

#include "../../parsec/ast.h"

#include "../data_structure/token_stream.h"
#include "../data_structure/symbol_table.h"

#include "../combinators/line.h"

/*!
 * TODO
 * @param token_stream
 * @param symbol_table
 *
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
      write_file(cur_token->imm_val);
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

uint32_t encode_DP(proc_t instruction)
{
  uint32_t result = 0;
  set_bit_range(&result, instruction.cond, 28, 31);
  set_bit(&result, instruction.is_imm, 25);
  set_bit_range(&result, instruction.opcode, 21, 24);
  set_bit(&result, instruction.set_cond, 20);
  set_bit_range(&result, instruction.Rn, 16, 19);
  set_bit_range(&result, instruction.Rd, 12, 15);
  reg_imm_helper(instruction.is_imm, instruction.operand2, result);
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
  set_bit(&result, instruction.is_reg, 25);
  set_bit(&result, instruction.is_pre, 24);
  set_bit(&result, instruction.is_up, 23);
  set_bit(&result, instruction.is_load, 20);
  set_bit_range(&result, instruction.Rn, 16, 19);
  set_bit_range(&result, instruction.Rd, 12, 15);
  reg_imm_helper(!instruction.is_reg, instruction.offset, result);
}

uint32_t encode_BRANCH(branch_t instruction)
{
  uint32_t result = 0;
  set_bit_range(&result, instruction.cond, 28, 31);
  set_bit_range(&result, instruction.offset, 0, 23);
}

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
    set_bit_range(target, reg_or_imm.rot_imm.imm, 0, 7);
    set_bit_range(target, reg_or_imm.rot_imm.amount, 8, 11);
  }
  else
  {
    set_bit_range(target, reg_or_imm.shift_reg.Rm, 0, 3);
    set_bit_range(target, reg_or_imm.shift_reg.type, 5, 6);
    set_bit_range(target, reg_or_imm.shift_reg.val, 7, 11);
  }
}