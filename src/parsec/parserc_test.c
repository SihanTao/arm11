#include <stdlib.h>
#include <ctype.h>

#include "ast.h"
#include "char_stream.h"

#include "parsec.h"

int main(void)
{
  char *     string;
  CharStream char_stream;
  AST        ast;
  Parsec     parsec;

  print_ast(ast, 0);
  printf("\n");

  Parsec abc = match("first", "abc");
  Parsec cde = match("second", "cde");
  Parsec ghf = match("third", "ghf");

  Parsec and1 = make_and("and1", abc, cde);
  Parsec and2 = make_and("and2", ghf, and1);

  print_pc(and2, 0);
  printf("\n");

  char * string      = "ghfabccde";
  char **char_stream = &string;
  AST    ast         = perform_parse(char_stream, and2, NULL);
  print_ast(ast, 0);
  printf("\n");

  string          = "ghfabccde";
  char_stream     = &string;
  Parsec asdf     = match("asdfasdf", "asdf");
  Parsec make_or1 = make_or("make_or", asdf, and2);
  ast             = perform_parse(char_stream, make_or1, NULL);
  print_ast(ast, 0);
  printf("\n");

  string          = "ghfabccde";
  char_stream     = &string;
  Parsec make_or2 = make_or("make_or", and2, asdf);
  ast             = perform_parse(char_stream, make_or2, NULL);

  print_ast(ast, 0);
  printf("\n");

  string      = "asdf";
  char_stream = &string;
  ast         = perform_parse(char_stream, make_or2, NULL);

  print_ast(ast, 0);
  printf("\n");

  Parsec and_components[3]
      = { match("one", "one"), match("two", "two"), match("three", "three") };

  Parsec my_and = seq("my add", and_components, 3);
  print_pc(my_and, 0);
  printf("\n");
}
