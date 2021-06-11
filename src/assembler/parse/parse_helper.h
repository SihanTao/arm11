#ifndef PARSE_HELPER
#define PARSE_HELPER

typedef int NotSure;

#define ADDRESS_INTERVAL (4)

extern void preprocess_and_gen_sym_table(char **     loaded_file,
                                         TokenStream  token_stream,
                                         SymbolTable symbol_tabl);

extern bool is_label(char * line);
extern NotSure allocate_address(TokenStream token_queue, SymbolTable symbol_table);
#endif // PARSE_HELPER
