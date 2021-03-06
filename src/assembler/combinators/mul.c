#include <stdlib.h>
#include <ctype.h>

#include "../../parsec/ast.h"
#include "../../parsec/char_stream.h"
#include "../../parsec/parsec.h"

#include "../../global_utils/types_and_macros.h"

#include "component.h"
#include "mul.h"

/*!
 * @return an encoded multiply instruction.
 */
mul_t e_mul_h(AST ast)
{
  mul_t result;
  result.Rd       = e_reg($G(ast, "Rd"));
  result.Rm       = e_reg($G(ast, "Rm"));
  result.Rs       = e_reg($G(ast, "Rs"));
  AST Rn_ast      = $G(ast, "Rn");
  result.Rn       = Rn_ast ? e_reg(Rn_ast) : 0;
  result.set_cond = false;
  return result;
}

/*!
 * @return an encoded multiply instruction.
 */
instruction_t e_mul(AST ast)
{
  instruction_t result;
  mul_t         mul;
  AST           mul_ast = $G(ast, "mul");
  if (mul_ast)
  {
    mul        = e_mul_h(mul_ast);
    mul.is_mla = false;
  }
  AST mla_ast = $G(ast, "mla");
  if (mla_ast)
  {
    mul        = e_mul_h(mla_ast);
    mul.is_mla = true;
  }

  result.cond     = AL;
  result.tag      = MUL;
  result.word.mul = mul;
  return result;
}

/*!
 * mul instruction is either mul or mla
 * @return
 */
Parsec p_mul()
{
  Parsec sequence1[4]
      = { match(NULL, "mul "), p_reg_i("Rd"), p_reg_i("Rm"), p_reg_e("Rs") };
  Parsec sequence2[5] = { match(NULL, "mla "), p_reg_i("Rd"), p_reg_i("Rm"),
                          p_reg_i("Rs"), p_reg_e("Rn") };
  return make_or("mul_mla", seq("mul", sequence1, 4),
                 seq("mla", sequence2, 5));
}
