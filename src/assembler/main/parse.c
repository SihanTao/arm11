#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../../parsec/char_stream.h"
#include "../../parsec/ast.h"
#include "../../parsec/parsec.h"

#include "../../global_utils/types_and_macros.h"

#include "../data_structure/token_stream.h"
#include "../data_structure/symbol_table.h"

#include "../combinators/line.h"

#include "parse.h"

/*!
 * The first pass is `parse` which convert instructions into ASTs and add them
 * into token stream, and make a map between label and address which is stored
 * in symbol table.
 * @param file_name the file name to read from
 * @param token_stream the token_stream to add to
 * @param symbol_table the symbol table to add to
 * @param end_address the pointer takes the address of last intruction
 */
void parse(char *file_name, TokenStream token_stream, SymbolTable symbol_table,
           int *end_address)
{
  FILE *f_handle = fopen(file_name, "r");
  if (!f_handle)
  {
    perror("cannot open file");
    exit(EXIT_FAILURE);
  }
  int        address;
  char *     buffer = malloc(MAX_LINE_LENGTH);
  CharStream char_stream;
  Parsec     line_parser = p_line();

  while (fgets(buffer, MAX_LINE_LENGTH, f_handle))
  {
    if (strcmp(buffer, "\n") == 0)
    {
      continue;
    }
    char_stream = &buffer;

    AST result  = perform_parse(char_stream, line_parser, NULL);
    AST label = $G(result, "label");

    if (label)
    {
      add_symbol_table(e_label(label), address, symbol_table);
    }
    else // is instruction
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