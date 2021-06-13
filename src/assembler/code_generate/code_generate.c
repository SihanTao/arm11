#include "../../global_utils/types_and_macros.h"

#include "../data_structure/token_stream.h"
#include "../data_structure/symbol_table.h"

#include "../code_generate/allocate_address.h"

//#include "../utils/write_file.h"

#include <string.h>
// token_t -> instruction

void code_generate(TokenStream token_stream, SymbolTable symbol_table)
{
  uint32_t binary_code;

  for (token_t *cur_token = token_stream->head; cur_token != NULL;
       cur_token          = cur_token->next)
  {
    allocate_address(cur_token, symbol_table);
    if (strcmp(cur_token->opcode, "mov") == 0)
    {
      binary_code =  to_bcode_mov(cur_token);
      write_file(binary_code);
    }
  }

  write_file(binary_code);
}