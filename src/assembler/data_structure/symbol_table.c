#include <string.h>
#include <stdlib.h>

#include "symbol_table.h"

static symbol_node_t *init_symbol_node(char *symbol_data, int address);
static void add_node(char *symbol_data, int address, symbol_node_t **node_ptr);
static void free_node(symbol_node_t *node);

SymbolTable init_symbol_table(void)
{
  SymbolTable result  = malloc(sizeof(symbol_table_t));
  result->head_holder = init_symbol_node(NULL, NULL);

  return result;
}

void add_symbol_table(char *symbol_data, int address, SymbolTable table)
{
  if (!table)
  {
    perror("Table is null\n");
    exit(EXIT_FAILURE);
  }
  // strlen(symbol_data) - 1(';') + 1('\0')
  char *copied = malloc(strlen(symbol_data));
  memcpy(copied, symbol_data, strlen(symbol_data));

  add_node(copied, address, &table->head_holder);
}

int find_symbol_table(char *symbol_data, SymbolTable table)
{
  if (!table)
  {
    perror("Table is null!\n");
    exit(EXIT_FAILURE);
  }

  return find_node(symbol_data, table->head_holder);
}

void free_symbol_table(SymbolTable table)
{
  free_node(table->head_holder);
  free(table);
}

void free_node(symbol_node_t *node)
{
  free(node->symbol_data);
  free_node(node->left);
  free_node(node->right);
  free(node);
}

static symbol_node_t *init_symbol_node(char *symbol_data, int address)
{
  symbol_node_t *result = malloc(sizeof(symbol_node_t));
  if (!result)
  {
    perror("out of memory!!\n");
    exit(EXIT_FAILURE);
  }

  result->address_to_next_ins = address;
  result->symbol_data         = symbol_data;
  result->left                = NULL;
  result->right               = NULL;

  return result;
}

void add_node(char *symbol_data, int address, symbol_node_t **node_ptr)
{
  if (*node_ptr == NULL)
  {
    (*node_ptr) = init_symbol_node(symbol_data, address);
  }

  symbol_node_t *node = *node_ptr;
  int            cmp  = strcmp(symbol_data, node->symbol_data);
  if (cmp > 0)
  {
    add_node(symbol_data, address, &node->left);
  }
  else if (cmp < 0)
  {
    add_node(symbol_data, address, &node->right);
  }
  else
  {
    perror("Error! Same Label being defined twice!");
    exit(EXIT_FAILURE);
  }
}

int find_node(char *symbol_data, symbol_node_t *node)
{
  if (!node)
  {
    perror("Error! Undefined symbol! %s\n", symbol_data);
    exit(EXIT_FAILURE);
  }

  int cmp = strcmp(symbol_data, node->symbol_data);
  if (cmp > 0)
  {
    return find_node(symbol_data, node->left);
  }
  else if (cmp < 0)
  {
    return find_node(symbol_data, node->right);
  }
  else
  {
    return node->address_to_next_ins;
  }
}
