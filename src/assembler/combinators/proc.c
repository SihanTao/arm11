#include <stdlib.h>
#include <ctype.h>

#include "../../parsec/ast.h"
#include "../../parsec/char_stream.h"

#include "../../parsec/parsec.h"

#include "../../global_utils/types_and_macros.h"

#include "component.h"
#include "proc.h"

Parsec p_arith(void)
{
  Parsec alts[6]
      = { match("and", "and "), match("eor", "eor "), match("sub", "sub "),
          match("rsb", "rsb "), match("add", "add "), match("orr", "orr ") };
  Parsec sequence[5]
      = { alt("opcode", alts, 6), p_reg_i("Rd"), p_reg_i("Rn"), p_operand2(), match(NULL, "\n")};
  return seq("sub proc", sequence, 5);
}

pd_opcode_type e_opcode(AST opcode)
{
  if ($G(opcode, "and"))
  {
    return AND;
  }

  if ($G(opcode, "eor"))
  {
    return EOR;
  }

  if ($G(opcode, "sub"))
  {
    return SUB;
  }

  if ($G(opcode, "rsb"))
  {
    return RSB;
  }

  if ($G(opcode, "add"))
  {
    return ADD;
  }

  if ($G(opcode, "orr"))
  {
    return ORR;
  }

  if ($G(opcode, "tst"))
  {
    return TST;
  }

  if ($G(opcode, "teq"))
  {
    return TEQ;
  }

  if ($G(opcode, "cmp"))
  {
    return CMP;
  }

  return MOV;
}

Parsec p_mov(void)
{
  Parsec seqs[4] = { match("opcode", "mov "),p_reg_i("Rd"), p_operand2(), match(NULL, "\n")};
  return seq("sub proc", seqs, 4);
}

Parsec p_cmp_tst(void)
{
  Parsec alts[6]
      = { match("tst", "tst "), match("teq", "teq "), match("cmp", "cmp ") };
  Parsec sequence[4] = { alt("opcode", alts, 6), p_reg_i("Rn"), p_operand2(), match(NULL, "\n") };
  return seq("sub proc", sequence, 4);
}

Parsec p_proc(void)
{
  Parsec alts[3] = { p_mov(), p_arith(), p_cmp_tst() };
  return alt("proc", alts, 3);
}

instruction_t e_proc(AST proc_ast)
{
  instruction_t result;
  proc_t        proc;
  AST           sub_proc = $G(proc_ast, "sub proc");
  AST           Rd       = $G(sub_proc, "Rd");
  AST           Rn       = $G(sub_proc, "Rn");
  proc.Rd                = Rd ? e_reg(Rd) : 0;
  proc.Rn                = Rn ? e_reg(Rn) : 0;

  bool *is_imm  = &proc.is_imm;
  proc.operand2 = e_operand2($G(sub_proc, "operand2"), is_imm);
  proc.opcode   = e_opcode($G(sub_proc, "opcode"));
  proc.set_cond = false;
  if (proc.opcode == CMP || proc.opcode == TST || proc.opcode == TEQ)
  {
    proc.set_cond = true;
  }


  result.cond      = AL;
  result.tag       = PROC;
  result.word.proc = proc;
  return result;
}
