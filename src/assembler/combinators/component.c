#include <stdlib.h>
#include <ctype.h>

#include "../../parsec/ast.h"
#include "../../parsec/char_stream.h"

#include "../../parsec/parsec.h"

#include "component.h"

Parsec p_number(char *name)
{
  return take_while(name, isnumber);
}

int e_deci(AST number)
{
  printf("HERE 1\n"); //DELETE_MARK
  return atoi($T(number));
}

Parsec p_hexa(char *name)
{
  return make_and(name, match(NULL, "0x"), p_number("hexa"));
}

int e_hexa(AST hexa)
{
  char * val = $TG(hexa, "hexa");
  return strtol(val, NULL, 16);
}

Parsec hash_expr(char * name)
{
  return make_and(name, match(NULL, "#"),
                  make_or("number", p_hexa("hexa"), p_number("deci")));
}

Parsec eq_expr(char * name)
{
  return make_and("eq_expr", match(NULL, "="),
                  make_or("number", p_hexa("hexa"), p_number("deci")));
}

int e_eq_hash_expr(AST hash_expr)
{
  AST number = $G(hash_expr, "number");
  AST deci = $G(number, "deci");
  if (deci)
  {
    return e_deci(deci);
  }

  AST hexa = $G(number, "hexa");
  return e_hexa(hexa);
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

int e_reg(AST reg)
{
  return atoi($TG(reg, "reg_num"));
}

Parsec p_operand2(void)
{
  return make_or("operand2", hash_expr("imm val"), p_reg_e("Rm"));
}

reg_or_imm e_operand2(AST operand2)
{
  reg_or_imm result;
  AST hash_ast = $G(operand2, "imm val");
  if (hash_ast)
  {
    result.is_reg = true;
    result.val = e_eq_hash_expr(hash_ast);
    return result;
  }

  AST reg = $G(operand2, "Rm");
  if (reg)
  {
    result.is_reg = false;
    result.val = e_reg(reg);
    result.shift = 0;
    return result;
  }
}

Parsec p_no_offset(void)
{
  Parsec seqs[3] = { match(NULL, "["), p_reg_e("Rn"), match(NULL, "]") };
  return seq("no offset", seqs, 3);
}

address_t e_no_offset(AST no_offset)
{
  address_t result;
  result.is_post = false;
  result.reg_num = e_reg($G(no_offset, "Rn"));
  result.offset = 0;
  return result;
}

Parsec p_has_offset(void)
{
  Parsec seqs[4] = {match(NULL, "]"), p_reg_i("Rn"), hash_expr("offset"), match(NULL, "]")};
  return seq("has offset", seqs, 4);
}

address_t e_has_offset(AST has_offset)
{
  address_t result;
  result.is_post = false;
  result.reg_num = e_reg($G(has_offset, "Rn"));
  result.offset = e_eq_hash_expr($G(has_offset, "offset"));
  return result;
}

Parsec p_pre_index(void)
{
  return make_or("pre index", p_no_offset(), p_has_offset());
}

address_t e_pre_index(AST pre_index)
{
  AST no_offset = $G(pre_index, "no offset");
  if (no_offset)
  {
    return e_no_offset(no_offset);
  }

  AST has_offset = $G(pre_index, "has offset");
  if (has_offset)
  {
    return e_has_offset(has_offset);
  }
}

Parsec p_post_index(void)
{
  Parsec seqs[3] = { p_no_offset(), match(NULL, ","), hash_expr("offset") };
  return seq("post index", seqs, 3);
}

address_t e_post_index(AST post_index)
{
  address_t result = e_no_offset($G(post_index, "no offset"));
  result.is_post = true;
  result.offset = e_eq_hash_expr($G(post_index, "offset"));
  return result;
}

Parsec p_address(void)
{
  return make_or("address", p_post_index(), p_pre_index());
}

address_t e_address(AST address)
{
  AST post_index = $G(address, "post index");
  if (post_index)
  {
    return e_post_index(post_index);
  }

  AST pre_index = $G(address, "pre index");
  if (pre_index)
  {
    return e_pre_index(pre_index);
  }
}

