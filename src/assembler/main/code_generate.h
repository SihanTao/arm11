#ifndef CODE_GENERATE
#define CODE_GENERATE

#define ADDRESS_INTERVAL (4)
#define MAX_LINE_LENGTH  (512)

extern void code_generate(char *file_name, TokenStream token_stream,
                          SymbolTable symbol_table, int *end_address);

#endif // CODE_GENERATE
