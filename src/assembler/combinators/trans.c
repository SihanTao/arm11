#include <stdlib.h>
#include <ctype.h>

#include "../../parsec/ast.h"
#include "../../parsec/char_stream.h"

#include "../../parsec/parsec.h"

#include "../../global_utils/types_and_macros.h"

#include "../data_structure/token_stream.h"

#include "component.h"
#include "trans.h"

/*!
 * @return an encoded instruction in the case ldr as mov.
 */
instruction_t e_ldr_as_mov(AST trans)
{
  instruction_t result;
  proc_t        proc;
  bool is_imm;
  bool is_up;
  address_t     address = e_address($G(trans, "address"), &is_imm, &is_up);
  reg_or_imm_t  operand2;

  operand2.rot_imm.amount = 0;
  operand2.rot_imm.imm    = address.eq_expr_val;

  proc.is_imm   = true;
  proc.Rd       = e_reg($G(trans, "Rd"));
  proc.operand2 = operand2;
  proc.Rn       = 0;
  proc.opcode   = MOV;
  proc.set_cond = false;

  result.cond      = AL;
  result.tag       = PROC;
  result.word.proc = proc;
  return result;
}

/*!
 * @return an encoded instruction in the case ldr as immediate value.
 */
instruction_t e_ldr_imm(AST trans, int cur_address, TokenStream token_stream,
                        int *end_address)
{
  instruction_t result;
  bool is_imm;
  bool is_up;
  address_t     address = e_address($G(trans, "address"), &is_imm, &is_up);
  trans_t       trans_ins;
  reg_or_imm_t  offset;

  token_t *token = malloc(sizeof(token_t));
  token->imm_val = address.eq_expr_val;
  token->ast     = NULL;
  token->address = end_address;
  token->next = NULL;
  add_token_stream(token, token_stream);

  offset.rot_imm.amount = 0;
  offset.rot_imm.imm    = *end_address - cur_address - 8;
  trans_ins.offset      = offset;

  trans_ins.Rd      = e_reg($G(trans, "Rd"));
  trans_ins.is_reg  = false; // is imm
  trans_ins.is_load = true;
  trans_ins.is_pre  = true;
  trans_ins.Rn      = PC;
  trans_ins.is_up   = true;
  trans_ins.Rn      = 0xf;

  result.tag        = TRANS;
  result.cond       = AL;
  result.word.trans = trans_ins;

  *end_address += 4;
  return result;
}

/*!
 * @return an encoded sigle data transfer instruction.
 */
instruction_t e_trans_h(AST trans)
{
  instruction_t result;
  trans_t       trans_ins;
  bool is_imm;
  bool is_up;
  address_t     address = e_address($G(trans, "address"), &is_imm, &is_up);

  AST opcode = $G(trans, "opcode");
  trans_ins.is_load = !!$G(opcode, "ldr");
  trans_ins.Rd = e_reg($G(trans, "Rd"));

  trans_ins.is_reg = !is_imm;
  trans_ins.is_up = is_up;
  trans_ins.is_pre = !address.is_post;
  trans_ins.offset = address.operand2;
  trans_ins.Rn = address.Rn;
  trans_ins.is_up = is_up;

  result.cond       = AL;
  result.tag        = TRANS;
  result.word.trans = trans_ins;

  return result;
}

/*!
 * @return an encoded sigle data transfer instruction.
 */
instruction_t e_trans(AST trans, int cur_address, TokenStream token_stream,
                      int *end_address)
{
  instruction_t result;
  bool is_imm;
  bool is_up;
  address_t     address = e_address($G(trans, "address"), &is_imm, &is_up);

  if (address.is_eq_expr && address.eq_expr_val < 0xFF)
  {
    return e_ldr_as_mov(trans);
  }

  if (address.is_eq_expr) // greater than 0xFF
  {
    return e_ldr_imm(trans, cur_address, token_stream, end_address);
  }

  // else is not imm
  return e_trans_h(trans);
}

/*!
 * @return a parser combinator of single data transfer instruction.
 */
Parsec p_trans(void)
{
  Parsec seqs[3]
      = { make_or("opcode", match("ldr", "ldr "), match("str", "str ")),
          p_reg_i("Rd"), p_address() };
  return seq("trans", seqs, 3);
}