#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/unit_test.h"
#include "../utils/instruction_processor.h"
#include "../utils/bit_operation.h"

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
    }
    end_test();

    add_test("test_getcond:");
    {
        instruction_t ins = {.tag = 0, .u.i = 0x1234ABCD}; // cond = 0001
        test_int_v(getcond(&ins), 0x1, "Cond == 0001");
    }
    end_test();
    return 0;
}
