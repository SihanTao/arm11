#ifndef AST_PARSERC
#define AST_PARSERC

#define $G get_child_by_name
#define $T get_matched
#define $TG(x, y) $T($G(x, y))

typedef struct ast_node_t
{
  char *       key;
  char *       matched;
  struct ast_node_t* brother;
  struct ast_node_t* child;
} ast_node_t;

typedef ast_node_t* AST;

typedef AST (ast_mapper)(AST);

typedef struct ast_holder_t
{
  AST root_holder;
  ast_mapper* map_while_build;
} ast_holder_t;

typedef ast_holder_t *AST_Holder;


AST merge_into(AST parent, AST child);

AST make_atom(char * name, char * matched);

AST add_child(AST self, AST child);

AST merge(AST self, AST brother_parent);

AST add_brother(AST self, AST brother);

ast_node_t* get_child_by_name(ast_node_t* ast, char* name);

ast_node_t* get_brother_by_name(ast_node_t* ast, char * name);

void free_ast(ast_node_t* ast_node);

char * get_matched(ast_node_t* ast);

void print_ast(AST ast, int indent);

#endif //AST_PARSERC


