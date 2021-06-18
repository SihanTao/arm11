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
  address_t     address = e_address($G(trans, "address"));
  reg_or_imm_t  operand2;

  operand2.rot_imm.amount = 0;
  operand2.rot_imm.imm    = address.offset_or_eq_expr;

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
  address_t     address = e_address($G(trans, "address"));
  trans_t       trans_ins;
  reg_or_imm_t  offset;

  token_t *token = malloc(sizeof(token_t));
  token->imm_val = address.offset_or_eq_expr;
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
  address_t     address = e_address($G(trans, "address"));
  trans_t       trans_ins;
  reg_or_imm_t  offset;
  bool          is_up = address.offset_or_eq_expr >= 0;
  int           offset_unsigned
      = is_up ? address.offset_or_eq_expr : -address.offset_or_eq_expr;
  int amount;
  int imm;

  if (address.offset_or_eq_expr != 0)
  {
    reverse_rotate(offset_unsigned, &amount, &imm);
    offset.rot_imm.amount = amount;
    offset.rot_imm.imm    = imm;
  }
  else
  {
    offset.shift_reg.Rm   = 0;
    offset.shift_reg.type = LSL;
    offset.shift_reg.val  = 0;
  }

  AST opcode = $G(trans, "opcode");

  // DO NOT REMOVE THE !! OPERATOR
  trans_ins.is_load = !!$G(opcode, "ldr");
  trans_ins.is_pre  = !address.is_post;
  trans_ins.is_reg  = false;
  trans_ins.is_up   = is_up;
  trans_ins.offset  = offset;
  trans_ins.Rd      = e_reg($G(trans, "Rd"));
  trans_ins.Rn      = address.Rn;

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
  address_t     address = e_address($G(trans, "address"));

  if (address.is_eq_expr && address.offset_or_eq_expr < 0xFF)
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