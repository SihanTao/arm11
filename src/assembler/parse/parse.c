#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "../data_structure/token_stream.h"
#include "../data_structure/symbol_table.h"

#include "parse.h"
#include "../parse/tokenize.h"


static bool is_label_line(char *line);

/*!
 * TODO
 * @param file_name
 * @param token_stream
 * @param symbol_table
 * @return  
 */
void parse(char *file_name, TokenStream token_stream, SymbolTable symbol_table)
{
  FILE *f_handle = fopen(file_name, "r");
  if (!f_handle)
  {
    perror("cannot open file");
    exit(EXIT_FAILURE);
  }
  int   address;
  char *buffer = malloc(MAX_LINE_LENGTH);

  fgets(buffer, MAX_LINE_LENGTH, f_handle);

  if (is_label_line(buffer))
  {
    // corresponding to the address of next instruction
    buffer[strlen(buffer) - 1] = '\0';
    add_symbol_table(buffer, address + ADDRESS_INTERVAL, symbol_table);
  }
  else
  {
    add_token_stream(tokenize(buffer), token_stream);
    address += ADDRESS_INTERVAL;
  }

  fclose(f_handle);
  free(buffer);
}

/*!
 * 
 * @param line
 * @return a bool value that represents whether the line is label or not 
 */
bool is_label_line(char *line) { return line[strlen(line) - 1] == ':'; }
