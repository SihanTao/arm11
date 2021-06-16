#ifndef TRANS_COMBINATORS
#define TRANS_COMBINATORS

instruction_t e_ldr_as_mov(AST trans);
instruction_t e_ldr_imm(AST trans, int cur_address, TokenStream token_stream,
                        int* end_address);
instruction_t e_trans_h(AST trans);
instruction_t e_trans(AST trans, int cur_address, TokenStream token_stream,
                      int* end_address);
Parsec p_trans(void);

#endif //TRANS_COMBINATORS
