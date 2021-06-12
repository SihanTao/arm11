#include <stdbool.h>
#include <stdlib.h>

#include "../data_structure/token_stream.h"
#include "../data_structure/symbol_table.h"

#include "parse.h"
#include "../parse/tokenize.h"

static bool is_label_line(char *line);

void parse(char **loaded_file, TokenStream token_stream, SymbolTable symbol_table)
{
  TokenStream token_stream = init_token_stream();
  SymbolTable symbol_table = init_symbol_table();

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