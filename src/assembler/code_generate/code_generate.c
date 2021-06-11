#include "../../global_utils/types_and_macros.h"

#include "../data_structure/token_stream.h"

// token_t -> instruction

void code_generate(TokenStream token_stream)
{
  uint32_t binary_code;

  for (token_t *cur_token = token_stream->head; cur_token != NULL;
       cur_token          = cur_token->next)
  {
    if (strcmp(cur_token->opcode, "mov") == 0)
    {
      binary_code =  to_bcode_mov(cur_token);
      write_file(binary_code);
    }
  }

  write_file(binary_code);
}

uint32_t to_bcode_mov(cur_token)
{

}

bitfield encode_DP(proc_t instruction)
{
  uint32_t result = 0;
  set_bit_range(&result, instruction.cond, 28, 31);
  set_bit(&result, instruction.iFlag, 25);
  set_bit_range(&result, instruction.opcode, 21, 24);
  set_bit(&result, instruction.set_cond, 20);
  set_bit_range(&result, instruction.Rn, 16, 19);
  set_bit_range(&result, instruction.Rd, 12, 15);
  set_bit_range(&result, instruction.operand2, 0, 11);
}

bitfield encode_MUL(mul_t instruction)
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

bitfield encode_TRANS(trans_t instruction)
{
  uint32_t result = 0;
  set_bit_range(&result, instruction.cond, 28, 31);
  set_bit(&result, instruction.iFlag, 25);
  set_bit(&result, instruction.is_pre, 24);
  set_bit(&result, instruction.is_up, 23);
  set_bit(&result, instruction.is_load, 20);
  set_bit_range(&result, instruction.Rn, 16, 19);
  set_bit_range(&result, instruction.Rd, 12, 15);
  set_bit_range(&result, instruction.offset, 0, 11);
}

bitfield encode_BRANCH(branch_t instruction)
{
  uint32_t result = 0;
  set_bit_range(&result, instruction.cond, 28, 31);
  set_bit_range(&result, instruction.offset, 0, 23);
}
