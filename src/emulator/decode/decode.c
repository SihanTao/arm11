#include <stdlib.h>
#include "../utils/types_and_macros.h"
#include "../utils/tools.h"

instruction_t decode (bitfield fetched)
{
    instruction_t decode = init_instruction(fetched);
    find_type(&decode);
    return decode;
}

void find_type(instruction_t* instruction)
{
    uint32_t n = instruction->u.i;
    if (get_k_bit(n, 27))
    {
        instruction->tag = BRANCH;
    } else if (get_k_bit(n, 26))
    {
        instruction->tag = TRANS;
    } else if (get_k_bit(n, 25))
    {
        instruction->tag = DATA_PROCESS; // the case with op2 an immediate value
    } else if (get_k_bit(n, 4) && get_k_bit(n, 7))
    {
        instruction->tag = MUL;
    } else
    {
        instruction->tag = DATA_PROCESS;
    }
}