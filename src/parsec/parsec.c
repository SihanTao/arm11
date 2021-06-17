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

/*!
 *
 * @param in
 * @return a bool value of whether in is true
 */
bool identity(bool in)
{
  return in;
}

/*!
 *
 * @param in
 * @return a bool value of whether in is false
 */
bool negate(bool in)
{
  return !in;
}

/*!
 *
 * @param char_stream
 * @param parserc
 * @param map_while_build
 * @return an AST and it depends on parserc.type (PARSERC_OR, PARSERC_AND, PARSERC_MATCH, 
 *                                          PARSERC_TAKE, PARSERC_UNTIL, PARSERC_UNIT)
 */
AST perform_parse(CharStream char_stream, Parsec parserc,
                  ast_mapper map_while_build)
{
  return parse_h(char_stream, parserc, map_while_build);
}

/*!
 *
 * @return a parsec with type = PARSERC_UNIT
 */
Parsec end()
{
  Parsec result = calloc(1, sizeof(parsec_t));
  result->type  = PARSERC_UNIT;
  return result;
}

/*!
 *
 * @param name
 * @param accepts
 * @return a parsec with type = PARSERC_TAKE, name = name, prop = accepts
 */
Parsec take_while(char *name, proposition accepts)
{
  Parsec result = calloc(1, sizeof(parsec_t));
  result->type  = PARSERC_TAKE;
  result->name  = name;
  result->prop  = accepts;
  return result;
}

/*!
 *
 * @param name
 * @param until
 * @return a parsec with type = PARSERC_UNTIL, name = name, prop = until
 */
Parsec take_until(char *name, proposition until)
{
  Parsec result = calloc(1, sizeof(parsec_t));
  result->type  = PARSERC_UNTIL;
  result->name  = name;
  result->prop  = until;
  return result;
}

/*!
 *
 * @param name
 * @param left
 * @param right
 * @return a parsec with type = PARSERC_OR, name = name, curr = left, next = right
 */
Parsec make_or(char *name, Parsec left, Parsec right)
{
  Parsec result = calloc(1, sizeof(parsec_t));
  result->type  = PARSERC_OR;
  result->name  = name;
  result->curr  = left;
  result->next  = right;
  return result;
}

/*!
 *
 * @param name
 * @param left
 * @param right
 * @return a parsec with type = PARSERC_AND, name = name, curr = left, next = right
 */
Parsec make_and(char *name, Parsec left, Parsec right)
{
  Parsec result = calloc(1, sizeof(parsec_t));
  result->type  = PARSERC_AND;
  result->name  = name;
  result->curr  = left;
  result->next  = right;
  return result;
}

/*!
 *
 * @param choices
 * @param num
 * @return a parsec with type = PARSERC_OR, name = NULL, curr = choices[0],
 *         next = end() if num = 1 else next = alt_h(choices + 1, num - 1)
 */
Parsec alt_h(Parsec *choices, size_t num)
{
  if (num == 1)
  {
    return make_or(NULL, choices[0], end());
  }
  return make_or(NULL, choices[0], alt_h(choices + 1, num - 1));
}

/*!
 *
 * @param name
 * @param choices
 * @param num
 * @return a parsec with type = PARSERC_OR, name = name, curr = choices[0],
 *         next = end() if num = 1 else next = alt_h(choices + 1, num - 1)
 */
Parsec alt(char *name, Parsec *choices, size_t num)
{
  assert(num >= 1);
  Parsec result = alt_h(choices, num);
  result->name  = name;
  return result;
}

/*!
 *
 * @param sequence
 * @param num
 * @return a parsec with type = PARSERC_AND, name = NULL, curr = sequence[0], 
 *         next = end() if num = 1 else next = seq_h(sequence + 1, num - 1)
 */
Parsec seq_h(Parsec *sequence, size_t num)
{
  if (num == 1)
  {
    return make_and(NULL, sequence[0], end());
  }
  return make_and(NULL, sequence[0], seq_h(sequence + 1, num - 1));
}

/*!
 *
 * @param name
 * @param sequence
 * @param num
 * @return a parsec with type = PARSERC_AND, name = name, curr = sequence[0], 
 *         next = end() if num = 1 else next = seq_h(sequence + 1, num - 1)
 */
Parsec seq(char *name, Parsec *sequence, size_t num)
{
  assert(num >= 1);
  Parsec result = seq_h(sequence, num);
  result->name  = name;
  return result;
}

/*!
 *
 * @param name
 * @param template
 * @return a parsec with name = name, type = PARSERC_MATCH, matching_string = template
 */
Parsec match(char *name, char *template)
{
  Parsec result           = calloc(1, sizeof(parsec_t));
  result->name            = name;
  result->type            = PARSERC_MATCH;
  result->matching_string = template;
  return result;
}

/*!
 *
 * @param s
 * @param p
 * @param map
 * @return an AST and it depends on p.type (PARSERC_OR, PARSERC_AND, PARSERC_MATCH, 
 *                                          PARSERC_TAKE, PARSERC_UNTIL, PARSERC_UNIT)
 */
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
    RecordPoint record_point = get_trace_back(s);
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
        do_trace_back(s, record_point);
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

/*!
 *
 * @param p
 * @param s
 * @param decorator
 * @return an AST with key = p.name and matched = buffer
 */
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

/*!
 *
 * @param p
 * @param s
 * @return an AST with key = p.name and matched = buffer
 */
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

/*!
 *
 * @param parsec
 * @return : free parsec
 */
void free_parsec(Parsec parsec)
{
  if (parsec == NULL)
  {
    return;
  }

  free_parsec(parsec->next);
  free_parsec(parsec->curr);

  free(parsec);
}
