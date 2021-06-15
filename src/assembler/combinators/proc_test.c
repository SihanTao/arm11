#include <stdlib.h>
#include <ctype.h>

#include "../../parsec/ast.h"
#include "../../char_stream.h"

#include "../../parsec.h"

#include "component.h"
#include "proc.h"

int main(void)
{
  char *     string;
  CharStream char_stream;
  AST        ast;
  Parsec     parsec;

  string      = "add r1,r2,#3";
  char_stream = &string;
  ast = parse(char_stream, p_arith(), NULL);
  print_ast(ast, 0);
  printf("\n");

  string      = "add r1,r2,#0x14";
  char_stream = &string;
  ast = parse(char_stream, p_proc(), NULL);
  print_ast(ast, 0);
  printf("\n");
}
