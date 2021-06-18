#include <stdlib.h>
#include <ctype.h>

#include "../../parsec/ast.h"
#include "../../parsec/char_stream.h"
#include "../../parsec/parsec.h"

#include "../../global_utils/types_and_macros.h"

#include "component.h"

int main(void)
{
  char *     string;
  CharStream char_stream;
  AST        ast;
  Parsec     parsec;

  string      = "#10";
  char_stream = &string;

  ast = perform_parse(char_stream, p_hash_expr("hash_expr"), NULL);

  print_ast(ast, 0);
  printf("\n");
  printf("e_hash_expr(ast) :>> %d\n", e_eq_hash_expr(ast)); // DELETE_MARK

  string      = "#-0x10";
  char_stream = &string;
  ast         = perform_parse(char_stream,
                 p_hash_expr("hash_expr"), NULL);
  print_ast(ast, 0);
  printf("e_hash_expr(ast) :>> %d\n",
           e_eq_hash_expr(ast));

  string      = "=0x10";
  char_stream = &string;
  ast         = perform_parse(char_stream, p_eq_expr("eq_expr"), NULL);
  print_ast(ast, 0);
  printf("\n");
  printf("e_hash_expr(ast) :>> %d\n", e_eq_hash_expr(ast)); // DELETE_MARK

  string      = "r10";
  char_stream = &string;
  ast         = perform_parse(char_stream, p_reg_e("Rm"), NULL);
  print_ast(ast, 0);
  printf("\n");
  printf("e_reg(ast) :>> %d\n", e_reg(ast)); // DELETE_MARK

  string      = "r10";
  char_stream = &string;
  ast         = perform_parse(char_stream, p_operand2(), NULL);
  print_ast(ast, 0);
  printf("\n");
  bool         imm;
  reg_or_imm_t reg_or_imm1 = e_operand2(ast, &imm);
  printf("imm :>> %d\n", imm);                                  // DELETE_MARK
  printf("reg_or_imm1.val :>> %d\n", reg_or_imm1.shift_reg.Rm); // DELETE_MARK

  string      = "#14";
  char_stream = &string;
  ast         = perform_parse(char_stream, p_operand2(), NULL);
  print_ast(ast, 0);
  printf("\n");
  reg_or_imm1 = e_operand2(ast, &imm);
  printf("imm :>> %d\n", imm);                                 // DELETE_MARK
  printf("reg_or_imm1.val :>> %d\n", reg_or_imm1.rot_imm.imm); // DELETE_MARK

  string      = "[r10]";
  char_stream = &string;
  ast         = perform_parse(char_stream, p_no_offset(), NULL);
  print_ast(ast, 0);
  printf("\n");
  address_t address = e_no_offset(ast);
  printf("address.Rn :>> %d\n", address.Rn);      // DELETE_MARK
  printf("address.offset :>> %d\n", address.offset_or_eq_expr); // DELETE_MARK
  printf("address.is_post :>> %d\n", address.is_post);      // DELETE_MARK

  string      = "[r10]";
  char_stream = &string;
  ast         = perform_parse(char_stream, p_pre_index(), NULL);
  print_ast(ast, 0);
  printf("\n");
  address = e_pre_index(ast);
  printf("address.Rn :>> %d\n", address.Rn);      // DELETE_MARK
  printf("address.offset :>> %d\n", address.offset_or_eq_expr); // DELETE_MARK
  printf("address.is_post :>> %d\n", address.is_post);      // DELETE_MARK

  string      = "[r10,#10]";
  char_stream = &string;
  ast         = perform_parse(char_stream, p_pre_index(), NULL);
  print_ast(ast, 0);
  printf("\n");
  address = e_pre_index(ast);
  printf("address.Rn :>> %d\n", address.Rn);      // DELETE_MARK
  printf("address.offset :>> %d\n", address.offset_or_eq_expr); // DELETE_MARK
  printf("address.is_post :>> %d\n", address.is_post);      // DELETE_MARK

  string      = "[r10],#13";
  char_stream = &string;
  ast         = perform_parse(char_stream, p_post_index(), NULL);
  print_ast(ast, 0);
  printf("\n");
  address = e_post_index(ast);
  printf("address.Rn :>> %d\n", address.Rn);      // DELETE_MARK
  printf("address.offset :>> %d\n", address.offset_or_eq_expr); // DELETE_MARK
  printf("address.is_post :>> %d\n", address.is_post);      // DELETE_MARK

  string      = "[r10],#12";
  char_stream = &string;
  ast         = perform_parse(char_stream, p_address(), NULL);
  print_ast(ast, 0);
  printf("\n");
  address = e_address(ast);
  printf("address.Rn :>> %d\n", address.Rn);      // DELETE_MARK
  printf("address.offset :>> %d\n", address.offset_or_eq_expr); // DELETE_MARK
  printf("address.is_post :>> %d\n", address.is_post);      // DELETE_MARK

  string      = "=0x100";
  char_stream = &string;
  ast         = perform_parse(char_stream, p_address(), NULL);
  print_ast(ast, 0);
  printf("\n");
  address = e_address(ast);
  printf("address.is_imm :>> %d\n", address.is_eq_expr);        // DELETE_MARK
  printf("address.Rn :>> %d\n", address.Rn);      // DELETE_MARK
  printf("address.offset :>> %d\n", address.offset_or_eq_expr); // DELETE_MARK
  printf("address.is_post :>> %d\n", address.is_post);      // DELETE_MARK
}


