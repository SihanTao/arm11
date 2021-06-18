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
  ast         = perform_parse(char_stream, p_hash_expr("hash_expr"), NULL);
  print_ast(ast, 0);
  printf("\n");
  printf("e_hash_expr(ast) :>> %d\n", e_eq_hash_expr(ast));

  string      = "#-0x10";
  char_stream = &string;
  ast         = perform_parse(char_stream, p_hash_expr("hash_expr"), NULL);
  print_ast(ast, 0);
  printf("e_hash_expr(ast) :>> %d\n", e_eq_hash_expr(ast));

  string      = "=0x10";
  char_stream = &string;
  ast         = perform_parse(char_stream, p_eq_expr("eq_expr"), NULL);
  print_ast(ast, 0);
  printf("\n");
  printf("e_hash_expr(ast) :>> %d\n", e_eq_hash_expr(ast));

  string      = "r10";
  char_stream = &string;
  ast         = perform_parse(char_stream, p_reg_e("Rm"), NULL);
  print_ast(ast, 0);
  printf("\n");
  printf("e_reg(ast) :>> %d\n", e_reg(ast));

  string      = "r10";
  char_stream = &string;
  ast         = perform_parse(char_stream, p_operand2(), NULL);
  print_ast(ast, 0);
  printf("\n");
  bool         imm;
  bool         is_up;
  reg_or_imm_t reg_or_imm1 = e_operand2(ast, &imm, &is_up);
  printf("imm :>> %d\n", imm);
  printf("reg_or_imm1.val :>> %d\n", reg_or_imm1.shift_reg.Rm);

  string      = "#14";
  char_stream = &string;
  ast         = perform_parse(char_stream, p_operand2(), NULL);
  print_ast(ast, 0);
  printf("\n");
  bool imm;
  bool is_up;
  reg_or_imm1 = e_operand2(ast, &imm, &is_up);
  printf("imm :>> %d\n", imm);
  printf("reg_or_imm1.val :>> %d\n", reg_or_imm1.rot_imm.imm);

  string      = "[r10]";
  char_stream = &string;
  ast         = perform_parse(char_stream, p_address(), NULL);
  print_ast(ast, 0);
  printf("\n");
  bool      imm;
  bool      is_up;
  address_t address = e_address(ast, &imm, &is_up);
  printf("address.Rn :>> %d\n", address.Rn);
  printf("address.offset :>> %d\n", address.eq_expr_val);
  printf("address.is_post :>> %d\n", address.is_post);

  string      = "[r10]";
  char_stream = &string;
  ast         = perform_parse(char_stream, p_pre_index(), NULL);
  print_ast(ast, 0);
  printf("\n");
  bool imm;
  bool is_up;
  address = e_address(ast, &imm, &is_up);
  printf("address.Rn :>> %d\n", address.Rn);
  printf("address.offset :>> %d\n", address.is_eq_expr);
  printf("address.is_post :>> %d\n", address.is_post);

  string      = "[r10,#10]";
  char_stream = &string;
  ast         = perform_parse(char_stream, p_pre_index(), NULL);
  print_ast(ast, 0);
  printf("\n");
  bool imm;
  bool is_up;
  address = e_address(ast, &imm, &is_up);
  printf("address.Rn :>> %d\n", address.Rn);
  printf("address.offset :>> %d\n", address.is_eq_expr);
  printf("address.is_post :>> %d\n", address.is_post);

  string      = "[r10],#13";
  char_stream = &string;
  ast         = perform_parse(char_stream, p_post_index(), NULL);
  print_ast(ast, 0);
  printf("\n");
  bool imm;
  bool is_up;
  address = e_address(ast, &imm, &is_up);
  printf("address.Rn :>> %d\n", address.Rn);
  printf("address.offset :>> %d\n", address.is_eq_expr);
  printf("address.is_post :>> %d\n", address.is_post);

  string      = "[r10],#12";
  char_stream = &string;
  ast         = perform_parse(char_stream, p_address(), NULL);
  print_ast(ast, 0);
  printf("\n");
  bool imm;
  bool is_up;
  address = e_address(ast, &imm, &is_up);
  printf("address.Rn :>> %d\n", address.Rn);
  printf("address.offset :>> %d\n", address.is_eq_expr);
  printf("address.is_post :>> %d\n", address.is_post);

  string      = "=0x100";
  char_stream = &string;
  ast         = perform_parse(char_stream, p_address(), NULL);
  print_ast(ast, 0);
  printf("\n");
  bool imm;
  bool is_up;
  address = e_address(ast, &imm, &is_up);
  printf("address.Rn :>> %d\n", address.Rn);
  printf("address.offset :>> %d\n", address.is_eq_expr);
  printf("address.is_post :>> %d\n", address.is_post);
}
