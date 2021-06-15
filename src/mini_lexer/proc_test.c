#include <stdlib.h>
#include <ctype.h>

#include "ast.h"
#include "char_stream.h"

#include "parsec.h"

Parsec p_number(char *name)
{
  return take_while(name, isnumber);
}

Parsec p_hexa(char *name)
{
  return make_and(name, match(NULL, "0x"), p_number("hexa"));
}

Parsec hash_expr(void)
{
  return make_and("hash_expr", match(NULL, "#"),
                  make_or("number", p_hexa("hexa"), p_number("deci")));
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


Parsec operand2(void)
{
  return make_or("operand2", hash_expr(), p_reg_e("Rm"));
}

Parsec p_arith(void)
{
  Parsec alts[6] = {
    match("and", "and "),
    match("eor", "eor "),
    match("sub", "sub "),
    match("rsb", "rsb "),
    match("add", "add "),
    match("orr", "orr ")
  };
  Parsec sequence[4] = {
    alt("opcode", alts, 6),
    p_reg_i("Rd"),
    p_reg_i("Rn"),
    operand2()
  };
  return seq("arith", sequence, 4);
}

Parsec p_mov(void)
{
  return make_and("mov", match("opcode", "mov"), make_and(NULL, p_reg_i("Rd"), operand2()));
}

Parsec p_cmp_tst(void)
{
    Parsec alts[6] = {
    match("tst", "tst "),
    match("teq", "teq "),
    match("cmp", "cmp ")
  };
  Parsec sequence[4] = {
    alt("opcode", alts, 6),
    p_reg_i("Rn"),
    operand2()
  };
  return seq("arith", sequence, 4);
}

Parsec p_proc(void)
{
  Parsec alts[3] = {
    p_arith(),
    p_mov(),
    p_cmp_tst()
  };
  return alt("proc", alts, 3);
}

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
