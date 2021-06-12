#ifndef PARSE
#define PARSE

#define ADDRESS_INTERVAL (4)

/*
 * read the file and generate a token stream and a symbol table
 *
 * `token_stream` and `symbol_table` are returned params
 *
 * usage
 * ```
 * TokenStream token_stream = init_token_stream();
 * SymbolTable symbol_table = init_symbol_table();
 * parse(file_name, token_stream, symbol_table);
 * ```
 */
extern void parse(char **loaded_file, TokenStream token_stream,
             SymbolTable symbol_table);

extern bool is_label_line(char *line);

#endif // PARSE
