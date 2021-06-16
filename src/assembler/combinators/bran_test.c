#include <stdlib.h>
#include <ctype.h>

#include "../../parsec/ast.h"
#include "../../parsec/char_stream.h"

#include "../../parsec/parsec.h"

#include "../../global_utils/types_and_macros.h"

#include "../data_structure/symbol_table.h"

#include "bran.h"

int main(void)
{
  char *     string;
  CharStream char_stream;
  AST        ast;
  Parsec     parsec;
  instruction_t bran_ins;
  SymbolTable symbol_table = init_symbol_table();
  add_symbol_table("wait", 123, symbol_table);

  string      = "b wait";
  char_stream = &string;
  ast         = parse(char_stream, p_bran(), NULL);
  print_ast(ast, 0);
  printf("\n");
  bran_ins = e_bran(ast, symbol_table);
  printf("bran_ins.tag :>> %d\n,", bran_ins.tag); //DELETE_MARK
  printf("bran_ins.cond :>> %d\n,", bran_ins.cond); //DELETE_MARK
  printf("bran_ins.word.branch.offset :>> %d\n,", bran_ins.word.branch.offset); //DELETE_MARK

  string      = "bal wait";
  char_stream = &string;
  ast         = parse(char_stream, p_bran(), NULL);
  print_ast(ast, 0);
  printf("\n");
    bran_ins = e_bran(ast, symbol_table);

    printf("bran_ins.tag :>> %d\n,", bran_ins.tag); //DELETE_MARK
  printf("bran_ins.cond :>> %d\n,", bran_ins.cond); //DELETE_MARK
  printf("bran_ins.word.branch.offset :>> %d\n,", bran_ins.word.branch.offset); //DELETE_MARK

  string      = "beq wait";
  char_stream = &string;
  ast         = parse(char_stream, p_bran(), NULL);
  print_ast(ast, 0);
    bran_ins = e_bran(ast, symbol_table);

  printf("\n");
    printf("bran_ins.tag :>> %d\n,", bran_ins.tag); //DELETE_MARK
  printf("bran_ins.cond :>> %d\n,", bran_ins.cond); //DELETE_MARK
  printf("bran_ins.word.branch.offset :>> %d\n,", bran_ins.word.branch.offset); //DELETE_MARK
}
