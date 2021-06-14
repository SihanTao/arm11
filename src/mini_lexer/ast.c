#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ast.h"

AST_Holder init_parserc_ast(ast_mapper map_while_build)
{
  ast_node_t* root_holder = calloc(1, sizeof(ast_node_t));
  if (! root_holder)
  {
    perror("out of memory!!\n");
    return NULL;
  }

  AST_Holder result = calloc(1, sizeof(ast_holder_t));
  if (! result)
  {
    perror("out of memory!!\n");
    return NULL;
  }

  result->root_holder = root_holder;
  result->map_while_build = map_while_build;
  return result;
}

AST merge_into(AST parent, AST child)
{
  child->brother = parent->child;
  parent->child = child;
  return parent;
}

AST make_atom(char * name, char * matched)
{
  AST result = calloc(1, sizeof(ast_node_t));
  if (! result)
  {
    perror("out of memory!!\n");
    exit(EXIT_FAILURE);
  }

  result->key = name;
  result->matched = matched;
  return result;
}

// void build_parserc_ast(char * name, AST_Holder ast, char * buffer, bro_par b_p)
// {
//   ast_node_t* new_node = calloc(1, sizeof(ast_node_t));
//   if (! new_node)
//   {
//     perror("out of memory!!\n");
//     exit(EXIT_FAILURE);
//   }

//   new_node->key = name;
//   new_node->matched = buffer;

//   if (b_p == PARENT)
//   {
//     new_node->child = ast->root_holder;
//     ast->root_holder = new_node;
//   }
//   else
//   {
//     new_node->brother = ast->root_holder;
//     ast->root_holder = new_node;
//   }

//   if (ast->map_while_build != NULL)
//   {
//     ast->root_holder  = ast->map_while_build(ast->root_holder);
//   }
// }

void free_ast_holder(AST_Holder ast_holder)
{
  free_ast(ast_holder->root_holder);
  free(ast_holder);
}

void free_ast(ast_node_t* ast_node)
{
  if (ast_node == NULL)
  {
    return;
  }

  free_ast(ast_node->brother);
  free_ast(ast_node->child);

  free(ast_node);
}

ast_node_t* get_child_by_name(ast_node_t* ast, char* name)
{
  if (ast == NULL)
  {
    return NULL;
  }
  return get_brother_by_name(ast->child, name);
}

ast_node_t* get_brother_by_name(ast_node_t* ast, char * name)
{
  if (ast == NULL || ast->key == NULL)
  {
    return NULL;
  }

  if (strcmp(name, ast->key) == 0)
  {
    return ast;
  }

  return get_brother_by_name(ast->brother, name);
}


char * get_matched(ast_node_t* ast)
{
  return ast->matched;
}

void print_ast(AST ast, int indent)
{
  if (ast == NULL || ast->key == NULL)
  {
    return;
  }

  for (int i = 0; i < indent;i++)
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