#include <string.h>
#include <stdlib.h>

#include "symbol_table.h"

static symbol_node_t * init_symbol_node(char * symbol_data, int fake_address);
void add_node(char * symbol_data, int fake_address, symbol_node_t ** node_ptr);
void free_node(symbol_node_t * node);
static symbol_node_t * init_symbol_node(char * symbol_data, int fake_address);
void add_node(char * symbol_data, int fake_address, symbol_node_t ** node_ptr);
int find_node(char * symbol_data, symbol_node_t * node);

SymbolTable init_symbol_table(void)
{
  SymbolTable result = malloc(sizeof(symbol_table_t));
  result->head_holder = init_symbol_node(NULL, NULL);

  return result;
}

void add_symbol_table(char * symbol_data, int fake_address, SymbolTable table)
{
  if (! table)
  {
    perror("Table is null\n");
    exit(EXIT_FAILURE);
  }

  add_node(symbol_data, fake_address, &table->head_holder);
}

int find_symbol_table(char * symbol_data, SymbolTable table)
{
  if (! table)
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

void free_node(symbol_node_t * node)
{
  free(node->symbol_data);
  free_node(node->left);
  free_node(node->right);
  free(node);
}

static symbol_node_t * init_symbol_node(char * symbol_data, int fake_address)
{
  symbol_node_t * result = malloc(sizeof(symbol_node_t));
  if (! result)
  {
    perror("out of memory!!\n");
    exit(EXIT_FAILURE);
  }

  result->fake_address = fake_address;
  result->symbol_data = symbol_data;
  result->left = NULL;
  result->right = NULL;

  return result;
}


void add_node(char * symbol_data, int fake_address, symbol_node_t ** node_ptr)
{
  if (*node_ptr == NULL)
  {
    (*node_ptr) = init_symbol_node(symbol_data, fake_address);
  }

  symbol_node_t * node = *node_ptr;
  int cmp = strcmp(symbol_data, node->symbol_data);
  if (cmp > 0)
  {
    add_node(symbol_data, fake_address, &node->left);
  }
  else if (cmp < 0)
  {
    add_node(symbol_data, fake_address, &node->right);
  }
  else
  {
    perror("Error! Same Label being defined twice!");
    exit(EXIT_FAILURE);
  }
}


int find_node(char * symbol_data, symbol_node_t * node)
{
  if (! node)
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
    return node->fake_address;
  }
}
