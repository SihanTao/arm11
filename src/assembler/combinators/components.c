#include <stdlib.h>
#include <ctype.h>

#include "../../parsec/ast.h"
#include "../../char_stream.h"

#include "../../parsec.h"

#include "component.h"

Parsec p_number(char *name)
{
  return take_while(name, isnumber);
}

int e_deci(AST number)
{
  return atoi($T(number));
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

Parsec eq_expr(void)
{
  return make_and("eq_expr", match(NULL, "="),
                  make_or("number", p_hexa("hexa"), p_number("deci")));
}

Parsec p_reg_i(char *name)
{
  return make_and(name, match(NULL, "r"),
                  make_and(NULL, p_number("reg_num"), match(NULL, ",")));
}

Parsec p_reg_e(char *name)
{
  return make_and(name, match(NULL, "r"), p_number("reg_num"));
}

Parsec no_offset(void)
{
  Parsec seqs[3] = {
    match(NULL, "["),
    p_reg_e("Rn"),
    match(NULL, "]")
  };
  return seq("no offset", seqs, 3);
}

Parsec pre_index(void)
{
  Parsec seqs[4] = {
    match(NULL, "["),
    p_reg_i("Rn"),
    hash_expr(),
    match(NULL, "]"),
  };
  Parsec has_offset = seq("has offset", seqs, 4);
  return make_or("pre index", no_offset(), has_offset);
}

Parsec post_index(void)
{
  Parsec seqs[3] = {
    no_offset(),
    match(NULL, ","),
    hash_expr()
  };
  return seq("post index", seqs, 3);
}

Parsec address(void)
{
  return make_or("address", post_index(), pre_index());
}

Parsec operand2(void)
{
  return make_or("operand2", hash_expr(), p_reg_e("Rm"));
}