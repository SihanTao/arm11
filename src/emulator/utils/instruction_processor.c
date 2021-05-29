#include <stdio.h>
#include "../utils/types_and_macros.h"
#include "instruction_processor.h"
#include "bit_operation.h"

/*
 * Decode the instruction:
 * First test condition, then find the type
 * of the instruction.
 */
// void decode(instruction_t* instruction)
// {
//     test_instruction_cond(instruction);
//     find_type(instruction);
//     return;
// }

bool test_instruction_cond(instruction_t* instruction, ArmState armstate)
{
    bool N = armstate->flagN;
    bool Z = armstate->flagZ;
    bool C = armstate->flagC;
    bool V = armstate->flagV;
    switch (instruction->u.data_process.cond) // cond is at the same position in all cases
    {
    case EQ: return (N);
    case NE: return (!N);
    case GE: return (N == V);
    case LT: return (N != V);
    case GT: return (!N && N == V);
    case LE: return (N || N != V);
    case AL: return true;
    default:
        break;
    }
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