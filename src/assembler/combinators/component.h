#ifndef COMPONET
#define COMPONET

typedef struct address_t
{
  bool is_post;
  int offset;
  int reg_num;
}address_t;

extern Parsec p_number(char *name);
extern Parsec p_hexa(char *name);
extern Parsec hash_expr(char * name);
extern Parsec eq_expr(char *name);
extern Parsec p_reg_i(char *name);
extern Parsec p_reg_e(char *name);
extern Parsec p_no_offset(void);
extern Parsec p_pre_index(void);
extern Parsec p_post_index(void);
extern Parsec p_address(void);
extern Parsec p_operand2(void);

extern int e_eq_hash_expr(AST hash_expr);
int e_reg(AST reg);
reg_or_imm_t e_operand2(AST operand2, bool* is_imm);
address_t e_address(AST address);
address_t e_no_offset(AST no_offset);
address_t e_has_offset(AST has_offset);
address_t e_pre_index(AST pre_index);
address_t e_post_index(AST post_index);
#endif //COMPONET


