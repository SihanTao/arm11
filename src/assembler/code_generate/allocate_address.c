#include <stdlib.h>

#include "../data_structure/token_stream.h"
#include "../data_structure/symbol_table.h"

#include "allocate_address.h"

static operand_t *get_label_ptr(Token token, SymbolTable symbol_table);

void allocate_address(Token token, SymbolTable symbol_table)
{
  operand_t *label_operand_ptr;
  int        address;

  label_operand_ptr = get_label_ptr(token, symbol_table);

  if (label_operand_ptr != NULL)
  {
    address = find_symbol_table(label_operand_ptr->operand_data.letters,
                                symbol_table);
    label_operand_ptr->tag                 = NUMBER;
    label_operand_ptr->operand_data.number = address;
  }
  return;
}

operand_t *get_label_ptr(token_t *token, SymbolTable symbol_table)
{
  operand_t *operands = token->operands;
  for (int i = 0; i < token->num_operand; i++)
  {
    if (operands[i].tag == STRING
        && find_symbol_table(operands[i].operand_data.letters, symbol_table))
    {
      return (operands + i);
    }
  }

  return NULL;
}