#include <stdlib.h>
#include <stdio.h>

#include "file_loader/assembler_file_loader.h"

#include "../data_structure/token_stream.h"
#include "../data_structure/symbol_table.h"

#include "../parse/parse.h"

int main(int argc, char **argv)
{
  if (argc != 3)
  {
    perror("The number of argument is invalid.");
    exit(EXIT_FAILURE);
  }

  char *source = argv[1];
  char *output = argv[2];

  TokenStream token_stream = init_token_stream();
  SymbolTable symbol_table = init_symbol_table();

  char **loaded_file = load_source_file(source);

  parse(loaded_file, token_stream, symbol_table);

  code_generate(token_stream, symbol_table);
}



