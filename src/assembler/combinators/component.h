#ifndef COMPONET
#define COMPONET

typedef struct address_t
{
  bool is_post;
  bool is_up;
  bool is_eq_expr;
  int eq_expr_val;
  reg_or_imm_t operand2;
  int Rn;
}address_t;

extern Parsec p_number(char *name);
extern Parsec p_hexa(char *name);
extern Parsec p_hash_expr(char * name);
extern Parsec p_eq_expr(char *name);
extern Parsec p_reg_i(char *name);
extern Parsec p_reg_e(char *name);
extern Parsec p_no_offset(void);
extern Parsec p_pre_index(void);
extern Parsec p_post_index(void);
extern Parsec p_address(void);
extern Parsec p_operand2(void);

extern int e_eq_hash_expr(AST hash_expr);
extern no_reg_t e_reg(AST reg);
extern reg_or_imm_t e_operand2(AST operand2, bool* is_imm, bool* is_positive);
extern address_t e_address(AST address, bool* is_imm, bool* is_up);
extern address_t e_pre_index(AST pre_index, bool* is_imm, bool* is_up);
extern address_t e_post_index(AST post_index, bool* is_imm, bool* is_up);

bool reverse_rotate(uint32_t target, int *rotation_amount, uint32_t *imm);

#endif //COMPONET
