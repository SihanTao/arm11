#include <stdlib.h>
#include <ctype.h>

#include "ast.h"
#include "char_stream.h"

#include "parserc.h"

int main(void)
{
  Parserc my_parser;
  my_parser = match("hello", "hi");
  // print_pc(my_parser, 0);
  // print_pc(optional("optional", my_parser), 0);
  Parserc componets[3]
      = { match("a", "asdf"), opt("opt", match("b", "asdsdf")),
          match("c", "asdfasdf") };
  Parserc ands = { seq("seq", componets, 2) };
  Parserc ors = {ands, take_until("until", isalnum)};
  print_pc(ors, 0);
  // print_pc(ands, 0);
}