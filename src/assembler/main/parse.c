#include <stdlib.h>
#include <stdio.h>

#include "../data_structure/token_stream.h"
#include "../data_structure/symbol_table.h"

#include "../../parsec/char_stream.h"
#include "../../parsec/ast.h"
#include "../../parsec/parsec.h"

#include "../combinators/line.h"

#define ADDRESS_INTERVAL (4)
#define MAX_LINE_LENGTH  (512)

/*!
 * TODO
 * @param file_name
 * @param token_stream
 * @param symbol_table
 * @return
 */
void parse(char *file_name, TokenStream token_stream, SymbolTable symbol_table, int *end_address)
{
  FILE *f_handle = fopen(file_name, "r");
  if (!f_handle)
  {
    perror("cannot open file");
    exit(EXIT_FAILURE);
  }
  int  address;
  char buffer[MAX_LINE_LENGTH];
  Parsec line_parser = p_line();

  while (fgets(buffer, MAX_LINE_LENGTH, f_handle))
  {
    AST result = perform_parse(buffer, line_parser, NULL);

    AST label = $G(result, "label");
    if (label)
    {
      add_symbol_table(e_label(label), address + ADDRESS_INTERVAL,
                       symbol_table);
      free_ast(result);
    }
    else
    {
      AST      instruction = $G(result, "instruction");
      token_t *token       = malloc(sizeof(token_t));
      token->ast           = instruction;
      token->address       = address;
      token->imm_val       = -1;
      add_token_stream(token, token_stream);
      address += ADDRESS_INTERVAL;
    }

    result->child = NULL;
    free_ast(result);
  }

  free_parsec(line_parser);
  fclose(f_handle);
  *end_address = address;
}