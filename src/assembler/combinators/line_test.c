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
  ins = e_instruction($G(ast, "instruction"), 0, token_stream,
                           symbol_table, 0);
  printf("\n");
  printf("ins.tag :>> %d\n", ins.tag);   // DELETE_MARK
  printf("ins.cond :>> %d\n", ins.cond); // DELETE_MARK
  printf("ins.word.branch.offset :>> %d\n",
         ins.word.branch.offset); // DELETE_MARK

  printf("HERE 1\n"); //DELETE_MARK
  string      = "ldr r1,=0x100";
  char_stream = &string;
  ast         = perform_parse(char_stream, parsec, NULL);
  int end_address = 100;
  print_ast(ast, 0);
  ins = e_instruction($G(ast, "instruction"), 0, token_stream, symbol_table, &end_address);
  printf("ins.cond :>> %d\n", ins.cond); // DELETE_MARK
  printf("ins.tag :>> %d\n", ins.tag);   // DELETE_MARK
  trans_t trans = ins.word.trans;
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

    string      = "mul r12,r2,r3\n";
  char_stream = &string;
  ast         = perform_parse(char_stream, parsec, NULL);
  print_ast(ast, 0);
  printf("\n");
  ins = e_instruction($G(ast, "instruction"), 0, token_stream, symbol_table, &end_address);
  printf("ins.tag :>> %d\n", ins.tag);   // DELETE_MARK
  printf("ins.cond :>> %d\n", ins.cond); // DELETE_MARK
  mul_t mul = ins.word.mul;
  printf("mul.Rn :>> %d\n", mul.Rn);         // DELETE_MARK
  printf("mul.Rd :>> %d\n", mul.Rd);         // DELETE_MARK
  printf("mul.Rm :>> %d\n", mul.Rm);         // DELETE_MARK
  printf("mul.is_mla :>> %d\n", mul.is_mla); // DELETE_MARK
  printf("mul.Rs :>> %d\n", mul.Rs);         // DELETE_MARK

  string      = "add r1,r2,#0x14";
    char_stream = &string;
  ast         = perform_parse(char_stream, parsec, NULL);
  print_ast(ast, 0);
  printf("\n");
  ins = e_instruction($G(ast, "instruction"), 0, token_stream, symbol_table, &end_address);
  printf("instruction.cond :>> %d\n", ins.cond); // DELETE_MARK
  printf("instruction.tag :>> %d\n", ins.tag);   // DELETE_MARK
  proc_t proc = ins.word.proc;
  printf("proc.opcode :>> %d\n", proc.opcode); // DELETE_MARK
  printf("proc.is_imm :>> %d\n", proc.is_imm);   // DELETE_MARK
  printf("proc.Rn :>> %d\n", proc.Rn);         // DELETE_MARK
  printf("proc.Rd :>> %d\n", proc.Rd);         // DELETE_MARK
  printf("proc.operand2.rot_imm :>> %d\n",
         proc.operand2.rot_imm.imm); // DELETE_MAR

    string      = "add r1,r2,r3";
    char_stream = &string;
  ast         = perform_parse(char_stream, parsec, NULL);
  print_ast(ast, 0);
  printf("\n");
  ins = e_instruction($G(ast, "instruction"), 0, token_stream, symbol_table, &end_address);
  printf("instruction.cond :>> %d\n", ins.cond); // DELETE_MARK
  printf("instruction.tag :>> %d\n", ins.tag);   // DELETE_MARK
  proc = ins.word.proc;
  printf("proc.opcode :>> %d\n", proc.opcode); // DELETE_MARK
  printf("proc.is_imm :>> %d\n", proc.is_imm);   // DELETE_MARK
  printf("proc.Rn :>> %d\n", proc.Rn);         // DELETE_MARK
  printf("proc.Rd :>> %d\n", proc.Rd);         // DELETE_MARK
  printf("proc.Rm :>> %d\n", proc.operand2.shift_reg.Rm); //DELETE_MARK
  printf("proc.operand2.rot_imm :>> %d\n",
         proc.operand2.rot_imm.imm); // DELETE_MAR

      string      = "and r2,r1,#0xAB";
    char_stream = &string;
  ast         = perform_parse(char_stream, parsec, NULL);
  print_ast(ast, 0);
  printf("\n");
  ins = e_instruction($G(ast, "instruction"), 0, token_stream, symbol_table, &end_address);
  printf("instruction.cond :>> %d\n", ins.cond); // DELETE_MARK
  printf("instruction.tag :>> %d\n", ins.tag);   // DELETE_MARK
  proc = ins.word.proc;
  printf("proc.opcode :>> %d\n", proc.opcode); // DELETE_MARK
  printf("proc.is_imm :>> %d\n", proc.is_imm);   // DELETE_MARK
  printf("proc.Rn :>> %d\n", proc.Rn);         // DELETE_MARK
  printf("proc.Rd :>> %d\n", proc.Rd);         // DELETE_MARK
  printf("proc.rot_imm :>> %d\n", proc.operand2.rot_imm.imm); //DELETE_MARK
  printf("proc.operand2.rot_imm.amount :>> %d\n",
         proc.operand2.rot_imm.amount); // DELETE_MAR

  free_parsec(parsec);
  free_ast(ast);
}
