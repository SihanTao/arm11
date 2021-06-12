#include "../../global_utils/types_and_macros.h"

#include "reverse_rotate.h"

#include "../../global_utils/unit_test.h"

int main(void)
{
  add_test("test helepr");
  {
     uint32_t target = 0xF0000000;
     int *rotation_amount = (int *) malloc(sizeof(int));
     uint32_t *imm = (uint32_t *) malloc(sizeof(uint32_t));
     bool valid = reverse_rotate(target, rotation_amount, imm);
     test_int_v(*rotation_amount, 2, "result is 2");
     test_int_v(*imm, 0x0000000F, "result is 0x0000000F");
     test_true(valid);
  }

  end_all_tests();
}