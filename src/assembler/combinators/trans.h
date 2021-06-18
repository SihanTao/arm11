#ifndef TRANS_COMBINATORS
#define TRANS_COMBINATORS


extern instruction_t e_trans(AST trans, int cur_address, TokenStream token_stream,
                      int *end_address);
extern Parsec        p_trans(void);

#endif // TRANS_COMBINATORS
