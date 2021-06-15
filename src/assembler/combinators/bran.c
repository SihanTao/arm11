#include <stdlib.h>
#include <ctype.h>

#include "../../parsec/ast.h"
#include "../../char_stream.h"

#include "../../parsec.h"

#include "component.h"
#include "bran.h"

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

Parsec bran_cond(void)
{
  Parsec alts[8] = {
    match("beq", "beq "),
    match("bne", "bne "),
    match("bge", "bge "),
    match("blt", "blt "),
    match("bgt", "bgt "),
    match("ble", "ble "),
    match("bal", "bal "),
    match("bal", "b ")
  };
  return alt("bran_cond", alts, 8);
}

Parsec p_bran(void)
{
  return make_and("bran", bran_cond(), take_while("label", isalpha));
}