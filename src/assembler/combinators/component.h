#ifndef COMPONET
#define COMPONET

extern Parsec p_number(char *name);
extern Parsec p_hexa(char *name);
extern Parsec hash_expr(void);
extern Parsec eq_expr(void);
extern Parsec p_reg_i(char *name);
extern Parsec p_reg_e(char *name);
extern Parsec no_offset(void);
extern Parsec pre_index(void);
extern Parsec post_index(void);
extern Parsec address(void);
extern Parsec operand2(void);

#endif //COMPONET


