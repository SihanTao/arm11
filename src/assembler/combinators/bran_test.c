#include <stdlib.h>
#include <ctype.h>

#include "../../parsec/ast.h"
#include "../../char_stream.h"

#include "../../parsec.h"

#include "component.h"
#include "bran.h"

int main(void)
{
  char *     string;
  CharStream char_stream;
  AST        ast;
  Parsec     parsec;

  string      = "b wait";
  char_stream = &string;
  ast         = parse(char_stream, p_bran(), NULL);
  print_ast(ast, 0);
  printf("\n");

  string      = "bal wait";
  char_stream = &string;
  ast         = parse(char_stream, p_bran(), NULL);
  print_ast(ast, 0);
  printf("\n");

  string      = "beq wait";
  char_stream = &string;
  ast         = parse(char_stream, p_bran(), NULL);
  print_ast(ast, 0);
  printf("\n");
}
