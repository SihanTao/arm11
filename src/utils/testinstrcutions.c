#include <stdio.h>
#include <stdint.h>
#include "./utils/testutils.h"
#include "instruction_processor.h"

int main(void)
{
    instruction_t ins = {0, 0xFFFF};
    testcond(ins.tag == UNDEFINED, "Tag(new insturction)==UNDEFINED");

    testint(ins.u.bf.byte1, 15, "First byte == 15");

    return 0;
}
