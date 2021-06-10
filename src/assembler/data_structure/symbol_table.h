#ifndef SYMBOL_TABLE
#define SYMBOL_TABLE

typedef struct symbol_node_t {
	int symbol_id;
	int fake_address;
	symbol_node_t * next;
	symbol_node_t * prev;
} symbol_node_t;

typedef struct symbol_table_t {
	symbol_node_t * head_holder;
	symbol_node_t * tail_holder;
} symbol_table_t;

typedef symbol_table_t *SymbolTable;

#endif //SYMBOL_TABLE
