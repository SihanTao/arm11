#ifndef BRAN_COMBINATOR
#define BRAN_COMBINATOR


extern Parsec p_bran_cond(void);
extern Parsec p_bran(void);

cond_type e_bran_cond(AST bran_cond);
instruction_t e_bran(AST bran, SymbolTable symbol_table, int current_address);

#endif //BRAN_COMBINATOR
