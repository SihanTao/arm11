#include <stdio.h>
#include "../../global_utils/types_and_macros.h"

#include "../code_generate/code_generate_helper.h"

#include "../data_structure/token_stream.h"
#include "../../global_utils/unit_test.h"

int main(void)
{
  add_test("test generate_dp");
  {
    //mov r1, #56
    Token token1 = tokenize("mov r1, #56\n");
    uint32_t binary_code =  token_to_binary_code(token1);
    test_int_v(binary_code, 0xE3A00038, "mov r1, #56 = 0b1110 001 11010 0000 0001 0000 0011 1000");

    //add r2, r4, r3
    Token token2 = tokenize("add r2, r4, r3\n");
    uint32_t binary_code =  token_to_binary_code(token2);
    test_int_v(binary_code, 0xE2842003, "add r2, r4, r3 = 0b1110 001 01000 0100 0010 0000 0000 0011");

    //tst r0 r12
    Token token3 = tokenize("tst r0, r12\n");
    uint32_t binary_code =  token_to_binary_code(token3);
    test_int_v(binary_code, 0xE3A00038, "tst r0, r12 = 0b1110 001 10001 0000 0000 0000 0000 1100");

    //mov r1, #0x0F
    Token token4 = tokenize("mov r1, #0x0F\n");
    uint32_t binary_code =  token_to_binary_code(token4);
    test_int_v(binary_code, 0xE3A0100F, "mov r1, #0x0F = 0b1110 001 11010 0000 0001 0000 0000 1111");
  }

  add_test("test generate_mul");
  {
    //mla r1, r2, r3, r4
    Token token1 = tokenize("mla r1, r2, r3, r4\n");
    uint32_t binary_code =  token_to_binary_code(token1);
    test_int_v(binary_code, 0xE0214392, "mla r1, r2, r3, r4 = 0b1110 000000 10 0001 0100 0011 1001 0010");

    //mul r1, r2, r3
    Token token2 = tokenize("mul r1, r2, r3\n");
    uint32_t binary_code =  token_to_binary_code(token2);
    test_int_v(binary_code, 0xE0010392, "mul r1, r2, r3 = 0b1110 000000 00 0001 0000 0011 1001 0010");
  }

  add_test("test generate_trans");
  {
    //ldr r1, =0x0F, treat as mov r1, #0x0F
    Token token1 = tokenize("ldr r1, =0x0F\n");
    uint32_t binary_code =  token_to_binary_code(token1);
    test_int_v(binary_code, 0xE3A0100F, "ldr r1, =0x0F = 0b1110 001 11010 0000 0001 0000 0000 1111");

    // //ldr r0, =0x555555
    // Token token2 = tokenize("ldr r0, =0x555555\n");
    // uint32_t binary_code =  token_to_binary_code(token2);
    // test_int_v(binary_code, answer, "ldr r0, =0x555555 = 0b1110 01 111 001 1111 0000 offset");

    //str r0, [r3, #56]
    Token token3 = tokenize("str r0, [r3, #56]\n");
    uint32_t binary_code =  token_to_binary_code(token3);
    test_int_v(binary_code, 0xE7830038, "str r0, [r3, #56] = 0b1110 01 111 000 0011 0000 0000 0011 1000");

    //str r0, [r3], #56
    Token token4 = tokenize("str r0, [r3], #56\n");
    uint32_t binary_code =  token_to_binary_code(token4);
    test_int_v(binary_code, 0xE6830038, "str r0, [r3], #56 = 0b1110 01 101 000 0011 0000 0000 0011 1000");
  }

  end_all_tests();
  return 0;
}