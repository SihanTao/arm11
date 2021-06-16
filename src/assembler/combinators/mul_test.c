#include <stdlib.h>
#include <ctype.h>

#include "ast.h"
#include "char_stream.h"

#include "parsec.h"
#include "mul.h"

int main(void)
{
 char * string;
 CharStream char_stream;
 AST ast;
 Parsec parsec;

  string      = "mul r12,r2,r3\n";
  char_stream = &string;

  ast = parse(char_stream, p_mul(), NULL);

  print_ast(ast, 0);
  printf("\n");

  string      = "mla r12,r2,r3,r4\n";
  char_stream = &string;

  parsec = p_mul();

  ast = parse(char_stream, parsec, NULL);

  print_ast(ast, 0);
  printf("\n");


  printf("%d",
        encode_reg( $G($G(ast, "mla"), "Rn")));
  printf("\n");

}
