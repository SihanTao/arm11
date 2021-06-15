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

// Parsec operand2()
// {
//   return make_or(hash_expr(), shifted_reg());
// }

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
  Parsec abc = match("first", "abc");
  Parsec cde = match("second", "cde");
  Parsec ghf = match("third", "ghf");

  Parsec and1 = make_and("and1", abc, cde);
  Parsec and2 = make_and("and2", ghf, and1);

  print_pc(and2, 0);
  printf("\n");

  char * string      = "ghfabccde";
  char **char_stream = &string;
  AST    ast         = parse(char_stream, and2, NULL);
  print_ast(ast, 0);
  printf("\n");

  string          = "ghfabccde";
  char_stream     = &string;
  Parsec asdf     = match("asdfasdf", "asdf");
  Parsec make_or1 = make_or("make_or", asdf, and2);
  ast             = parse(char_stream, make_or1, NULL);
  print_ast(ast, 0);
  printf("\n");

  string          = "ghfabccde";
  char_stream     = &string;
  Parsec make_or2 = make_or("make_or", and2, asdf);
  ast             = parse(char_stream, make_or2, NULL);

  print_ast(ast, 0);
  printf("\n");

  string      = "asdf";
  char_stream = &string;
  ast         = parse(char_stream, make_or2, NULL);

  print_ast(ast, 0);
  printf("\n");

  Parsec and_components[3]
      = { match("one", "one"), match("two", "two"), match("three", "three") };

  Parsec my_and = seq("my add", and_components, 3);
  print_pc(my_and, 0);
  printf("\n");

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

  abc = p_mul();

  ast = parse(char_stream, abc, NULL);

  print_ast(ast, 0);
  printf("\n");


  printf("%d",
        encode_reg( $G($G(ast, "mla"), "Rn")));
  printf("\n");

}
