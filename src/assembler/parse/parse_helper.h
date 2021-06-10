#ifndef PARSE_HELPER
#define PARSE_HELPER

typedef int NotSure;

extern void preprocess_and_gen_sym_table(char **     loaded_file,
                                         TokenStream  token_stream,
                                         SymbolTable symbol_tabl);

extern void tokenize(char * line, TokenStream token_stream);
extern bool is_label(char * line);
extern NotSure allocate_address(TokenStream token_queue, SymbolTable symbol_table);
extern bool is_label_line(char* line);
#endif // PARSE_HELPER
