#ifndef SYMBOL_TABLE
#define SYMBOL_TABLE

typedef struct symbol_node_t
{
  char *                symbol_data;
  int                   address_to_next_ins;
  struct symbol_node_t *left;
  struct symbol_node_t *right;
} symbol_node_t;

typedef struct symbol_table_t
{
  symbol_node_t *head_holder;
} symbol_table_t;

/*
 * A SymbolTable is a binary tree map, which uses `char * symbol_data` as its
 * key, and `int address_to_to_next_ins` as its value.
 * The reason to use binary tree as Symbol table is because its simple to
 * implement and more efficient than a linked list.
 */
typedef symbol_table_t *SymbolTable;

SymbolTable init_symbol_table(void);
void        add_symbol_table(char *symbol_data, int address_to_next_ins,
                             SymbolTable table);
int         find_symbol_table(char *symbol_data, SymbolTable table);
void        free_symbol_table(SymbolTable table);

#endif // SYMBOL_TABLE
