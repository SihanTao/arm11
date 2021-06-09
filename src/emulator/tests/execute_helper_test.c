#include <stdio.h>

#include "../utils/types_and_macros.h"
#include "../execute/execute_helper.h"
#include "../utils/unit_test.h"

int main(void)
{
  add_test("Test for rotate");
  {
    size_t size = sizeof(value_carry_t);
    value_carry_t expect = {
        .value = 0x80000005,
        .carry = 1};
    value_carry_t got = rotate(0x0000000B, 1);
    test_eq_v(&got, &expect, size, "1011 becomes 1000..0101");

    expect.value = 0x40000006;
    expect.carry = 1;
    got = rotate(0x0000001A, 2);

    test_eq_v(&got, &expect, size, "11010 becomes 0100..0110");

    expect.value = 0xA000000B;
    expect.carry = 1;
    got = rotate(0x0000005D, 3);
    printf("got.value :>> %d\n,", got.value); //DELETE_MARK

    test_eq_v(&got, &expect, size, "1011101 becomes 1010..1011");
  }

  end_all_tests();

  // add_test("Test for shift");
  // {
  //   test_int_v(shift(0x0000000B, 1, LSL), 0x00000016, "1011 becomes 10110");

  //   test_int_v(shift(0x0000001A, 2, LSR), 0x00000006, "11010 becomes 110");

  //   test_int_v(shift(0xA000000B, 3, ASR), 0xF4000001, "1010..1011 becomes 1111010..0001");

  //   test_int_v(shift(0x0000001A, 1, ASR), 0x0000000D, "..11010 becomes ..1101");

  //   test_int_v(shift(0x80000005, 2, ASR), 0xE0000001, "1000..0101 becomes 1110..0001");

  //   test_int_v(shift(0x0000005D, 3, ROR), 0xA000000B, "1011101 becomes 1010..1011");
  // }

  // add_test("Test for reg_imm_handle");
  // {
  //   bitfield reg_imm_bf[1];
  //   reg_or_imm_t imm1 =
  //       {.rot_imm.imm = 0x0000000B,
  //        .rot_imm.amount = 1};

  //   test_int_v(reg_imm_handle(reg_imm_bf, imm1, 0), 0xC0000002, "1011 becomes 1100..0010");

  //   bitfield reg_imm_bf[1];
  //   reg_or_imm_t imm2 =
  //       {.rot_imm.imm = 0x0000001A,
  //        .rot_imm.amount = 2};

  //   test_int_v(reg_imm_handle(reg_imm_bf, imm2, 0), 0xA0000001, "11010 becomes 1010..0001");

  //   reg_or_imm_t reg1 =
  //       {.shift_reg = {
  //            .Rm = 1,
  //            0,
  //            .type = LSL,
  //            .val = 1}};

  //   bitfield reg_imm_bf[1] = to_bf(0x0000000B); //Rm
  //   test_int_v(reg_imm_handle(reg_imm_bf, reg1, 1), 0x00000016, "1011 becomes 10110");

  //   reg_or_imm_t reg2 =
  //       {.shift_reg = {
  //            .Rm = 1,
  //            0,
  //            .type = LSR,
  //            .val = 2}};

  //   bitfield reg_imm_bf[1] = to_bf(0x0000001A); //Rm
  //   test_int_v(reg_imm_handle(reg_imm_bf, reg2, 1), 0x00000006, "11010 becomes 110");

  //   reg_or_imm_t reg3 =
  //       {.shift_reg = {
  //            .Rm = 1,
  //            0,
  //            .type = ASR,
  //            .val = 3}};

  //   bitfield reg_imm_bf[1] = to_bf(0xA000000B); //Rm
  //   test_int_v(reg_imm_handle(reg_imm_bf, reg3, 1), 0xF4000001, "1010..1011 becomes 1111010..0001");

  //   reg_or_imm_t reg4 =
  //       {.shift_reg = {
  //            .Rm = 1,
  //            0,
  //            .type = ROR,
  //            .val = 3}};

  //   bitfield reg_imm_bf[1] = to_bf(0x0000005D); //Rm
  //   test_int_v(reg_imm_handle(reg_imm_bf, reg4, 1), 0xA000000B, "1011101 becomes 1010..1011");
  // }
}