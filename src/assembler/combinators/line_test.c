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

#include "line.h"

int main(void)
{
  char *        string;
  CharStream    char_stream;
  AST           ast;
  Parsec        parsec = p_line();
  instruction_t ins;
  SymbolTable   symbol_table = init_symbol_table();
  TokenStream   token_stream = init_token_stream();
  add_symbol_table("wait", 123, symbol_table);

  string      = "beq wait";
  char_stream = &string;
  ast         = perform_parse(char_stream, parsec, NULL);
  print_ast(ast, 0);
  ins = e_instruction($G(ast, "instruction"), 0, token_stream, symbol_table,
                      0);
  printf("\n");
  printf("ins.tag :>> %d\n", ins.tag);
  printf("ins.cond :>> %d\n", ins.cond);
  printf("ins.word.branch.offset :>> %d\n", ins.word.branch.offset);

  printf("HERE 1\n");
  string          = "ldr r1,=0x100";
  char_stream     = &string;
  ast             = perform_parse(char_stream, parsec, NULL);
  int end_address = 100;
  print_ast(ast, 0);
  ins = e_instruction($G(ast, "instruction"), 0, token_stream, symbol_table,
                      &end_address);
  printf("ins.cond :>> %d\n", ins.cond);
  printf("ins.tag :>> %d\n", ins.tag);
  trans_t trans = ins.word.trans;
  printf("trans.is_load :>> %d\n", trans.is_load);
  printf("trans.is_pre :>> %d\n", trans.is_pre);
  printf("trans.is_reg :>> %d\n", trans.is_reg);
  printf("trans.is_up :>> %d\n", trans.is_up);
  printf("trans.offset :>> %d\n", trans.offset.rot_imm.imm);
  printf("trans.Rd :>> %d\n", trans.Rd);
  printf("trans.Rn :>> %d\n", trans.Rn);
  printf("token_stream->tail->imm_val :>> %d\n", token_stream->tail->imm_val);
  printf("end_address :>> %d\n", end_address);

  string      = "mul r12,r2,r3\n";
  char_stream = &string;
  ast         = perform_parse(char_stream, parsec, NULL);
  print_ast(ast, 0);
  printf("\n");
  ins = e_instruction($G(ast, "instruction"), 0, token_stream, symbol_table,
                      &end_address);
  printf("ins.tag :>> %d\n", ins.tag);
  printf("ins.cond :>> %d\n", ins.cond);
  mul_t mul = ins.word.mul;
  printf("mul.Rn :>> %d\n", mul.Rn);
  printf("mul.Rd :>> %d\n", mul.Rd);
  printf("mul.Rm :>> %d\n", mul.Rm);
  printf("mul.is_mla :>> %d\n", mul.is_mla);
  printf("mul.Rs :>> %d\n", mul.Rs);

  string      = "add r1,r2,#0x14";
  char_stream = &string;
  ast         = perform_parse(char_stream, parsec, NULL);
  print_ast(ast, 0);
  printf("\n");
  ins = e_instruction($G(ast, "instruction"), 0, token_stream, symbol_table,
                      &end_address);
  printf("instruction.cond :>> %d\n", ins.cond);
  printf("instruction.tag :>> %d\n", ins.tag);
  proc_t proc = ins.word.proc;
  printf("proc.opcode :>> %d\n", proc.opcode);
  printf("proc.is_imm :>> %d\n", proc.is_imm);
  printf("proc.Rn :>> %d\n", proc.Rn);
  printf("proc.Rd :>> %d\n", proc.Rd);
  printf("proc.operand2.rot_imm :>> %d\n", proc.operand2.rot_imm.imm);

  string      = "add r1,r2,r3";
  char_stream = &string;
  ast         = perform_parse(char_stream, parsec, NULL);
  print_ast(ast, 0);
  printf("\n");
  ins = e_instruction($G(ast, "instruction"), 0, token_stream, symbol_table,
                      &end_address);
  printf("instruction.cond :>> %d\n", ins.cond);
  printf("instruction.tag :>> %d\n", ins.tag);
  proc = ins.word.proc;
  printf("proc.opcode :>> %d\n", proc.opcode);
  printf("proc.is_imm :>> %d\n", proc.is_imm);
  printf("proc.Rn :>> %d\n", proc.Rn);
  printf("proc.Rd :>> %d\n", proc.Rd);
  printf("proc.Rm :>> %d\n", proc.operand2.shift_reg.Rm);
  printf("proc.operand2.rot_imm :>> %d\n", proc.operand2.rot_imm.imm);

  string      = "and r2,r1,#0xAB";
  char_stream = &string;
  ast         = perform_parse(char_stream, parsec, NULL);
  print_ast(ast, 0);
  printf("\n");
  ins = e_instruction($G(ast, "instruction"), 0, token_stream, symbol_table,
                      &end_address);
  printf("instruction.cond :>> %d\n", ins.cond);
  printf("instruction.tag :>> %d\n", ins.tag);
  proc = ins.word.proc;
  printf("proc.opcode :>> %d\n", proc.opcode);
  printf("proc.is_imm :>> %d\n", proc.is_imm);
  printf("proc.Rn :>> %d\n", proc.Rn);
  printf("proc.Rd :>> %d\n", proc.Rd);
  printf("proc.rot_imm :>> %d\n", proc.operand2.rot_imm.imm);
  printf("proc.operand2.rot_imm.amount :>> %d\n",
         proc.operand2.rot_imm.amount);

  free_parsec(parsec);
  free_ast(ast);
}
