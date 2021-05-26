#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/instruction_processor.h"
#include "../utils/bit_operation.h"

int main(void)
{
    instruction_t ins = {.tag = 0, .u.i = 0x1234ABCD};
    print_bit(ins.u.i);
    printf("%d\n", ins.u.bf.byte1);
    printf("%d\n", ins.u.bf.byte2);
    printf("%d\n", ins.u.i);
    printf("%ld, %ld\n", sizeof(ins.u.i), sizeof(ins.u.bf));
}