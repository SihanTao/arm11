#include "../data_structure/token_stream.h"
#include "../data_structure/symbol_table.h"

#include "parse.h"
#include "../parse/parse_helper.h"

NotSure parse(char **loaded_file)
{
  TokenStream token_stream; // = init_token_stream();
  SymbolTable symbol_table; //= init_symbol_table();

  preprocess_and_gen_sym_table(loaded_file, token_stream, symbol_table);

  return substitute_in(token_stream, symbol_table);
}
