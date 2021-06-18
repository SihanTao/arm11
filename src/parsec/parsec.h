#ifndef PARSERC
#define PARSERC

#include <stdio.h>
#include <stdbool.h>

typedef int proposition(int);

typedef enum parsec_type
{
  PARSERC_OR,
  PARSERC_AND,
  PARSERC_UNIT,
  PARSERC_MATCH,
  PARSERC_TAKE,
  PARSERC_UNTIL
} parsec_type;

typedef struct parsec_t *Parsec;

typedef struct parsec_t
{
  parsec_type type;
  Parsec      curr;
  Parsec      next;
  char *       name;
  char *       matching_string;
  proposition *prop;
} parsec_t;

void print_pc(Parsec p, int ident);

AST perform_parse(CharStream char_stream, Parsec parserc,
                  ast_mapper map_while_build);
Parsec make_and(char* name, Parsec left, Parsec right);
Parsec make_or(char* name, Parsec left, Parsec right);
Parsec match(char *name, char *template);
Parsec seq(char* name, Parsec *sequence, size_t num);
Parsec alt(char* name, Parsec *choices, size_t num);
Parsec take_while(char *name, proposition accepts);
Parsec take_until(char *name, proposition until);
void free_parsec(Parsec parsec);

#endif // PARSERC
