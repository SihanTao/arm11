#ifndef LINE_COMBINATOR
#define LINE_COMBINATOR

extern instruction_t e_instruction(AST ins_ast, int cur_address,
                            TokenStream token_stream, SymbolTable symbol_table,
                            int* end_address);
extern Parsec        p_line(void);
extern char * e_label(AST label);

#endif //LINE_COMBINATOR
