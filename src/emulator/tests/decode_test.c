#include "../../global_utils/types_and_macros.h"

#include "../decode/decode.h"

#include "../../global_utils/tools.h"
#include "../utils/unit_test.h"

int main(void)
{
  add_test("Find the type of instructions:");
  {
    /* The example from P5 in the spec:
     * mov r1, #1       data processing
     * b foo            branch
     * mov r2, #2       data processing
     * foo: mov r3, #3  data processing
     */
    instruction_t instructions[] = { { 0, { 0xe3a01001 } },
                                     { 0, { 0xea000000 } },
                                     { 0, { 0xe3a02002 } },
                                     { 0, { 0xe3a03003 } } };

    instruction_t *p = instructions;
    find_type(p);
    //print_bit(p->word.i);
    test_int_v(p->tag, DATA_PROCESS, "mov r1, #1 == data processing");
    test_int_v(p->word.proc.cond, AL, "mov r1, #1 cond == always");
    test_int_v(p->word.proc.operand2.rot_imm.imm, 1,
               "mov r1, #1 oprand2.imm = 1");
    test_int_v(p->word.proc.operand2.rot_imm.amount, 0, "rotate amount = 0");
    test_int_v(p->word.proc.Rd, 1, "mov r1, #1, Rn == 1");

    find_type(++p);
    //print_bit(p->word.i);
    test_int_v(p->tag, BRANCH, "b foo == branch");

    find_type(++p);
    //print_bit(p->word.i);
    test_int_v(p->tag, DATA_PROCESS, "mov r2, #2 == data processing");

    find_type(p);
    //print_bit(p->word.i);
    test_int_v(p->tag, DATA_PROCESS, "foo: mov r3, #3 == data processing");

    /* add01
     * mov r1,#1
     * add r2,r1,#2
     */
    instruction_t add01[] = { { 0, { 0xe3a01001 } }, { 0, { 0xe2812002 } } };
    find_type(&add01[0]);
    //print_bit(add01[0].word.i);
    test_int_v(add01[0].tag, DATA_PROCESS, "mov r1,#1 == DATA_PROCESS");

    find_type(&add01[1]);
    //print_bit(add01[1].word.i);
    test_int_v(add01[1].tag, DATA_PROCESS, "add r2,r1,#2 == DATA_PROCESS");

    instruction_t mul = { 0, { 0xe0020091 } }; // 9100 02e0 mul r2,r1,r0
    find_type(&mul);
    //print_bit(mul.word.i);
    test_int_v(mul.tag, MUL, "mul r2,r1,r0 == multiply");

    instruction_t bne = { 0, { 0x1afffffa } }; // faff ff1a bne loop
    find_type(&bne);
    //print_bit(bne.word.i);
    test_int_v(bne.tag, BRANCH, "bne == BRANCH");

    /*
     * ldr r0,=0x02 0200 a0e3
     * ldr r2,[r0]  0020 90e5
     * cmp r2,r0    0000 52e1
     */
    instruction_t ldr01[] = { { 0, { 0xe3a00002 } },
                              { 0, { 0xe5902000 } },
                              { 0, { 0xe1520000 } } };

    p = ldr01;
    find_type(p);
    //print_bit(p->word.i);
    /* Note here the instruction ldr r0,=0x02 works as mov r0, =0x42
     * I realise it after reading the example on page 15.
     * This confused me a lot when testing
     */
    test_int_v(p->tag, DATA_PROCESS, "ldr r0,=0x02 == DATA_PROCESS");
    p++;
    find_type(p);
    //print_bit(p->word.i);
    test_int_v(p->tag, TRANS, "ldr r2,[r0] == TRANS");
    p++;
    find_type(p);
    //print_bit(p->word.i);
    test_int_v(p->tag, DATA_PROCESS, "cmp r2,r0 == DATA_PROCESS");
  }

  add_test("Test decode:");
  {
    instruction_t instructions[] = { { 0, { 0xe3a01001 } },
                                     { 0, { 0xea000000 } },
                                     { 0, { 0xe3a02002 } },
                                     { 0, { 0xe3a03003 } } };
    // instruction_t add01[] = {{0, {0xe3a01001}}, {0, {0xe2812002}}};
    // instruction_t mul = {0, {0xe0020091}}; //9100 02e0 mul r2,r1,r0
    // instruction_t bne = {0, {0x1afffffa}};// faff ff1a bne loop
    // instruction_t ldr01[] = {
    //     {0, {0xe3a00002}}, {0, {0xe5902000}}, {0, {0xe1520000}}
    // };

    bitfield instructions_bf[4];
    instruction_t res[4];
    for (int i = 0; i < 4; i++)
    {
      instructions_bf[i] = instructions[i];
      res[i]             = decode(instructions_bf[i]);
    }

    test_int_v(res[0].tag, DATA_PROCESS, "res[0]");
    test_int_v(res[1].tag, BRANCH, "res[0]");
    test_int_v(res[2].tag, DATA_PROCESS, "res[0]");
    test_int_v(res[3].tag, DATA_PROCESS, "res[0]");
  }

  end_all_tests();

  return 0;
}
