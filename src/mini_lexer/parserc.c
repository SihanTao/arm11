#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "ast.h"
#include "char_stream.h"

#include "parserc.h"

void print_indent(int indent)
{
  for (int i = 0; i < indent; i++)
  {
    printf(" ");
  }
}

void print_pc(Parserc p, int indent)
{
  if (p == NULL)
  {
    return;
  }
  print_indent(indent);
  printf("%d, %s\n", p->type, p->name);
  if (p->type == PARSERC_AND || p->type == PARSERC_OR)
  {
    print_pc(p->curr, indent + 2);
    print_pc(p->next, indent);
  }
  else if (p->type == PARSERC_MATCH)
  {
    print_indent(indent + 2);
    printf("%s\n", p->matching_string);
  }
  else
  {
    print_pc(p->curr, indent + 2);
  }
}

AST_Holder parse(CharStream char_stream, Parserc parserc, ast_mapper map_while_build)
{
  AST_Holder result = init_parserc_ast(map_while_build);
  parse_h(parserc, char_stream, result);
  free_parserc(parserc);
  return result;
}

Parserc one_of(char *name, Parserc *choices, size_t num)
{
  Parserc result = calloc(1, sizeof(parserc_t));
  result->name   = name;
  result->type   = PARSERC_OR;
  Parserc cur    = result;
  cur->curr      = *choices;
  if (num != 0)
  {
    cur->next = one_of(name, choices + 1, num - 1);
  }
  return result;
}

Parserc seq(char *name, Parserc *sequence, size_t num)
{
  Parserc result = calloc(1, sizeof(parserc_t));
  result->name   = name;
  result->type   = PARSERC_AND;
  Parserc cur    = result;
  cur->curr      = *sequence;
  if (num != 0)
  {
    cur->next = seq(name, sequence + 1, num - 1);
  }
  return result;
}

Parserc match(char *name, char *template)
{
  Parserc result          = calloc(1, sizeof(parserc_t));
  result->name            = name;
  result->type            = PARSERC_MATCH;
  result->matching_string = template;
  return result;
}

Parserc take_while(char *name, proposition accepts)
{
  Parserc result = calloc(1, sizeof(parserc_t));
  result->name   = name;
  result->type   = PARSERC_TAKE;
  result->prop   = accepts;
  return result;
}

Parserc take_until(char *name, proposition until)
{
  Parserc result = calloc(1, sizeof(parserc_t));
  result->name   = name;
  result->type   = PARSERC_TAKE;
  result->prop   = until;
  return result;
}

Parserc opt(char *name, Parserc target)
{
  Parserc result = calloc(1, sizeof(parserc_t));
  result->name   = name;
  result->type   = PARSERC_OPT;
  result->curr   = target;
  return result;
}

Parserc many(char *name, Parserc target)
{
  Parserc result = calloc(1, sizeof(parserc_t));
  result->name   = name;
  result->type   = PARSERC_STAR;
  result->curr   = target;
  return result;
}

Parserc at_least(char *name, Parserc target)
{
  Parserc sequence[2] = { target, many(NULL, target) };
  return seq(name, sequence, 2);
}

void free_parserc(Parserc parserc)
{
  if (parserc == NULL)
  {
    return;
  }

  free_parserc(parserc->curr);
  free_parserc(parserc->next);
  free(parserc);
}

bool parse_h(Parserc p, CharStream char_stream, AST_Holder ast)
{
  if (p == NULL)
  {
    return false;
  }

  switch (p->type)
  {
  case PARSERC_OR:
  {
    if (parse_h(p->curr, char_stream, ast)
        || parse_h(p->next, char_stream, ast))
    {
      build_parserc_ast(p->name, ast, NULL, PARENT);
      return true;
    }
  }
  case PARSERC_AND:
  {
    RecordPoint record_point = get_trace_back(char_stream);
    if (parse_h(p->curr, char_stream, ast)
        && parse_h(p->next, char_stream, ast))
    {
      build_parserc_ast(p->name, ast, NULL, PARENT);
      return true;
    }
    do_trace_back(char_stream, record_point);
    return false;
  }
  case PARSERC_OPT:
  {
    if (parse_h(p->curr, char_stream, ast))
    {
      build_parserc_ast(p->name, ast, NULL, PARENT);
    }
    return true;
  }
  case PARSERC_MATCH:
  {
    return parse_match(p, char_stream, ast);
  }
  case PARSERC_TAKE:
  {
    return parse_take(p, char_stream, ast);
  }
  case PARSERC_UNTIL:
  {
    return parse_until(p, char_stream, ast);
  }
  case PARSERC_STAR:
  {
    if (parse_h(p->curr, char_stream, ast))
    {
      parse_h(p->curr, char_stream, ast);
    }
    build_parserc_ast(p->name, ast, NULL, PARENT);
    return true;
  }
  }
}

bool parse_match(Parserc p, CharStream char_stream, AST_Holder ast)
{
  RecordPoint record_point = get_trace_back(char_stream);
  char        buffer[100];
  if (match_h(char_stream, p->matching_string, buffer))
  {
    if (p->name != NULL)
    {
      build_parserc_ast(p->name, strdup(buffer), SIBLING);
    }
    return true;
  }
  do_trace_back(char_stream, record_point);
  return false;
}

bool parse_take(Parserc p, CharStream char_stream, AST_Holder ast)
{
  char buffer[100];
  ntake_while(char_stream, p->prop, buffer, 99);
  if (p->name != NULL)
  {
    build_parserc_ast(p->name, strdup(buffer), SIBLING);
  }
  return true;
}

bool parse_until(Parserc p, CharStream char_stream, AST_Holder ast)
{
  char buffer[100];
  ntake_until(char_stream, p->prop, buffer, 99);
  if (p->name != NULL)
  {
    build_parserc_ast(p->name, strdup(buffer), SIBLING);
  }
  return true;
}

bool match_h(CharStream char_stream, char *template, char *buffer)
{
  if (get_char(char_stream) != *template || get_char(char_stream) == '\0'
      || *template == '\0')
  {
    return false;
  }
  next_char(char_stream);
  *buffer = *template;
  return match_h(char_stream, template + 1, buffer + 1);
}

void ntake_while(char **char_stream, proposition accepts, char *buffer,
                 size_t n)
{
  if (n == 0)
  {
    return;
  }
  if (accepts(**char_stream))
  {
    *buffer = get_char(char_stream);
    next_char(char_stream);
    ntake_while(char_stream, accepts, buffer + 1, n - 1);
  }
}

void ntake_until(char **char_stream, proposition until, char *buffer, size_t n)
{
  if (n == 0)
  {
    return;
  }
  if (!until(**char_stream))
  {
    *buffer = get_char(char_stream);
    next_char(char_stream);
    ntake_until(char_stream, until, buffer + 1, n - 1);
  }
}
