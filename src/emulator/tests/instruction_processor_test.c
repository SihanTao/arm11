#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/unit_test.h"
#include "../utils/instruction_processor.h"
#include "../utils/tools.h"

int main(void)
{
    add_test("test_intructions");
    {
        instruction_t ins = {.tag = 0, .u.i = 0x1234ABCD};
        test_true(ins.tag == UNDEFINED);

        /* Use i to define the content of union */
        test_int(ins.u.i, 0x1234abcd);
        test_int_v(ins.u.bf.byte1, 0x12, "1st byte == 0x12");
        test_int_v(ins.u.bf.byte2, 0x34, "2nd byte == 0x34");
        test_int_v(ins.u.bf.byte3, 0xAB, "3rd byte == 0xAB");
        test_int_v(ins.u.bf.byte4, 0xCD, "4th byte == 0xCD");

        /* Use bit-field to change u.i */
        ins.u.bf.byte1 = 0x40;
        test_int_v(ins.u.i, 0x4034abcd, "1st byte == 0x40, other unchanged");

        ins.tag = 0;
        ins.u.i = 0b11100011101000000000000000000001; // This is the example for: mov r0, #1
        test_int_v(ins.u.bf.byte1, 0b11100011, "1st byte == 0x12");
        test_int_v(ins.u.bf.byte2, 0b10100000, "2nd byte == 0x34");
        test_int_v(ins.u.bf.byte3, 0b00000000, "3rd byte == 0xAB");
        test_int_v(ins.u.bf.byte4, 0b00000001, "4th byte == 0xCD");

    }

    add_test("test_getcond:");
    {
        instruction_t ins = {.tag = 0, .u.i = 0x1234ABCD}; // cond = 0001
        test_int_v(getcond(&ins), 0x1, "Cond == 0001");
    }

    add_test("test data process in union");
    {
        instruction_t ins = {
            .tag = 0,
            .u.i = 0b11100011101000000000000000000001 // This is the example for: mov r0, #1
        };
        test_int_v(ins.u.data_process.cond, 0b1110, "Cond == 1110");
        test_int_v(ins.u.data_process.I, 0b1, "Cond == 1110");
        test_int_v(ins.u.data_process.OpCode, 0b1101, "Cond == 1110");
        test_int_v(ins.u.data_process.S, 0b0, "Cond == 1110");
        test_int_v(ins.u.data_process.Rn, 0b0, "Cond == 1110");
        test_int_v(ins.u.data_process.Rd, 0b0000, "Cond == 1110");
        test_int_v(ins.u.data_process.operand2, 0b1, "Cond == 1110");
    }

    add_test("test mul in union");
    {

    }

    add_test("test trans in union");
    {

    }

    add_test("test branch in union");
    {

    }

    add_test("Find the type of instructions:");
    {
        /* The example from P5 in the spec:
         * mov r1, #1       data processing
         * b foo            branch
         * mov r2, #2       data processing
         * foo: mov r3, #3  data processing
         */
        instruction_t instructions[] = {
            {0, {0xe3a01001}}, {0, {0xea000000}},
            {0, {0xe3a02002}}, {0, {0xe3a03003}}
        };

        instruction_t *p = instructions;
        find_type(p);
	    print_bit(p->u.i);
        test_int_v(p->tag, DATA_PROCESS, "mov r1, #1 == data processing");

        find_type(++p);
	    print_bit(p->u.i);
        test_int_v(p->tag, BRANCH, "b foo == branch");

        find_type(++p);
	    print_bit(p->u.i);
        test_int_v(p->tag, DATA_PROCESS, "mov r2, #2 == data processing");

        find_type(p);
	    print_bit(p->u.i);
        test_int_v(p->tag, DATA_PROCESS, "foo: mov r3, #3 == data processing");

        /* add01
         * mov r1,#1
         * add r2,r1,#2
         */
        instruction_t add01[] = {{0, {0xe3a01001}}, {0, {0xe2812002}}};
        find_type(&add01[0]);
	    print_bit(add01[0].u.i);
        test_int_v(add01[0].tag, DATA_PROCESS, "mov r1,#1 == DATA_PROCESS");

        find_type(&add01[1]);
	    print_bit(add01[1].u.i);
        test_int_v(add01[1].tag, DATA_PROCESS, "add r2,r1,#2 == DATA_PROCESS");

        instruction_t mul = {0, {0xe0020091}}; //9100 02e0 mul r2,r1,r0
        find_type(&mul);
	    print_bit(mul.u.i);
        test_int_v(mul.tag, MUL, "mul r2,r1,r0 == multiply");

        instruction_t bne = {0, {0x1afffffa}};// faff ff1a bne loop
        find_type(&bne);
	    print_bit(bne.u.i);
        test_int_v(bne.tag, BRANCH, "bne == BRANCH");

        /*
         * ldr r0,=0x02 0200 a0e3
         * ldr r2,[r0]  0020 90e5
         * cmp r2,r0    0000 52e1
         */
        instruction_t ldr01[] = {
            {0, {0xe3a00002}}, {0, {0xe5902000}}, {0, {0xe1520000}}
        };

        p = ldr01;
        find_type(p);
	    print_bit(p->u.i);
        /* Note here the instruction ldr r0,=0x02 works as mov r0, =0x42
         * I realise it after reading the example on page 15.
         * This confused me a lot when testing
         */
        test_int_v(p->tag, DATA_PROCESS, "ldr r0,=0x02 == DATA_PROCESS");
        p++;
        find_type(p);
	    print_bit(p->u.i);
        test_int_v(p->tag, TRANS, "ldr r2,[r0] == TRANS");
        p++;
        find_type(p);
	    print_bit(p->u.i);
        test_int_v(p->tag, DATA_PROCESS, "cmp r2,r0 == DATA_PROCESS");
    }

    end_all_tests();
    return 0;
}
