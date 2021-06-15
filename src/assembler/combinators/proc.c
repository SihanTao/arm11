#include <stdlib.h>
#include <ctype.h>

#include "../../parsec/ast.h"
#include "../../char_stream.h"

#include "../../parsec.h"

#include "component.h"
#include "proc.h"

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
