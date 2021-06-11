#include <stdbool.h>

#include "types_and_macros.h"

#include "tools.h"

#include "unit_test.h"

int main(void)
{
  add_test("test get_bit_range");
  {
    int target = 0b011101001;
    int start = 0;
    int end = 3;
    test_int_v(get_bit_range(target,start,end), 0b1001, "result is 0b1001");
  }

  add_test("test set_bit");
  {
    int *dest = (int *) malloc(sizeof(int));
    *dest = 0b1001;
    bool value = 0;
    int position = 3;
    set_bit(dest, value, position);
    test_int_v(*dest, 0b0001, "result is 0b1000");
    free(dest);
  }

  add_test("test set_bit_range");
  {
    int *dest = (int *) malloc(sizeof(int));
    *dest = 0b011101001;
    int src = 0b0000;
    int start = 0;
    int end = 3;
    set_bit_range(dest, src, start, end);
    test_int_v(*dest, 0b011100000, "result is 0b011100000");
    free(dest);
  }

  end_all_tests();
}