#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "../data_structure/token_stream.h"
#include "../data_structure/symbol_table.h"

#include "parse_helper.h"

#include "../parse/tokenize.h"

void preprocess_and_gen_sym_table(char **loaded_file, TokenStream token_stream,
                                  SymbolTable symbol_table)
{
  char *line;
  int   address;
  for (int i = 0; loaded_file[i] != NULL; i++)
  {
    line = loaded_file[i];
    if (is_label_line(line))
    {
      // corresponding to the address of next instruction
      add_symbol_table(line, address + ADDRESS_INTERVAL, symbol_table);
    }
    else
    {
      tokenize(line, address, token_stream);
      address += ADDRESS_INTERVAL;
    }
  }
}

bool is_label_line(char *line) { return line[strlen(line) - 1] == ':'; }

NotSure allocate_address(TokenStream token_stream, SymbolTable symbol_table)
{
  operand_t *label_operand_ptr;
  int        address;

  for (token_t *cur = token_stream->head; cur != NULL; cur = cur->next)
  {
    label_operand_ptr = get_label_ptr(cur, symbol_table);

    if (label_operand_ptr != NULL)
    {
      address = find_symbol_table(label_operand_ptr->operand_data.letters,
                                  symbol_table);
      label_operand_ptr->tag = NUMBER;
      // TODO : check the logic of
      // free(label_operand_ptr->operand_data.letters);
      label_operand_ptr->operand_data.number = address;
    }
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
