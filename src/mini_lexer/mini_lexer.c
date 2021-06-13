#include <stdbool.h>
#include <string.h>
#include <ctype.h>

typedef bool (*proposition)(char);
typedef bool (*partial)(char *, char);
typedef bool (*parsec)(parsec, parsec, AST);

typedef struct tree_t
{
  char *       key;
  char *       matched;
  struct list *brother;
  struct list *child;
} tree_t;

typedef tree_t *AST;

typedef enum parsec_type
{
  PARSEC_OR,
  PARSEC_AND,
  PARSEC_STAR,
  PARSEC_OPT,
  PARSEC_MATCH,
  PARSEC_TAKE
} parsec_type;

typedef struct parsec_t *Parsec;

typedef struct parsec_or_t
{
  Parsec this;
  struct parsec_or_t *next;
} parsec_or_t;

typedef struct parsec_and_t
{
  Parsec this;
  struct parsec_and_t *next;
} parsec_and_t;

typedef struct parsec_star_t
{
  Parsec this;
} parsec_star_t;

typedef struct parsec_opt_t
{
  Parsec this;
} parsec_opt_t;

typedef struct parsec_match_t
{
  char *matching_string;
} parsec_match_t;

typedef struct parsec_take_t
{
  proposition prop;
} parsec_take_t;

typedef struct parsec_t
{
  parsec_type type;
  union
  {
    parsec_or_t or ;
    parsec_and_t and;
    parsec_star_t  star;
    parsec_opt_t   opt;
    parsec_match_t match;
    parsec_take_t  take;
  } data;
} parsec_t;

parsec seq(parsec front, parsec back, AST ast) {}

bool parse_h(Parsec p, char **string_holder, AST ast)
{
  if (p == NULL)
  {
    return false;
  }
  char *cur_pos = *string_holder;
  switch (p.type)
  {
  case PARSEC_OR:
  {
    if (parse(p.frist, string_holder, ast)
        || parse(p.second, string_holder, ast))
    {
      return true;
    }
    // trace back
    *string_holder = cur_pos;
    return false;
  }
  case PARSEC_AND:
  {
    if (parse(p.first, string_holder, ast)
        && parse(p.second, string_holder, ast))
    {
      return true;
    }
    // trace back
    *string_holder = cur_pos;
    return false;
  }
  case PARSEC_ATOM:
  {
  }
  }
}

bool parse_or(parsec_or_t *p, char **string_holder,
              AST ast)
{ return parse_h(p->this, string_holder, ast)
                               || parse_or(p->next, string_holder, ast) }

parsec register(void)
{
  return (seq(match("r"), take_while(isnumber, "reg name")));
}

parsec operands(void)
{
  return (
      seq(register(), seq(star(match(" ")),
                          seq(register(), seq(star(match(" ")), register())))))
}

bool alt(parsec either, parsec or, AST ast) {}

bool ntake_while_(char **cur_pos, proposition accepts, AST ast, size_t n) {}

void ntake_while(char **cur_pos, proposition accepts, char *buffer, size_t n)
{
  if (n == 0)
  {
    return;
  }
  if (accepts(**cur_pos))
  {
    *buffer  = **cur_pos;
    *cur_pos = *cur_pos + 1;
    ntake_while(cur_pos, accepts, buffer + 1, n - 1);
  }
}

void ntake_until(char **cur_pos, proposition until, char *buffer, size_t n)
{
  if (n == 0)
  {
    return;
  }
  if (!until(**cur_pos))
  {
    *buffer  = **cur_pos;
    *cur_pos = *cur_pos + 1;
    ntake_until(cur_pos, until, buffer + 1, n - 1);
  }
}

// bool match(char ** cur_pos, char * template)
// {
//   char * trace_back = *cur_pos;
//   if (match_h(cur_pos, template))
//   {
//     return true;
//   }
//   *cur_pos = trace_back;
//   return false;
// }

// bool match_h(char ** cur_pos, char * template)
// {
//   if (**cur_pos != *template || **cur_pos == '\0' || *template == '\0')
//   {
//     return false;
//   }
//   *cur_pos = *cur_pos + 1;
//   return match_h(cur_pos, template);
// }

// proposition belongs_to(char * set)
// {
//   bool inner(char elem)
//   {
//     return belongs_to_h(set, elem)
//   }
//   return inner;
// }

// bool belongs_to_h(char * set, char elem)
// {
//   if (*set == elem)
//   {
//     return true;
//   }
//   if (*set == '\0')
//   {
//     return false;
//   }
//   return belongs_to_h(set + 1, elem);
// }

void ntake_while(char **cur_pos, proposition accepts, char *buffer, size_t n)
{
  if (n == 0)
  {
    return;
  }
  if (accepts(**cur_pos))
  {
    *buffer  = **cur_pos;
    *cur_pos = *cur_pos + 1;
    ntake_while(cur_pos, accepts, buffer + 1, n - 1);
  }
}

void ntake_until(char **cur_pos, proposition until, char *buffer, size_t n)
{
  if (n == 0)
  {
    return;
  }
  if (!until(**cur_pos))
  {
    *buffer  = **cur_pos;
    *cur_pos = *cur_pos + 1;
    ntake_until(cur_pos, until, buffer + 1, n - 1);
  }
}

bool arith(char **string_holder, Token token)
{
  char *     trace_back = string_holder;
  mnemonic_t mnemonic;
  if (match(string_holder, "add "))
  {
    if (arith_continue(string_holder, token))
    {
      return true;
    }
  }
  //
  //

  *string_holder = trace_back;

  return false;
}

bool arith_continue(char **string_holder, Token token)
{
  char *trace_back = string_holder;
  char  buffer[10];
  if (match(string_holder, "r"))
  {
    ntake_while(string_holder, isnumber, buffer, 10);
    token->operands[0] = atoi(buffer);
    if (match(string_holder, ", r"))
    {
      ntake_while(string_holder, isnumber, buffer, 10);
      token->operands[1] = atoi(buffer);
    }
    if (match(string_holder, ", r"))
    {
      ntake_while(string_holder, isnumber, buffer, 10);
    }
  }
}
