#ifndef TOKENIZE
#define TOKENIZE

extern token_t *tokenize(char *line, int address, TokenStream token_stream);
extern char *   get_opcode(token_t *token, char *instruction);
extern int      count_num_operand(char *rest);
extern char **  split_operand_field(char *rest, int length);
extern void     set_token_operand(token_t *token, char **operand_field,
                                  int length);
extern void     print_token(token_t *token);

#endif //TOKENIZE


