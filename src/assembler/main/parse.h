#ifndef PARSE_ASSEMBLE
#define PARSE_ASSEMBLE

extern void parse(char *file_name, TokenStream token_stream,
                  SymbolTable symbol_table, int *end_address);

#endif // PARSE_ASSEMBLE
