#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "symbol_table.h"

static symbol_node_t *init_symbol_node(char *symbol_data, int address);
static void add_node(char *symbol_data, int address, symbol_node_t *node);
static int  find_node(char *symbol_data, symbol_node_t *node_ptr);
static void free_node(symbol_node_t *node);

SymbolTable init_symbol_table(void)
{
  SymbolTable result = malloc(sizeof(symbol_table_t));
  result->head_holder = init_symbol_node(strdup("hello"), 0);

  return result;
}

/*!
 * 
 * @param symbol_data
 * @param address
 * @param table
 * @return : add the symbol data to the table in the given address
 */
void add_symbol_table(char *symbol_data, int address, SymbolTable table)
{
  if (!table)
  {
    perror("Table is null\n");
    exit(EXIT_FAILURE);
  }

  add_node(symbol_data, address, table->head_holder);
}

/*!
 * 
 * @param symbol_data
 * @param table
 * @return : find the given symbol data from symboltable
 */
int find_symbol_table(char *symbol_data, SymbolTable table)
{
  if (!table)
  {
    perror("Table is null!\n");
    exit(EXIT_FAILURE);
  }

  return find_node(symbol_data, table->head_holder);
}

/*!
 * 
 * @param table
 * @return : free every node from the symbol table
 */
void free_symbol_table(SymbolTable table)
{
  free_node(table->head_holder);
  free(table);
}

/*!
 * 
 * @param node
 * @return : free the node
 */
void free_node(symbol_node_t *node)
{
  if (node == NULL)
  {
    return;
  }
  free(node->symbol_data);
  free_node(node->left);
  free_node(node->right);
  free(node);
}

/*!
 * 
 * @param symbol_data
 * @param address
 * @return : initialize the symbol data
 */
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

/*!
 * 
 * @param symbol_data
 * @param address
 * @param node 
 * @return : add the node which the key is equal to the symbol data 
 *           to the symbol node(binary tree) in the given address
 */
void add_node(char *symbol_data, int address, symbol_node_t *node)
{
  int cmp = strcmp(symbol_data, node->symbol_data);
  if (cmp > 0)
  {
    if (node->left != NULL)
    {
      add_node(symbol_data, address, node->left);
      return;
    }
    node->left = init_symbol_node(symbol_data, address);
  }
  else if (cmp < 0)
  {
    if (node->right != NULL)
    {
      add_node(symbol_data, address, node->right);
      return;
    }
    node->right = init_symbol_node(symbol_data, address);
  }
  else
  {
    perror("Error! Same Label being defined twice!");
    exit(EXIT_FAILURE);
  }
}

/*!
 * 
 * @param symbol_data
 * @param node 
 * @return find the node which the key is equal to the symbol data
 */
int find_node(char *symbol_data, symbol_node_t *node)
{
  if (!node)
  {
    perror("Error! Undefined symbol! %s\n");
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
