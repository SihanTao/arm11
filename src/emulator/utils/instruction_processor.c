#include <stdio.h>
#include <stdint.h>
#include "instruction_processor.h"
#include "bit_operation.h"

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
    return;
}