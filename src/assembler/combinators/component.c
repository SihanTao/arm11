#include <stdlib.h>
#include <ctype.h>

#include "../../parsec/ast.h"
#include "../../parsec/char_stream.h"

#include "../../parsec/parsec.h"

#include "../../global_utils/types_and_macros.h"

#include "component.h"

/*!
 *
 * @param target
 * @return : determine whether the target is an immediate value or not
 */
static bool is_valid_imm(uint32_t target)
{
  uint32_t mask = 0X000000FF;
  return (target & mask) == target;
}

/*!
 *
 * @param target
 * @param rotation_amount
 * @param imm
 * @return the result after reverse rotate
 */
bool reverse_rotate(uint32_t target, int *rotation_amount, uint32_t *imm)
{
  if (is_valid_imm(target))
  {
    *rotation_amount = 0;
    *imm             = target;
    return true;
  }

  for (int i = 1; i < 16; i++)
  {
    target = (target << 2) | (target >> 30); // rotate left by 2.
    if (is_valid_imm(target))
    {
      *rotation_amount = i;
      *imm             = target;
      return true;
    }
  }
  perror("invalid imm");
  return false;
}

bool is_space(char target)
{
  return (target == ' ');
}

/*!
 * @param name
 * @return a parser combinator of number;
 */
Parsec p_number(char *name)
{
  return take_while(name, isnumber);
}

/*!
 * @return an encoded decimal.
 */
int e_deci(AST number)
{
  return atoi($T(number));
}

/*!
 * @return a parser combinator of Hexadecimal.
 */
Parsec p_hexa(char *name)
{
  // a hexadecimal number can be either positive or negative
  return make_or(
      name, make_and("pos", match(NULL, "0x"), take_while("hexa", isalnum)),
      make_and("neg", match(NULL, "-0x"), take_while("hexa", isalnum)) );
}

/*!
 * @return an encoded Hexadecimal.
 */
int e_hexa(AST hexa)
{
  AST pos = $G(hexa, "pos");
  if (pos)
  {
    char * val = $TG(pos, "hexa");
    return strtol(val, NULL, 16);
  }

  AST neg = $G(hexa, "neg");
  char * val = $TG(neg, "hexa");
  return -strtol(val, NULL, 16);
}

/*!
 * a hash expression is '#' + (decimal number | hexa number)
 * @return
 */
Parsec p_hash_expr(char *name)
{
  return make_and(name, match(NULL, "#"),
                  make_or("number", p_hexa("hexa"), p_number("deci")));
}

/*!
 * a hash expression is '=' + (decimal number | hexa number)
 * @return
 */
Parsec p_eq_expr(char *name)
{
  return make_and(name, match(NULL, "="),
                  make_or("number", p_hexa("hexa"), p_number("deci")));
}

/*!
 * encode # or = expr to int
 * @return
 */
int e_eq_hash_expr(AST hash_expr)
{
  AST number = $G(hash_expr, "number");
  AST deci   = $G(number, "deci");
  if (deci)
    return e_deci(deci);
  AST hexa = $G(number, "hexa");
  return e_hexa(hexa);
}

/*!
 * a not end register is 'r'+ number + ','
 * @return
 */
Parsec p_reg_i(char *name)
{
  Parsec alts[3] = { match(NULL, ", "), match(NULL, ","), match(NULL, ",  ") };
  Parsec seq1[3]
      = { match(NULL, "r"), p_number("reg_num"), alt(NULL, alts, 3) };
  return seq(name, seq1, 3);
}
/*!
 * an end register is 'r'+ number
 * @return
 */
Parsec p_reg_e(char *name)
{
  return make_and(name, match(NULL, "r"), p_number("reg_num"));
}

/*!
 * encode register to no_reg_t
 * @return
 */
no_reg_t e_reg(AST reg)
{
  return atoi($TG(reg, "reg_num"));
}

/*!
 * @return a parser combinator of operand2.
 */
Parsec p_operand2(void)
{
  return make_or("operand2", p_hash_expr("imm val"), p_reg_e("Rm"));
}

/*!
 * encode operand2 AST to reg_or_imm_t
 * @return
 */
reg_or_imm_t e_operand2(AST operand2, bool *is_imm)
{
  reg_or_imm_t result;
  AST          hash_ast = $G(operand2, "imm val");
  if (hash_ast)
  {
    int      amount;
    uint32_t imm;
    printf("e_eq_hash_expr(hash_ast) :>> %d\n",
           e_eq_hash_expr(hash_ast)); // DELETE_MARK
    reverse_rotate(e_eq_hash_expr(hash_ast), &amount, &imm);
    result.rot_imm.amount = amount;
    result.rot_imm.imm    = imm;
    *is_imm               = true;
    return result;
  }

  AST reg = $G(operand2, "Rm");
  if (reg)
  {
    *is_imm               = false;
    result.shift_reg.Rm   = e_reg(reg);
    result.shift_reg.val  = 0;
    result.shift_reg.type = LSL;
    return result;
  }
}

/*!
 * @return a parser combinator of no offset.
 */
Parsec p_no_offset(void)
{
  Parsec seqs[3] = { match(NULL, "["), p_reg_e("Rn"), match(NULL, "]") };
  return seq("no offset", seqs, 3);
}

/*!
 * @return an encoded no offset.
 */
address_t e_no_offset(AST no_offset)
{
  address_t result;
  result.is_post           = false;
  result.is_eq_expr        = false;
  result.Rn                = e_reg($G(no_offset, "Rn"));
  result.offset_or_eq_expr = 0;
  return result;
}

/*!
 * @return a parser combinator of has offset.
 */
Parsec p_has_offset(void)
{
  Parsec seqs[4] = { match(NULL, "["), p_reg_i("Rn"), p_hash_expr("offset"),
                     match(NULL, "]") };
  return seq("has offset", seqs, 4);
}

/*!
 * @return an encoded has offset.
 */
address_t e_has_offset(AST has_offset)
{
  address_t result;
  result.is_post           = false;
  result.is_eq_expr        = false;
  result.Rn                = e_reg($G(has_offset, "Rn"));
  result.offset_or_eq_expr = e_eq_hash_expr($G(has_offset, "offset"));
  return result;
}

/*!
 * @return a parser combinator of pre index.
 */
Parsec p_pre_index(void)
{
  return make_or("pre index", p_no_offset(), p_has_offset());
}

/*!
 * @return an encoded pre insex.
 */
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

/*!
 * @return a parser combinator of pos index.
 */
Parsec p_post_index(void)
{
  Parsec seqs[3] = { p_no_offset(), match(NULL, ","), p_hash_expr("offset") };
  return seq("post index", seqs, 3);
}

/*!
 * @return an encode pos index.
 */
address_t e_post_index(AST post_index)
{
  address_t result         = e_no_offset($G(post_index, "no offset"));
  result.is_eq_expr        = true;
  result.is_post           = true;
  result.offset_or_eq_expr = e_eq_hash_expr($G(post_index, "offset"));
  return result;
}

/*!
 * @return a parser combinator of address.
 */
Parsec p_address(void)
{
  Parsec alts[3] = { p_eq_expr("eq expr"), p_post_index(), p_pre_index() };
  return alt("address", alts, 3);
}

/*!
 * @return an encoded '=' expression.
 */
address_t e_eq_expr(AST imm)
{
  address_t result;
  result.is_eq_expr        = true;
  result.is_post           = false;
  result.offset_or_eq_expr = e_eq_hash_expr(imm);
  result.Rn                = 0xf;
  return result;
}

/*!
 * @return an encoded address.
 */
address_t e_address(AST address)
{
  AST imm = $G(address, "eq expr");
  if (imm)
  {
    return e_eq_expr(imm);
  }

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
