#include <stdlib.h>
#include <ctype.h>

#include "../../parsec/ast.h"
#include "../../parsec/char_stream.h"

#include "../../parsec/parsec.h"

#include "../../global_utils/types_and_macros.h"

#include "../data_structure/token_stream.h"

#include "component.h"
#include "trans.h"

int main(void)
{
  char *        string;
  CharStream    char_stream;
  AST           ast;
  Parsec        parsec;
  instruction_t ins;
  trans_t       trans;
  TokenStream   token_stream = init_token_stream();

  string      = "ldr r1,[r2]";
  char_stream = &string;
  ast         = perform_parse(char_stream, p_trans(), NULL);
  print_ast(ast, 0);
  ins = e_trans(ast, 0, token_stream, 0);
  printf("ins.cond :>> %d\n", ins.cond);
  printf("ins.tag :>> %d\n", ins.tag);
  trans = ins.word.trans;
  printf("trans.is_load :>> %d\n", trans.is_load);
  printf("trans.is_pre :>> %d\n", trans.is_pre);
  printf("trans.is_reg :>> %d\n", trans.is_reg);
  printf("trans.is_up :>> %d\n", trans.is_up);
  printf("trans.offset :>> %d\n", trans.offset.rot_imm.imm);
  printf("trans.Rd :>> %d\n", trans.Rd);
  printf("trans.Rn :>> %d\n", trans.Rn);

  string      = "str r1,[r2]";
  char_stream = &string;
  ast         = perform_parse(char_stream, p_trans(), NULL);
  print_ast(ast, 0);
  ins = e_trans(ast, 0, token_stream, 0);
  printf("ins.cond :>> %d\n", ins.cond);
  printf("ins.tag :>> %d\n", ins.tag);
  trans = ins.word.trans;
  printf("trans.is_load :>> %d\n", trans.is_load);
  printf("trans.is_pre :>> %d\n", trans.is_pre);
  printf("trans.is_reg :>> %d\n", trans.is_reg);
  printf("trans.is_up :>> %d\n", trans.is_up);
  printf("trans.offset :>> %d\n", trans.offset.rot_imm.imm);
  printf("trans.Rd :>> %d\n", trans.Rd);
  printf("trans.Rn :>> %d\n", trans.Rn);

  string      = "ldr r1,[r2,#100]";
  char_stream = &string;
  ast         = perform_parse(char_stream, p_trans(), NULL);
  print_ast(ast, 0);
  ins = e_trans(ast, 0, token_stream, 0);
  printf("ins.cond :>> %d\n", ins.cond);
  printf("ins.tag :>> %d\n", ins.tag);
  trans = ins.word.trans;
  printf("trans.is_load :>> %d\n", trans.is_load);
  printf("trans.is_pre :>> %d\n", trans.is_pre);
  printf("trans.is_reg :>> %d\n", trans.is_reg);
  printf("trans.is_up :>> %d\n", trans.is_up);
  printf("trans.offset :>> %d\n", trans.offset.rot_imm.imm);
  printf("trans.Rd :>> %d\n", trans.Rd);
  printf("trans.Rn :>> %d\n", trans.Rn);

  string          = "ldr r1,=0x100";
  char_stream     = &string;
  ast             = perform_parse(char_stream, p_trans(), NULL);
  int end_address = 100;
  print_ast(ast, 0);
  ins = e_trans(ast, 0, token_stream, &end_address);
  printf("ins.cond :>> %d\n", ins.cond);
  printf("ins.tag :>> %d\n", ins.tag);
  trans = ins.word.trans;
  printf("trans.is_load :>> %d\n", trans.is_load);
  printf("trans.is_pre :>> %d\n", trans.is_pre);
  printf("trans.is_reg :>> %d\n", trans.is_reg);
  printf("trans.is_up :>> %d\n", trans.is_up);
  printf("trans.offset :>> %d\n", trans.offset.rot_imm.imm);
  printf("trans.Rd :>> %d\n", trans.Rd);
  printf("trans.Rn :>> %d\n", trans.Rn);
  printf("token_stream->tail->imm_val :>> %d\n", token_stream->tail->imm_val);
  printf("end_address :>> %d\n", end_address);
}