#include <stdio.h>
#include <stdint.h>
#include "instruction_processor.h"
#include "bit_operation.h"
#include "types_and_macros.h"

/*
 * Decode the instruction: 
 * First test condition, then find the type
 * of the instruction. 
 */
void decode(instruction_t* instruction)
{
    test_instruction_cond(instruction);
    find_type(instruction);
    return;
}

void test_instruction_cond(instruction_t* instruction)
{
    getcond(instruction);
    return;
}

int getcond(instruction_t* instruction)
{
    int cond;
    cond = (instruction->u.i >> 28) & 0x000000007;
    return cond;
}

void find_type(instruction_t* instruction)
{
    uint32_t n = instruction->u.i;
    if (get_k_bit(n, 27) == 1)
    {
        instruction->tag = BRANCH;
    } else if (get_k_bit(n, 26) == 1)
    {
        instruction->tag = TRANS;
    } else if (get_k_bit(n, 25) == 1)
    {
        instruction->tag = DATA_PROCESS; // the case with op2 an immediate value
    } else if (get_k_bit(n, 4) == 1 && get_k_bit(n, 7) == 1)
    {
        instruction->tag = MUL;
    } else 
    {
        instruction->tag = DATA_PROCESS;
    }
}