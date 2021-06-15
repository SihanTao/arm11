#include <stdlib.h>
#include <ctype.h>

#include "ast.h"
#include "char_stream.h"

#include "parsec.h"

Parsec p_number(char *name)
{
  return take_while(name, isnumber);
}

Parsec p_reg_i(char *name)
{
  return make_and(name, match(NULL, "r"),
                  make_and(NULL, p_number("reg_num"), match(NULL, ",")));
}

Parsec p_reg_e(char *name)
{
  return make_and(name, match(NULL, "r"),
                  p_number("reg_num"));
}

int encode_reg(AST ast)
{
  char * reg_num = $TG(ast, "reg_num");
  return atoi(reg_num);
}


int encode_mul(AST ast)
{
  AST mul = $G(ast, "mul");
  if (mul)
  {
    // some logic
  }
  AST mla = $G(ast, "mla");
  if (mla)
  {
    // some logic
  }
  return 0;
}

Parsec p_mul()
{
  Parsec sequence1[5]
      = { match(NULL, "mul "), p_reg_i("Rd"), p_reg_i("Rm"), p_reg_e("Rs")};
  Parsec sequence2[6] = { match(NULL, "mla "), p_reg_i("Rd"), p_reg_i("Rm"),
                          p_reg_i("Rs"), p_reg_e("Rn")};
  return make_or("mul_mla", seq("mul", sequence1, 4),
                 seq("mla", sequence2, 5));
}

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
