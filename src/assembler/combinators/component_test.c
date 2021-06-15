#include <stdlib.h>
#include <ctype.h>

#include "../../parsec/ast.h"
#include "../../parsec/char_stream.h"
#include "../../parsec/parsec.h"

#include "component.h"

int main(void)
{
  char *     string;
  CharStream char_stream;
  AST        ast;
  Parsec     parsec;

  string      = "#10";
  char_stream = &string;

  ast = parse(char_stream, hash_expr("hash_expr"), NULL);

  print_ast(ast, 0);
  printf("\n");
  printf("e_hash_expr(ast) :>> %d\n,", e_eq_hash_expr(ast)); // DELETE_MARK

  string      = "#0x10";
  char_stream = &string;
  ast         = parse(char_stream, hash_expr("hash_expr"), NULL);
  print_ast(ast, 0);
  printf("\n");
  printf("e_hash_expr(ast) :>> %d\n,", e_eq_hash_expr(ast)); // DELETE_MARK

  string      = "=0x10";
  char_stream = &string;
  ast         = parse(char_stream, eq_expr("eq_expr"), NULL);
  print_ast(ast, 0);
  printf("\n");
  printf("e_hash_expr(ast) :>> %d\n,", e_eq_hash_expr(ast)); // DELETE_MARK

  string      = "r10";
  char_stream = &string;
  ast         = parse(char_stream, p_reg_e("Rm"), NULL);
  print_ast(ast, 0);
  printf("\n");
  printf("e_reg(ast) :>> %d\n,", e_reg(ast)); //DELETE_MARK


  string      = "r10";
  char_stream = &string;
  ast         = parse(char_stream, p_operand2(), NULL);
  print_ast(ast, 0);
  printf("\n");
  reg_or_imm reg_or_imm1 = e_operand2(ast);
  printf("reg_or_imm1.val :>> %d\n,", reg_or_imm1.val); //DELETE_MARK

  string      = "#14";
  char_stream = &string;
  ast         = parse(char_stream, p_operand2(), NULL);
  print_ast(ast, 0);
  printf("\n");
  reg_or_imm reg_or_imm2 = e_operand2(ast);
  printf("reg_or_imm2.val :>> %d\n,", reg_or_imm2.val); //DELETE_MARK

  string      = "[r10]";
  char_stream = &string;
  ast         = parse(char_stream, p_no_offset(), NULL);
  print_ast(ast, 0);
  printf("\n");
  address_t address = e_no_offset(ast);
  printf("address.reg_num :>> %d\n,", address.reg_num); //DELETE_MARK
  printf("address.offset :>> %d\n,", address.offset); //DELETE_MARK
  printf("address.is_post :>> %d\n,", address.is_post); //DELETE_MARK

  string      = "[r10]";
  char_stream = &string;
  ast         = parse(char_stream, p_pre_index(), NULL);
  print_ast(ast, 0);
  printf("\n");
  address = e_pre_index(ast);
  printf("address.reg_num :>> %d\n,", address.reg_num); //DELETE_MARK
  printf("address.offset :>> %d\n,", address.offset); //DELETE_MARK
  printf("address.is_post :>> %d\n,", address.is_post); //DELETE_MARK


  string      = "[r10, #10]";
  char_stream = &string;
  ast         = parse(char_stream, p_pre_index(), NULL);
  print_ast(ast, 0);
  printf("\n");
  address = e_pre_index(ast);
  printf("address.reg_num :>> %d\n,", address.reg_num); //DELETE_MARK
  printf("address.offset :>> %d\n,", address.offset); //DELETE_MARK
  printf("address.is_post :>> %d\n,", address.is_post); //DELETE_MARK

  string      = "[r10],#13";
  char_stream = &string;
  ast         = parse(char_stream, p_post_index(), NULL);
  print_ast(ast, 0);
  printf("\n");
     address = e_post_index(ast);
  printf("address.reg_num :>> %d\n,", address.reg_num); //DELETE_MARK
  printf("address.offset :>> %d\n,", address.offset); //DELETE_MARK
  printf("address.is_post :>> %d\n,", address.is_post); //DELETE_MARK

  string      = "[r10],#12";
  char_stream = &string;
  ast         = parse(char_stream, p_address(), NULL);
  print_ast(ast, 0);
  printf("\n");
     address = e_address(ast);
  printf("address.reg_num :>> %d\n,", address.reg_num); //DELETE_MARK
  printf("address.offset :>> %d\n,", address.offset); //DELETE_MARK
  printf("address.is_post :>> %d\n,", address.is_post); //DELETE_MARK
}
