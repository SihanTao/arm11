#include <stdbool.h>
#include <string.h>

#include "../data_structure/token_stream.h"
#include "../data_structure/symbol_table.h"

#include "parse_helper.h"

void preprocess_and_gen_sym_table(char **loaded_file, TokenStream token_stream,
                                  SymbolTable symbol_table)
{
  char *line;
  for (int i = 0; loaded_file[i] != NULL; i++)
  {
    line = loaded_file[i];
    if (is_label(line))
    {
      add_symbol_table(line, symbol_table);
    }
    else
    {
      tokenize(line, token_stream);
    }
  }
}

bool is_label(char *line) { return false; }

void add_symbol_table(char *line, SymbolTable symbol_table) { return; }

NotSure substitute_in(TokenStream token_queue, SymbolTable symbol_table)
{
  return;
}