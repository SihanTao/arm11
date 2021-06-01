#include <stdlib.h>
#include "../utils/types_and_macros.h"
#include "../utils/tools.h"
#include "decode.h"

instruction_t decode (bitfield fetched)
{
    instruction_t decode = init_instruction(fetched);
    find_type(&decode);
    return decode;
}

void find_type(instruction_t* instruction)
{
    uint32_t n = to_int(instruction->u.bf);
    if (get_bit(n, 27))
    {
        instruction->tag = BRANCH;
    } else if (get_bit(n, 26))
    {
        instruction->tag = TRANS;
    } else if (get_bit(n, 25))
    {
        instruction->tag = DATA_PROCESS; // the case with op2 an immediate value
    } else if (get_bit(n, 4) && get_bit(n, 7))
    {
        instruction->tag = MUL;
    } else
    {
        instruction->tag = DATA_PROCESS;
    }
}
