#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ast.h"

/*!
 *
 * @param self
 * @param child
 * @return : add child into self
 */
AST add_child(AST self, AST child)
{
  self->child = child;
  return self;
}

/*!
 *
 * @param self
 * @param brother
 * @return : add brother into self
 */
AST add_brother(AST self, AST brother)
{
  self->brother = brother;
  return self;
}

/*!
 *
 * @param self
 * @param brother_parent
 * @return : merge brother_parent into self
 */
AST merge(AST self, AST brother_parent)
{
  self->brother = brother_parent->child;
  brother_parent->child = NULL;
  free_ast(brother_parent);
  return self;
}

/*!
 *
 * @param name
 * @param matched
 * @return : create AST node with key = name and matched = matched
 */
AST make_atom(char *name, char *matched)
{
  AST result = calloc(1, sizeof(ast_node_t));
  if (!result)
  {
    perror("out of memory!!\n");
    exit(EXIT_FAILURE);
  }

  result->key     = name;
  result->matched = matched;
  return result;
}

/*!
 *
 * @param ast_node
 * @return : free AST node
 */
void free_ast(ast_node_t *ast_node)
{
  if (ast_node == NULL)
  {
    return;
  }

  free_ast(ast_node->brother);
  free_ast(ast_node->child);
  free(ast_node->matched);

  free(ast_node);
}

/*!
 *
 * @param ast
 * @param name
 * @return the child which called 'name' of ast
 */
ast_node_t *get_child_by_name(ast_node_t *ast, char *name)
{
  if (ast == NULL)
  {
    return NULL;
  }
  return get_brother_by_name(ast->child, name);
}

/*!
 *
 * @param ast
 * @param name
 * @return the brother which called 'name' of ast
 */
ast_node_t *get_brother_by_name(ast_node_t *ast, char *name)
{
  if (ast == NULL)
  {
    return NULL;
  }

  if (ast->key == NULL)
  {
    return get_brother_by_name(ast->brother, name);
  }

  if (strcmp(name, ast->key) == 0)
  {
    return ast;
  }

  return get_brother_by_name(ast->brother, name);
}

/*!
 *
 * @param ast
 * @return the matched of ast
 */
char *get_matched(ast_node_t *ast)
{
  return ast->matched;
}

void print_ast(AST ast, int indent)
{
  if (ast == NULL)
  {
    return;
  }

  for (int i = 0; i < indent; i++)
  {
    printf(" ");
  }

  printf("name : %s, ", ast->key);
  if (ast->matched != NULL)
  {
    printf("matched : %s ", ast->matched);
  }
  if (ast->child != NULL)
  {
    printf("\n");
    print_ast(ast->child, indent + 2);
  }
  if (ast->brother != NULL)
  {
    printf("\n");
    print_ast(ast->brother, indent);
  }
}