#include <stdio.h>

#include "../utils/types_and_macros.h"

#include "../execute/execute_helper.h"

#include "../utils/unit_test.h"

int main(void)
{
  add_test("Test for rotate");
  {
    value_carry_t got = rotate(0x0000000B, 1);
    test_int_v(got.value, 0x80000005, "1011 becomes 1000..0101");
    test_true_v(got.carry, "carry = 1");

    got = rotate(0x0000001A, 2);
    test_int_v(got.value, 0x80000006, "11010 becomes 1000..0110");
    test_true_v(got.carry, "carry = 1");

    got = rotate(0x0000005D, 3);
    test_int_v(got.value, 0xA000000B, "1011101 becomes 1010..1011");
    test_true_v(got.carry, "carry = 1");
  }

  add_test("Test for shift");
  {
    value_carry_t got = shift(0x0000000B, 1, LSL);
    test_int_v(got.value, 0x00000016, "1011 becomes 10110");
    test_false(got.carry);

    got = shift(0x0000001A, 2, LSR);
    test_int_v(got.value, 0x00000006, "11010 becomes 110");
    test_true(got.carry);

    got = shift(0xA000000B, 3, ASR);
    test_int_v(got.value, 0xF4000001, "1010..1011 becomes 1111010..0001");
    test_false(got.carry);

    got = shift(0x0000001A, 1, ASR);
    test_int_v(got.value, 0x0000000D, "..11010 becomes ..1101");
    test_false(got.carry);

    got = shift(0x80000005, 2, ASR);
    test_int_v(got.value, 0xE0000001, "1000..0101 becomes 1110..0001");
    test_false(got.carry);

    got = shift(0x0000005D, 3, ROR);
    test_int_v(got.value, 0xA000000B, "1011101 becomes 1010..1011");
    test_true(got.carry);
  }

  end_all_tests();
}
