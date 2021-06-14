#ifndef PARSERC
#define PARSERC

#include <stdio.h>
#include <stdbool.h>

typedef int proposition(int);

typedef enum parsec_type
{
  PARSERC_OR,
  PARSERC_AND,
  PARSERC_STAR,
  PARSERC_OPT,
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

// bool parse_h(Parserc p, char **string_holder, AST_Holder ast, brother_child b_c);
// bool parse_match(Parserc p, CharStream string_holder, AST_Holder ast,
//                  brother_child b_c);
// bool parse_take(Parserc p, CharStream char_stream, AST_Holder ast, brother_child b_c);
// bool parse_until(Parserc p, CharStream char_stream, AST_Holder ast,
//                  brother_child b_c);
// bool match_h(CharStream char_stream, char *template, char *buffer);
// void ntake_while(char **char_stream, proposition accepts, char *buffer,
//                  size_t n);
// void ntake_until(char **char_stream, proposition until, char *buffer,
//                  size_t n);
// void free_parserc(Parserc parserc);

// AST_Holder     parse(CharStream char_stream, Parserc parsec);
// Parserc alt(char *name, Parserc *choices, size_t num);
// Parserc seq(char *name, Parserc *sequence, size_t num);
// Parserc match(char *name, char *template);
// Parserc take_while(char *name, proposition accepts);
// Parserc take_until(char *name, proposition until);
// Parserc opt(char *name, Parserc target);
// Parserc many(char *name, Parserc target);
// Parserc at_least(char *name, Parserc target);

void print_pc(Parsec p, int ident);

AST parse(CharStream char_stream, Parsec parserc, ast_mapper map_while_build);
Parsec make_and(char* name, Parsec left, Parsec right);
Parsec make_or(char* name, Parsec left, Parsec right);
Parsec match(char *name, char *template);
AST parse_h(CharStream s, Parsec p, ast_mapper map);
AST parse_match(Parsec p, CharStream s);
Parsec seq(char* name, Parsec *sequence, size_t num);
Parsec take_while(char *name, proposition accepts);
Parsec take_until(char *name, proposition until);
AST parse_prop(Parsec p, CharStream s, bool(decorator)(bool));

#endif // PARSERC
