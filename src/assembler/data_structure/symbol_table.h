#ifndef SYMBOL_TABLE
#define SYMBOL_TABLE

typedef struct symbol_node_t {
	char * symbol_data;
	int fake_address;
	symbol_node_t * left;
	symbol_node_t * right;
} symbol_node_t;

typedef struct symbol_table_t {
	symbol_node_t * head_holder;
} symbol_table_t;

typedef symbol_table_t *SymbolTable;

SymbolTable init_symbol_table(void);
void add_symbol_table(char * symbol_data, int fake_address, SymbolTable table);
int find_symbol_table(char * symbol_data, SymbolTable table);
void free_symbol_table(SymbolTable table);

#endif //SYMBOL_TABLE
