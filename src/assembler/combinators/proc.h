#ifndef PROC_COMBINATOR
#define PROC_COMBINATOR

extern Parsec p_arith(void);
extern Parsec p_mov(void);
extern Parsec p_cmp_tst(void);
extern Parsec p_proc(void);
instruction_t e_proc(AST arith);

#endif //PROC_COMBINATOR
