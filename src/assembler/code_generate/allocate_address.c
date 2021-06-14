#include <stdlib.h>

#include "../data_structure/token_stream.h"
#include "../data_structure/symbol_table.h"

#include "allocate_address.h"

static operand_t *get_label_ptr(Token token, SymbolTable symbol_table);

/*!
 * TODO
 * @param token
 * @param symbol_table
 * @return : allocate address of the given token
 */
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

/*!
 * TODO
 * @param token
 * @param symbol_table
 * @return the label pointer of the given token
 */
operand_t *get_label_ptr(token_t *token, SymbolTable symbol_table)
{
  operand_t *cur_operand = token->operands;
  while (cur_operand != NULL)
  {
    if (cur_operand->tag == STRING
        && find_symbol_table(cur_operand->operand_data.letters, symbol_table))
    {
      return cur_operand;
    }

    cur_operand = cur_operand->next;
  }

  return NULL;
}
