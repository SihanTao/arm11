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
  printf("ins.cond :>> %d\n", ins.cond); // DELETE_MARK
  printf("ins.tag :>> %d\n", ins.tag);   // DELETE_MARK
  trans = ins.word.trans;
  printf("trans.is_load :>> %d\n", trans.is_load); // DELETE_MARK
  printf("trans.is_pre :>> %d\n", trans.is_pre);   // DELETE_MARK
  printf("trans.is_reg :>> %d\n", trans.is_reg);   // DELETE_MARK
  printf("trans.is_up :>> %d\n", trans.is_up);     // DELETE_MARK
  printf("trans.offset :>> %d\n", trans.offset.rot_imm.imm); // DELETE_MARK
  printf("trans.Rd :>> %d\n", trans.Rd);                     // DELETE_MARK
  printf("trans.Rn :>> %d\n", trans.Rn);                     // DELETE_MARK

  string      = "str r1,[r2]";
  char_stream = &string;
  ast         = perform_parse(char_stream, p_trans(), NULL);
  print_ast(ast, 0);
  ins = e_trans(ast, 0, token_stream, 0);
  printf("ins.cond :>> %d\n", ins.cond); // DELETE_MARK
  printf("ins.tag :>> %d\n", ins.tag);   // DELETE_MARK
  trans = ins.word.trans;
  printf("trans.is_load :>> %d\n", trans.is_load); // DELETE_MARK
  printf("trans.is_pre :>> %d\n", trans.is_pre);   // DELETE_MARK
  printf("trans.is_reg :>> %d\n", trans.is_reg);   // DELETE_MARK
  printf("trans.is_up :>> %d\n", trans.is_up);     // DELETE_MARK
  printf("trans.offset :>> %d\n", trans.offset.rot_imm.imm); // DELETE_MARK
  printf("trans.Rd :>> %d\n", trans.Rd);                     // DELETE_MARK
  printf("trans.Rn :>> %d\n", trans.Rn);                     // DELETE_MARK

  string      = "ldr r1,[r2,#100]";
  char_stream = &string;
  ast         = perform_parse(char_stream, p_trans(), NULL);
  print_ast(ast, 0);
  ins = e_trans(ast, 0, token_stream, 0);
  printf("ins.cond :>> %d\n", ins.cond); // DELETE_MARK
  printf("ins.tag :>> %d\n", ins.tag);   // DELETE_MARK
  trans = ins.word.trans;
  printf("trans.is_load :>> %d\n", trans.is_load);           // DELETE_MARK
  printf("trans.is_pre :>> %d\n", trans.is_pre);             // DELETE_MARK
  printf("trans.is_reg :>> %d\n", trans.is_reg);             // DELETE_MARK
  printf("trans.is_up :>> %d\n", trans.is_up);               // DELETE_MARK
  printf("trans.offset :>> %d\n", trans.offset.rot_imm.imm); // DELETE_MARK
  printf("trans.Rd :>> %d\n", trans.Rd);                     // DELETE_MARK
  printf("trans.Rn :>> %d\n", trans.Rn);                     // DELETE_MARK

  string      = "ldr r1,=0x100";
  char_stream = &string;
  ast         = perform_parse(char_stream, p_trans(), NULL);
  int end_address = 100;
  print_ast(ast, 0);
  ins = e_trans(ast, 0, token_stream, &end_address);
  printf("ins.cond :>> %d\n", ins.cond); // DELETE_MARK
  printf("ins.tag :>> %d\n", ins.tag);   // DELETE_MARK
  trans = ins.word.trans;
  printf("trans.is_load :>> %d\n", trans.is_load);           // DELETE_MARK
  printf("trans.is_pre :>> %d\n", trans.is_pre);             // DELETE_MARK
  printf("trans.is_reg :>> %d\n", trans.is_reg);             // DELETE_MARK
  printf("trans.is_up :>> %d\n", trans.is_up);               // DELETE_MARK
  printf("trans.offset :>> %d\n", trans.offset.rot_imm.imm); // DELETE_MARK
  printf("trans.Rd :>> %d\n", trans.Rd);                     // DELETE_MARK
  printf("trans.Rn :>> %d\n", trans.Rn);                     // DELETE_MARK
  printf("token_stream->tail->imm_val :>> %d\n",
         token_stream->tail->imm_val); // DELETE_MARK
  printf("end_address :>> %d\n", end_address); //DELETE_MARK

}