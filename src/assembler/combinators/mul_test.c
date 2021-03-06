#include <stdlib.h>
#include <ctype.h>

#include "../../parsec/ast.h"
#include "../../parsec/char_stream.h"
#include "../../parsec/parsec.h"

#include "../../global_utils/types_and_macros.h"

#include "component.h"
#include "mul.h"

int main(void)
{
  char *        string;
  CharStream    char_stream;
  AST           ast;
  Parsec        parsec;
  instruction_t ins;
  mul_t         mul;

  string      = "mul r12,r2,r3\n";
  char_stream = &string;
  ast         = perform_parse(char_stream, p_mul(), NULL);
  print_ast(ast, 0);
  printf("\n");
  ins = e_mul(ast);
  printf("ins.tag :>> %d\n", ins.tag);
  printf("ins.cond :>> %d\n", ins.cond);
  mul = ins.word.mul;
  printf("mul.Rn :>> %d\n", mul.Rn);
  printf("mul.Rd :>> %d\n", mul.Rd);
  printf("mul.Rm :>> %d\n", mul.Rm);
  printf("mul.is_mla :>> %d\n", mul.is_mla);
  printf("mul.Rs :>> %d\n", mul.Rs);

  string      = "mla r12,r2,r3,r4\n";
  char_stream = &string;

  ast = perform_parse(char_stream, p_mul(), NULL);

  print_ast(ast, 0);
  ins = e_mul(ast);
  printf("ins.tag :>> %d\n", ins.tag);
  printf("ins.cond :>> %d\n", ins.cond);
  mul = ins.word.mul;
  printf("mul.Rn :>> %d\n", mul.Rn);
  printf("mul.Rd :>> %d\n", mul.Rd);
  printf("mul.Rm :>> %d\n", mul.Rm);
  printf("mul.is_mla :>> %d\n", mul.is_mla);
  printf("mul.Rs :>> %d\n", mul.Rs);
}
