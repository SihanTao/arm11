#ifndef PARSERC
#define PARSERC

#include <stdio.h>
#include <stdbool.h>

typedef int proposition(int);

typedef enum parserc_type
{
  PARSERC_OR,
  PARSERC_AND,
  PARSERC_STAR,
  PARSERC_OPT,
  PARSERC_MATCH,
  PARSERC_TAKE,
  PARSERC_UNTIL
} parserc_type;

typedef struct parserc_t *Parserc;

typedef struct parserc_t
{
  parserc_type type;
  Parserc      curr;
  Parserc      next;
  char *       name;
  char *       matching_string;
  proposition *prop;
} parserc_t;

bool parse_h(Parserc p, char **string_holder, AST_Holder ast, brother_child b_c);
bool parse_match(Parserc p, CharStream string_holder, AST_Holder ast,
                 brother_child b_c);
bool parse_take(Parserc p, CharStream char_stream, AST_Holder ast, brother_child b_c);
bool parse_until(Parserc p, CharStream char_stream, AST_Holder ast,
                 brother_child b_c);
bool match_h(CharStream char_stream, char *template, char *buffer);
void ntake_while(char **char_stream, proposition accepts, char *buffer,
                 size_t n);
void ntake_until(char **char_stream, proposition until, char *buffer,
                 size_t n);
void free_parserc(Parserc parserc);

AST_Holder     parse(CharStream char_stream, Parserc parsec);
Parserc one_of(char *name, Parserc *choices, size_t num);
Parserc seq(char *name, Parserc *sequence, size_t num);
Parserc match(char *name, char *template);
Parserc take_while(char *name, proposition accepts);
Parserc take_until(char *name, proposition until);
Parserc opt(char *name, Parserc target);
Parserc many(char *name, Parserc target);
Parserc at_least(char *name, Parserc target);

void print_pc(Parserc p, int ident);




#endif // PARSERC
