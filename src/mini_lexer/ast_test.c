#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ast.h"

int main(void)
{
  AST_Holder holder = init_parserc_ast(NULL);
  // build_parserc_ast("first node", holder, "matched1", SIBLING);
  // build_parserc_ast("second node", holder, "matched2", SIBLING);
  // build_parserc_ast("third node", holder, "mathed3", SIBLING);
  AST first = make_atom("first node", "matched1");
  AST second = make_atom("second node", "matched2");
  AST third = make_atom("third node", "matched3");

  AST parent1 = make_atom("parent1", NULL);
  AST forth = make_atom("forth node", "matched4");
  AST parent2 = make_atom("parent2", NULL);

  merge_into(parent1, first);
  merge_into(parent1, second);
  merge_into(parent1, third);
  merge_into(parent2, parent1);
  merge_into(parent2, forth);

  print_ast(parent2,0);
    printf("\n");

  AST parent1_ = get_child_by_name(parent2, "parent1");

  print_ast(parent1, 0);
    printf("\n");

  AST second_ = get_child_by_name(parent1_, "second node");

  print_ast(second_, 0);
    printf("\n");


  printf("%s \n", get_matched(second_));

  // build_parserc_ast("parent node", holder, NULL, PARENT);

  // AST ast = holder->root_holder;

  // print_ast(ast, 0);

  // AST second = get_child_by_name(ast, "second");
  // print_ast(second, 0);
  // // prints nothing since no child called second

  // AST first = get_child_by_name(ast, "first node");
  // print_ast(first, 0);

  // printf("\n");

  // build_parserc_ast("parent node2", holder, NULL, SIBLING);
  // build_parserc_ast("parent node3", holder, NULL, SIBLING);

  // print_ast(holder->root_holder, 0);

}