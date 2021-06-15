#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <assert.h>

#include "ast.h"
#include "char_stream.h"

#include "parsec.h"

void print_indent(int indent)
{
  for (int i = 0; i < indent; i++)
  {
    printf(" ");
  }
}

void print_pc(Parsec p, int indent)
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
    print_pc(p->next, indent + 2);
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

bool identity(bool in)
{
  return in;
}

bool negate(bool in)
{
  return !in;
}

AST parse(CharStream char_stream, Parsec parserc, ast_mapper map_while_build)
{
  return parse_h(char_stream, parserc, map_while_build);
}

Parsec end()
{
  Parsec result = calloc(1, sizeof(parsec_t));
  result->type = PARSERC_UNIT;
  return result;
}

Parsec take_while(char *name, proposition accepts)
{
  Parsec result = calloc(1, sizeof(parsec_t));
  result->type  = PARSERC_TAKE;
  result->name  = name;
  result->prop  = accepts;
  return result;
}

Parsec take_until(char *name, proposition until)
{
  Parsec result = calloc(1, sizeof(parsec_t));
  result->type  = PARSERC_UNTIL;
  result->name  = name;
  result->prop  = until;
  return result;
}

Parsec make_or(char *name, Parsec left, Parsec right)
{
  Parsec result = calloc(1, sizeof(parsec_t));
  result->type  = PARSERC_OR;
  result->name  = name;
  result->curr  = left;
  result->next  = right;
  return result;
}

Parsec make_and(char *name, Parsec left, Parsec right)
{
  Parsec result = calloc(1, sizeof(parsec_t));
  result->type  = PARSERC_AND;
  result->name  = name;
  result->curr  = left;
  result->next  = right;
  return result;
}

Parsec alt_h(Parsec *choices, size_t num)
{
  if (num == 1)
  {
    return make_or(NULL, choices[0], end());
  }
  return make_or(NULL, choices[0], alt_h(choices + 1, num - 1));
}

Parsec alt(char *name, Parsec *choices, size_t num)
{
  assert(num >= 2);
  Parsec result = alt_h(choices, num);
  result->name  = name;
  return result;
}

Parsec seq_h(Parsec *sequence, size_t num)
{
  if (num == 1)
  {
    return make_and(NULL, sequence[0], end());
  }
  return make_and(NULL, sequence[0], seq_h(sequence + 1, num - 1));
}

Parsec seq(char *name, Parsec *sequence, size_t num)
{
  assert(num >= 2);
  Parsec result = seq_h(sequence, num);
  result->name  = name;
  return result;
}

Parsec match(char *name, char *template)
{
  Parsec result           = calloc(1, sizeof(parsec_t));
  result->name            = name;
  result->type            = PARSERC_MATCH;
  result->matching_string = template;
  return result;
}

AST parse_h(CharStream s, Parsec p, ast_mapper map)
{
  if (p == NULL)
  {
    return NULL;
  }

  switch (p->type)
  {
  case PARSERC_OR:
  {
    AST new_ast  = make_atom(p->name, NULL);
    AST matched1 = parse_h(s, p->curr, map);
    if (matched1 != NULL)
    {
      return add_child(new_ast, matched1);
    }
    matched1 = make_atom(NULL, NULL);

    AST matched2 = parse_h(s, p->next, map);
    if (matched2 != NULL)
    {
      if (p->next->type == PARSERC_OR)
      {
        return add_child(new_ast, merge(matched1, matched2));
      }
      add_brother(matched1, matched2);
      return add_child(new_ast, matched1);
    }

    return NULL;
  }
  case PARSERC_AND:
  {
    RecordPoint record_point = get_trace_back(s);
    AST         new_ast      = make_atom(p->name, NULL);
    AST         matched1     = parse_h(s, p->curr, map);
    if (matched1 != NULL)
    {
      AST matched2 = parse_h(s, p->next, map);
      if (matched2 != NULL)
      {
        if (p->next->type == PARSERC_AND)
        {
          return add_child(new_ast, merge(matched1, matched2));
        }
        add_brother(matched1, matched2);
        return add_child(new_ast, matched1);
      }
    }
    do_trace_back(s, record_point);
    return NULL;
  }
  case PARSERC_MATCH:
  {
    RecordPoint record_point = get_trace_back(s);
    AST         matched      = parse_match(p, s);
    if (matched)
    {
      return matched;
    }
    do_trace_back(s, record_point);
    return NULL;
  }
  case PARSERC_TAKE:
  {
    return parse_prop(p, s, identity);
  }
  case PARSERC_UNTIL:
  {
    return parse_prop(p, s, negate);
  }
  case PARSERC_UNIT:
  {
    return make_atom(NULL, NULL);
  }
  default:
    return NULL;
  }
}

AST parse_prop(Parsec p, CharStream s, bool(decorator)(bool))
{
  char  cur_char;
  char *buffer = calloc(100, 1);
  for (int i = 0; i < 99; i++, next_char(s))
  {
    cur_char = get_char(s);
    if (decorator(p->prop(cur_char)) && cur_char != '\0')
    {
      buffer[i] = cur_char;
    }
    else
    {
      break;
    }
  }
  return make_atom(p->name, buffer);
}

AST parse_match(Parsec p, CharStream s)
{
  char *buffer = calloc(100, 1);
  for (int i = 0; p->matching_string[i] != '\0' && i < 99; i++, next_char(s))
  {
    if (get_char(s) != p->matching_string[i])
    {
      return NULL;
    }
    buffer[i] = p->matching_string[i];
  }

  return make_atom(p->name, buffer);
}

// AST_Holder parse(CharStream char_stream, Parsec parserc, ast_mapper
// map_while_build)
// {
//   AST_Holder result = init_parserc_ast(map_while_build);
//   parse_h(parserc, char_stream, result);
//   free_parserc(parserc);
//   return result;
// }

// Parsec take_while(char *name, proposition accepts)
// {
//   Parsec result = calloc(1, sizeof(parsec_t));
//   result->name   = name;
//   result->type   = PARSERC_TAKE;
//   result->prop   = accepts;
//   return result;
// }

// Parsec take_until(char *name, proposition until)
// {
//   Parsec result = calloc(1, sizeof(parsec_t));
//   result->name   = name;
//   result->type   = PARSERC_TAKE;
//   result->prop   = until;
//   return result;
// }

// Parsec opt(char *name, Parsec target)
// {
//   Parsec result = calloc(1, sizeof(parsec_t));
//   result->name   = name;
//   result->type   = PARSERC_OPT;
//   result->curr   = target;
//   return result;
// }

// Parsec many(char *name, Parsec target)
// {
//   Parsec result = calloc(1, sizeof(parsec_t));
//   result->name   = name;
//   result->type   = PARSERC_STAR;
//   result->curr   = target;
//   return result;
// }

// Parsec at_least(char *name, Parsec target)
// {
//   Parsec sequence[2] = { target, many(NULL, target) };
//   return seq(name, sequence, 2);
// }

// void free_parserc(Parsec parserc)
// {
//   if (parserc == NULL)
//   {
//     return;
//   }

//   free_parserc(parserc->curr);
//   free_parserc(parserc->next);
//   free(parserc);
// }

// bool parse_match(Parsec p, CharStream char_stream, AST_Holder ast)
// {
//   RecordPoint record_point = get_trace_back(char_stream);
//   char        buffer[100];
//   if (match_h(char_stream, p->matching_string, buffer))
//   {
//     if (p->name != NULL)
//     {
//       build_parserc_ast(p->name, strdup(buffer), SIBLING);
//     }
//     return true;
//   }
//   do_trace_back(char_stream, record_point);
//   return false;
// }

// bool parse_take(Parsec p, CharStream char_stream, AST_Holder ast)
// {
//   char buffer[100];
//   ntake_while(char_stream, p->prop, buffer, 99);
//   if (p->name != NULL)
//   {
//     build_parserc_ast(p->name, strdup(buffer), SIBLING);
//   }
//   return true;
// }

// bool parse_until(Parsec p, CharStream char_stream, AST_Holder ast)
// {
//   char buffer[100];
//   ntake_until(char_stream, p->prop, buffer, 99);
//   if (p->name != NULL)
//   {
//     build_parserc_ast(p->name, strdup(buffer), SIBLING);
//   }
//   return true;
// }

// bool match_h(CharStream char_stream, char *template, char *buffer)
// {
//   if (get_char(char_stream) != *template || get_char(char_stream) == '\0'
//       || *template == '\0')
//   {
//     return false;
//   }
//   next_char(char_stream);
//   *buffer = *template;
//   return match_h(char_stream, template + 1, buffer + 1);
// }

// void ntake_while(char **char_stream, proposition accepts, char *buffer,
//                  size_t n)
// {
//   if (n == 0)
//   {
//     return;
//   }
//   if (accepts(**char_stream))
//   {
//     *buffer = get_char(char_stream);
//     next_char(char_stream);
//     ntake_while(char_stream, accepts, buffer + 1, n - 1);
//   }
// }

// void ntake_until(char **char_stream, proposition until, char *buffer, size_t
// n)
// {
//   if (n == 0)
//   {
//     return;
//   }
//   if (!until(**char_stream))
//   {
//     *buffer = get_char(char_stream);
//     next_char(char_stream);
//     ntake_until(char_stream, until, buffer + 1, n - 1);
//   }
// }
