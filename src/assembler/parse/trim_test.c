#include <stdbool.h>

#include "tokenize.h"

#include "../global_utils/unit_test.h"

int main(void)
{
  add_test("trim test");
  {
    char ** cur_pos = "this is,a,string";
    char * dst;
    test_true(trim(cur_pos, ' ', dst));
    test_true(strcmp(dst, "this"));
    free(dst);
    test_true(trim(cur_pos, ',', dst));
    test_true(strcmp(dst, "is"));
    free()
  }
  end_all_tests();
}