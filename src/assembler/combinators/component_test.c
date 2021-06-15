#include <stdlib.h>
#include <ctype.h>

#include "../../parsec/ast.h"
#include "../../char_stream.h"
#include "../../parsec.h"

#include "component.h"

int main(void)
{
  char *     string;
  CharStream char_stream;
  AST        ast;
  Parsec     parsec;

  string      = "#10";
  char_stream = &string;

  ast = parse(char_stream, operand2(), NULL);

  print_ast(ast, 0);
  printf("\n");

  string      = "#0x10";
  char_stream = &string;
  ast         = parse(char_stream, operand2(), NULL);
  print_ast(ast, 0);
  printf("\n");

  string      = "r10";
  char_stream = &string;
  ast         = parse(char_stream, operand2(), NULL);
  print_ast(ast, 0);
  printf("\n");

  string      = "[r10]";
  char_stream = &string;
  ast         = parse(char_stream, address(), NULL);
  print_ast(ast, 0);
  printf("\n");

   string      = "[r10, #10]";
  char_stream = &string;
  ast         = parse(char_stream, address(), NULL);
  print_ast(ast, 0);
  printf("\n");

   string      = "[r10],#10";
  char_stream = &string;
  ast         = parse(char_stream, address(), NULL);
  print_ast(ast, 0);
  printf("\n");
}
