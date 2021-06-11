#include <stdio.h>
#include <string.h>
#include "symbol_table.h"

#include "../../global_utils/unit_test.h"

int main(void)
{
  add_test("test_symbol_table");
  {
    SymbolTable symbol_table = init_symbol_table();

    add_symbol_table(strdup("d"), 1, symbol_table);
    add_symbol_table(strdup("b"), 2, symbol_table);
    add_symbol_table(strdup("a"), 3, symbol_table);
    add_symbol_table(strdup("e"), 4, symbol_table);

    test_int(find_symbol_table("a", symbol_table), 3);
    test_int(find_symbol_table("b", symbol_table), 2);
    test_int(find_symbol_table("d", symbol_table), 1);
    test_int(find_symbol_table("e", symbol_table), 4);

    free_symbol_table(symbol_table);
  }

  end_all_tests();
  return 0;
}