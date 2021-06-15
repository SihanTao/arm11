#include <stdlib.h>
#include <ctype.h>

#include "../../parsec/ast.h"
#include "../../char_stream.h"

#include "../../parsec.h"

#include "component.h"
#include "mul.h"

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
  return 0;
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
