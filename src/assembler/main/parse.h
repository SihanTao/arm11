#ifndef PARSE_ASSEMBLE
#define PARSE_ASSEMBLE

#define ADDRESS_INTERVAL (4)
#define MAX_LINE_LENGTH  (512)

extern void parse(char *file_name, TokenStream token_stream,
                  SymbolTable symbol_table, int *end_address);

#endif // PARSE_ASSEMBLE
