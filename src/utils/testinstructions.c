#include <stdio.h>
#include <stdint.h>
#include "testutils.h"
#include "instruction_processor.h"

int main(void)
{
    instruction_t ins = {.tag = 0, .u.i = 0x1234ABCD};
    testcond(ins.tag == UNDEFINED, "Tag(new insturction)==UNDEFINED");

    /* Use i to define the content of union */
    testint(ins.u.i, 0x1234abcd, "First byte == 0x12");
    testint(ins.u.bf.byte1, 0x12, "1st byte == 0x12");
    testint(ins.u.bf.byte2, 0x34, "2nd byte == 0x34");
    testint(ins.u.bf.byte3, 0xAB, "3rd byte == 0xAB");
    testint(ins.u.bf.byte4, 0xCD, "4th byte == 0xCD");

    /* Use bit-field to change u.i */
    ins.u.bf.byte1 = 0x40;
    testint(ins.u.i, 0x4034abcd, "1st byte == 0x40, other unchanged");
    return 0;
}
