#include <stdlib.h>
#include <ctype.h>

#include "ast.h"
#include "char_stream.h"

#include "parsec.h"



int main(void)
{
 char * string;
 CharStream char_stream;
 AST ast;
 Parsec parsec;

  string      = "r12,";
  char_stream = &string;

  ast = parse(char_stream, p_reg_i("Rm"), NULL);

  print_ast(ast, 0);
  printf("\n");



}
