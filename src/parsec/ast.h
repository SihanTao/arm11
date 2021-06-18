#ifndef AST_PARSERC
#define AST_PARSERC

// macros to provide jQuery like experience
#define $G get_child_by_name
#define $T get_matched
#define $TG(ast, name) $T($G(ast, name))

typedef struct ast_node_t
{
  char *       key;
  char *       matched;
  struct ast_node_t* brother;
  struct ast_node_t* child;
} ast_node_t;

typedef ast_node_t* AST;

typedef AST (ast_mapper)(AST);

void free_ast(AST ast_node);
AST merge_into(AST parent, AST child);
AST make_atom(char * name, char * matched);
AST get_child_by_name(AST ast, char* name);
AST add_child(AST self, AST child);
AST merge(AST self, AST brother_parent);
AST add_brother(AST self, AST brother);
char * get_matched(AST ast);

void print_ast(AST ast, int indent);

#endif //AST_PARSERC


