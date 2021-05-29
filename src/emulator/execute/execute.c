#include <stdio.h>
#include <stdlib.h>
#include "../utils/types_and_macros.h"
#include "../utils/bit_operation.h"
#include "execute.h"

bool execute(instruction_t* decode, ArmState armstate)
{
    if (test_instruction_cond(decode, armstate))
    {
        switch ( decode->tag )
        {
        case DATA_PROCESS:
            execute_DP(decode, armstate);
            break;
        case MUL:
            execute_MUL(decode, armstate);
            break;
        case TRANS:
            execute_SDT(decode, armstate);
            break;
        case BRANCH:
            execute_BRANCH(decode, armstate);
            break;
        case ZERO:
            execute_ZERO(decode, armstate);
        case UNDEFINED:
        default:
            return EXIT_FAILURE;
        }
    }
}


void execute_MUL(instruction_t* decoded, ArmState armstate)
{
    uint32_t result;

    result = armstate->reg[decoded->u.mul.Rm] * armstate->reg[decoded->u.mul.Rs];
    
    // the accumulate bit is set
    if (decoded->u.mul.A)
    {
        result += armstate->reg[decoded->u.mul.Rn];
    }
    // Save the result
    armstate->reg[decoded->u.mul.Rd] = result;
    
    // If the S bit is set, we need to update the CPSR
    if (decoded->u.mul.S)
    {
        armstate->flagN = get_k_bit(result, 31));
        armstate->flagZ = (!result) ? 1 : 0;
    }
    
}

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