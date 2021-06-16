#include <stdlib.h>
#include <ctype.h>

#include "../../parsec/ast.h"
#include "../../parsec/char_stream.h"

#include "../../parsec/parsec.h"

#include "../../global_utils/types_and_macros.h"

#include "component.h"
#include "proc.h"

int main(void)
{
  char *     string;
  CharStream char_stream;
  AST        ast;
  Parsec     parsec;

  string      = "add r1,r2,#3";
  char_stream = &string;
  ast         = parse(char_stream, p_arith(), NULL);
  print_ast(ast, 0);
  printf("\n");
  string      = "add r1,r2,#0x14";

  char_stream = &string;
  ast         = parse(char_stream, p_proc(), NULL);
  print_ast(ast, 0);
  printf("\n");
  instruction_t instruction = e_proc(ast);
  printf("instruction.cond :>> %d\n", instruction.cond); // DELETE_MARK
  printf("instruction.tag :>> %d\n", instruction.tag);   // DELETE_MARK
  proc_t proc = instruction.word.proc;
  printf("proc.opcode :>> %d\n", proc.opcode); // DELETE_MARK
  printf("proc.is_imm :>> %d\n", proc.is_imm);   // DELETE_MARK
  printf("proc.Rn :>> %d\n", proc.Rn);         // DELETE_MARK
  printf("proc.Rd :>> %d\n", proc.Rd);         // DELETE_MARK
  printf("proc.operand2.rot_imm :>> %d\n",
         proc.operand2.rot_imm.imm); // DELETE_MARK

  string      = "mov r1,#0x14";
  char_stream = &string;
  ast         = parse(char_stream, p_proc(), NULL);
  print_ast(ast, 0);
  printf("\n");
  instruction = e_proc(ast);
  printf("instruction.cond :>> %d\n", instruction.cond); // DELETE_MARK
  printf("instruction.tag :>> %d\n", instruction.tag);   // DELETE_MARK
  proc = instruction.word.proc;
  printf("proc.opcode :>> %d\n", proc.opcode); // DELETE_MARK
  printf("proc.is_imm :>> %d\n", proc.is_imm);   // DELETE_MARK
  printf("proc.Rn :>> %d\n", proc.Rn);         // DELETE_MARK
  printf("proc.Rd :>> %d\n", proc.Rd);         // DELETE_MARK
  printf("proc.operand2.rot_imm :>> %d\n",
         proc.operand2.rot_imm.imm); // DELETE_MARK
}
