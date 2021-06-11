#ifndef PARSE
#define PARSE

#define ADDRESS_INTERVAL (4)

#define MAX_LINE_LENGTH (512)

extern parse(char *loaded_file, TokenStream token_stream,
             SymbolTable symbol_table);

extern bool is_label_line(char *line);

#endif // PARSE
