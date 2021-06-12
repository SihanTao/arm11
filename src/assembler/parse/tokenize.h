#ifndef TOKENIZE
#define TOKENIZE

extern token_t *tokenize(char *line);
extern void trim(char **cur_pos, char until, char **got);

#endif //TOKENIZE


