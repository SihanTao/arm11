#include <stdlib.h>
#include <ctype.h>

#include "../../parsec/ast.h"
#include "../../parsec/char_stream.h"
#include "../../parsec/parsec.h"

#include "../../global_utils/types_and_macros.h"

#include "../data_structure/symbol_table.h"
#include "../data_structure/token_stream.h"

#include "component.h"
#include "bran.h"
#include "mul.h"
#include "trans.h"
#include "proc.h"

#include "line.h"

static Parsec        p_label(void);
static instruction_t e_andeq(AST anded);
static Parsec        p_andeq(void);
static Parsec        p_lsl(void);
static instruction_t e_lsl(AST lsl);
static Parsec        p_instruction(void);

/*!
 * convert instruction AST into intruction_t
 * @return
 */
instruction_t e_instruction(AST ins_ast, int cur_address,
                            TokenStream token_stream, SymbolTable symbol_table,
                            int *end_address)
{
  AST andeq = $G(ins_ast, "andeq");
  if (andeq)
  {
    return e_andeq(andeq);
  }

  AST lsl = $G(ins_ast, "lsl");
  if (lsl)
  {
    return e_lsl(lsl);
  }

  AST trans = $G(ins_ast, "trans");
  if (trans)
  {
    return e_trans(trans, cur_address, token_stream, end_address);
  }

  AST mul = $G(ins_ast, "mul_mla");
  if (mul)
  {
    return e_mul(mul);
  }

  AST bran = $G(ins_ast, "bran");
  if (bran)
  {
    return e_bran(bran, symbol_table, cur_address);
  }

  AST proc = $G(ins_ast, "proc");
  if (proc)
  {
    return e_proc(proc);
  }
}

/*!
 * a label is (take while alnum + ':')
 * @return
 */
Parsec p_label(void)
{
  Parsec seqs[3] = { take_while("label string", isalnum), match(NULL, ":"),
                     match(NULL, "\n") };
  return seq("label", seqs, 3);
}

/*!
 * encode label to the content of label
 * @return
 */
char *e_label(AST label)
{
  return $TG(label, "label string");
}

/*!
 * a line is either a instruction or a label
 * @return
 */
Parsec p_line(void)
{
  Parsec alts[2] = { p_label(), p_instruction() };
  return alt("line", alts, 2);
}

/*!
 * a instruction is either special instruction or normal instrution
 * @return
 */
Parsec p_instruction(void)
{
  Parsec alts[6]
      = { p_andeq(), p_lsl(), p_trans(), p_mul(), p_bran(), p_proc() };
  return alt("instruction", alts, 6);
}

/*!
 * @return an encoded zero instruction
 */
instruction_t e_andeq(AST anded)
{
  instruction_t result;
  result.tag = ZERO;
  return result;
}

/*!
 * @return a parser combinator of and equals.
 */
Parsec p_andeq(void)
{
  return match("andeq", "andeq");
}

/*!
 * @return a parser combinator of lsl.
 */
Parsec p_lsl(void)
{
  Parsec seqs[3]
      = { match(NULL, "lsl "), p_reg_i("Rm"), p_hash_expr("shift amount") };
  return seq("lsl", seqs, 3);
}

/*!
 * @return an encoded lsl.
 */
instruction_t e_lsl(AST lsl)
{
  int Rm           = e_reg($G(lsl, "Rm"));
  int shift_amount = e_eq_hash_expr($G(lsl, "shift amount"));

  reg_or_imm_t operand2;
  operand2.shift_reg.Rm   = Rm;
  operand2.shift_reg.type = LSL;
  operand2.shift_reg.val  = shift_amount;

  proc_t mov;
  mov.is_imm   = false;
  mov.opcode   = MOV;
  mov.operand2 = operand2;
  mov.Rd       = Rm;
  mov.Rn       = 0;
  mov.set_cond = false;

  instruction_t result;
  result.cond      = AL;
  result.tag       = PROC;
  result.word.proc = mov;
  return result;
}