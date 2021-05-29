#include <stdio.h>

instruction_t decode (bitfield fetched)
{
    return NULL;
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