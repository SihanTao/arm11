#include <ctype.h>

#include "../../mini_lexer/ast.h"
#include "../../mini_lexer/char_stream.h"
#include "../../mini_lexer/parsec.h"

Parsec line()
{
  return make_or("line", label(), instruction());
}

Parsec label()
{
  return make_and("label", take_while("label_name", isalpha),
                  match(NULL, ":"));
}

Parsec instruction()
{
  Parsec sub_types[6]
      = { p_arith(), p_mov(), p_test(), p_bran(), p_trans(), p_mul() };
  return alt("instruction", sub_types, 6);
}

Parsec p_arith()
{
  Parsec opcodes[6]
      = { match("add", "add "), match("sub", "sub "), match("rsb", "rsb "),
          match("and", "and "), match("eor", "eor "), match("orr", "orr ") };
  Parsec sequence[4]
      = { alt("opcode", opcodes, 6), p_reg("Rd"), p_reg("Rn"), p_operand2() };
  return seq("arith", sequence, 4);
}

Parsec p_mov()
{
  Parsec sequence[3] = { match("mov", "mov "), p_reg("Rd"), p_operand2() };
  return seq("mov", sequence, 3);
}

Parsec p_test(){ Parsec }

Parsec p_reg(char *name)
{
  return make_and(name, match(NULL, "r"),
                  make_and(NULL, p_number("reg_num"), match(NULL, ",")));
}

Parsec p_number(char *name)
{
  return take_while(name, isnumber);
}

Parsec p_operand2()
{
  return make_or("operand2", p_shifted_reg(), p_hash_expr());
}

Parsec p_shifted_reg()
{
  return make_or("no_shift_reg", p_reg("Rm"),
                 make_and("shifted_reg", p_reg("Rm"), p_shift()));
}

Parsec p_shift(){ return make_or() }

Parsec p_mul()
{
  Parsec sequence1[4]
      = { match("mul", "mul "), p_reg("Rd"), p_reg("Rm"), p_reg("Rs") };
  Parsec sequence2[5] = { match("mla", "mla "), p_reg("Rd"), p_reg("Rm"),
                          p_reg("Rs"), p_reg("Rn") };
  return make_or("mul_mla", seq("mul", sequence1, 4),
                 seq("mla", sequence2, 5));
}

Parsec p_trans()
{
  Parsec sequence[3] return make_or("trans", ldr(), str());
}

Parsec ldr()
{
  return
}