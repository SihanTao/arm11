#include <stdlib.h>
#include <ctype.h>

#include "../../parsec/ast.h"
#include "../../parsec/char_stream.h"

#include "../../parsec/parsec.h"

#include "../../global_utils/types_and_macros.h"

#include "component.h"

static Parsec p_number(char *name);
static Parsec p_hexa(char *name);
static Parsec p_no_offset(void);
static Parsec p_pre_index(void);
static Parsec p_post_index(void);
static address_t e_pre_index(AST pre_index, bool* is_imm, bool* is_up);
static address_t e_post_index(AST post_index, bool* is_imm, bool* is_up);

/*!
 * determine whether the target is an immediate value or not
 * @param target
 * @return
 */
static bool is_valid_imm(uint32_t target)
{
  uint32_t mask = 0X000000FF;
  return (target & mask) == target;
}

/*!
 * convert immediate value into a rotated value
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

/*!
 * propsition to test if it is a space
 * @param target
 * @return
 */
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
 * @return an encoded decimal number.
 */
int e_deci(AST number)
{
  return atoi($T(number));
}

/*!
 * @return a parser combinator of Hexadecimal number
 */
Parsec p_hexa(char *name)
{
  // a hexadecimal number can be either positive or negative
  return make_or(
      name, make_and("pos", match(NULL, "0x"), take_while("hexa", isalnum)),
      make_and("neg", match(NULL, "-0x"), take_while("hexa", isalnum)));
}

/*!
 * @return encode hexa AST into int
 */
int e_hexa(AST hexa)
{
  AST pos = $G(hexa, "pos");
  if (pos)
  {
    char *val = $TG(pos, "hexa");
    return strtol(val, NULL, 16);
  }

  AST   neg = $G(hexa, "neg");
  char *val = $TG(neg, "hexa");
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
 * parser combination to match shift name, which can be (lsl | lsr | ror | asr)
 * @return
 */
Parsec p_shift_name(void)
{
  Parsec alts[4] = { match("lsl", "lsl "), match("lsr", "lsr "),
                     match("ror", "ror "), match("asr", "asr ") };
  return alt("shift name", alts, 4);
}
/*!
 * encode shift name AST into enum
 * @return
 */
shift_type e_shift_name(AST shift)
{
  if ($G(shift, "lsl"))
  {
    return LSL;
  }

  if ($G(shift, "lsr"))
  {
    return LSR;
  }

  if ($G(shift, "ror"))
  {
    return ROR;
  }

  if ($G(shift, "asr"))
  {
    return ASR;
  }
}

/*!
 * parser combination to match shifted register, which is Rm + shift name +
 * amount
 * @return
 */
Parsec p_shift_reg(void)
{
  Parsec seqs[3]
      = { p_reg_i("Rm"), p_shift_name(), p_hash_expr("shift amount") };
  return seq("shift reg", seqs, 3);
}
/*!
 * encode shifted register into reg or imm
 * @return
 */
reg_or_imm_t e_shift_reg(AST shift_reg)
{
  reg_or_imm_t result;
  result.shift_reg.Rm   = e_reg($G(shift_reg, "Rm"));
  result.shift_reg.type = e_shift_name($G(shift_reg, "shift name"));
  result.shift_reg.val  = e_eq_hash_expr($G(shift_reg, "shift amount"));
  return result;
}

/*!
 * a operand two is is either a (# expr | shifted register | a single register)
 * @return
 */
Parsec p_operand2(void)
{
  Parsec alts[3] = { p_hash_expr("number"), p_shift_reg(), p_reg_e("Rm") };
  return alt("operand2", alts, 3);
}

/*!
 * encode operand2 AST to reg_or_imm_t
 * @return
 */
reg_or_imm_t e_operand2(AST operand2, bool *is_imm, bool *is_positive)
{
  reg_or_imm_t result;
  AST          hash_ast = $G(operand2, "number");
  if (hash_ast)
  {
    int      amount;
    uint32_t imm;
    int      hash_expr_val = e_eq_hash_expr(hash_ast);
    if (hash_expr_val >= 0)
    {
      *is_positive = true;
    }
    else
    {
      *is_positive  = false;
      hash_expr_val = -hash_expr_val;
    }
    reverse_rotate(hash_expr_val, &amount, &imm);
    result.shift_reg.Rm   = 0;
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

  AST shift_reg = $G(operand2, "shift reg");
  return e_shift_reg(shift_reg);
}

/*!
 * encode pre index has offset AST into address_t
 * @return
 */
address_t e_pre_index_has_offset(AST pre_index, bool *is_imm, bool *is_up)
{
  address_t result;
  result.is_post     = false;
  result.is_eq_expr  = false;
  result.eq_expr_val = 0;
  result.operand2    = e_operand2($G(pre_index, "operand2"), is_imm, is_up);
  result.Rn          = e_reg($G(pre_index, "Rn"));
  return result;
}

/*!
 *
 * @return a parser combinator of pre index without offset
 */
Parsec p_pre_index_no_offset(void)
{
  Parsec seqs[3] = { match(NULL, "["), p_reg_i("Rn"), match(NULL, "]\n") };
  return seq("pre index no offset", seqs, 3);
}

/*!
 *
 * @return a parser combinator of pre index has offset
 */
Parsec p_pre_index_has_offset(void)
{
  Parsec seqs[4]
      = { match(NULL, "["), p_reg_i("Rn"), p_operand2(), match(NULL, "]\n") };
  return seq("pre index has offset", seqs, 4);
}

/*!
 * a pre index parser combinator either has a offset or without offset
 * @return
 */
Parsec p_pre_index(void)
{
  Parsec alts[2] = { p_pre_index_no_offset(), p_pre_index_has_offset() };
  return alt("pre index", alts, 2);
}

/*!
 * encode pre index no offset to address t
 * @return
 */
address_t e_pre_index_no_offset(AST pre_index, bool *is_imm, bool *is_up)
{
  address_t result;
  result.is_post                 = false;
  result.is_eq_expr              = false;
  result.eq_expr_val             = 0;
  result.operand2.shift_reg.Rm   = 0;
  result.operand2.shift_reg.type = 0;
  result.operand2.shift_reg.val  = 0;
  result.Rn                      = e_reg($G(pre_index, "Rn"));
  *is_imm                        = true;
  return result;
}

/*!
 * encode pre index to address t
 * @return
 */
address_t e_pre_index(AST pre_index, bool *is_imm, bool *is_up)
{
  *is_up        = true;
  AST no_offset = $G(pre_index, "pre index no offset");
  if (no_offset)
  {
    return e_pre_index_no_offset(no_offset, is_imm, is_up);
  }

  AST has_offset = $G(pre_index, "pre index has offset");
  if (has_offset)
  {
    return e_pre_index_has_offset(has_offset, is_imm, is_up);
  }
}

/*!
 * a post index is a [ Rn ], + operand2
 * @return a parser combinator of pos index.
 */
Parsec p_post_index(void)
{
  Parsec seqs[5]
      = { match(NULL, "["), p_reg_e("Rn"), match(NULL, "],"), p_operand2() };
  return seq("post index", seqs, 5);
}

/*!
 * @return an encode pos index.
 */
address_t e_post_index(AST post_index, bool *is_imm, bool *is_up)
{
  address_t result;
  result.is_eq_expr  = false;
  result.is_post     = true;
  result.eq_expr_val = 0;
  result.Rn          = e_reg($G(post_index, "Rn"));
  result.operand2    = e_operand2($G(post_index, "operand2"), is_imm, is_up);
  return result;
}

/*!
 * an address is either a eq expr a pre index or a post index
 * @return
 */
Parsec p_address(void)
{
  Parsec alts[3] = { p_eq_expr("eq expr"), p_pre_index(), p_post_index() };
  return alt("address", alts, 3);
}

/*!
 * @return an encoded '=' expression.
 */
address_t e_eq_expr(AST imm)
{
  address_t result;
  result.is_eq_expr  = true;
  result.is_post     = false;
  result.eq_expr_val = e_eq_hash_expr(imm);
  result.Rn          = 0xf;
  return result;
}

/*!
 * encode address AST to address t
 * @return an encoded address.
 */
address_t e_address(AST address, bool *is_imm, bool *is_up)
{
  AST imm = $G(address, "eq expr");
  if (imm)
  {
    return e_eq_expr(imm);
  }

  AST post_index = $G(address, "post index");
  if (post_index)
  {
    return e_post_index(post_index, is_imm, is_up);
  }

  AST pre_index = $G(address, "pre index");
  if (pre_index)
  {
    return e_pre_index(pre_index, is_imm, is_up);
  }
}
