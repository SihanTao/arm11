#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/unit_test.h"
#include "../utils/instruction_processor.h"

int main(void)
{
    add_test("test_intructions");
    {
        instruction_t ins = {.tag = 0, .u.i = 0x1234ABCD};
        test_true(ins.tag == UNDEFINED);

        /* Use i to define the content of union */
        test_int(ins.u.i, 0x1234abcd);
        test_int(ins.u.bf.byte1, 0x12);
        printf("1st byte == 0x12\n");
        test_int(ins.u.bf.byte2, 0x34);
        printf("2nd byte == 0x34\n");
        test_int(ins.u.bf.byte3, 0xAB);
        printf("3rd byte == 0xAB\n");
        test_int(ins.u.bf.byte4, 0xCD);
        printf("4th byte == 0xCD\n");

        /* Use bit-field to change u.i */
        ins.u.bf.byte1 = 0x40;
        test_int(ins.u.i, 0x4034abcd);
        printf("1st byte == 0x40, other unchanged");
    }
    end_test();
    return 0;
}
