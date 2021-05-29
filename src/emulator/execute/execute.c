#include <stdio.h>
#include <stdlib.h>
#include "../utils/tools.h"
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

void execute_DP(instruction_t* decode, ArmState armstate)
{
    uint32_t result;

    uint32_t Rn = bitfield_to_uint32(armstate->reg[decode->u.data_process.Rn]);
    uint32_t operand2 = bitfield_to_uint32(armstate->reg[decode->u.data_process.operand2]);


    // If the I bit is set, then Operand2 is an immediate constant, otherwise it is a shifted register.
    if (decode->u.data_process.I)
    {

    } else {

    }

    // Type of Opcode
    if (decode->u.data_process.OpCode)
    {
        switch (decode->u.data_process.OpCode)
        {
            case AND: result = (Rn && operand2);
            case EOR: result = (Rn ^ operand2);
            case SUB: result = (Rn - operand2);
            case RSB: result = (operand2 - Rn);
            case ADD: result = (Rn + operand2);
            case TST: (Rn && operand2);
            case TEQ: (Rn ^ operand2);
            case CMP: (Rn - operand2);
            case ORR: result = (Rn || operand2);
            case MOV: result = (operand2);
            default:
                break;
        }
    }
    
    // If the S bit is set, we need to update the CPSR
    if (decode->u.data_process.S)
    {
        switch (decode->u.data_process.OpCode)
        {
            
        }
        armstate->flagZ = (!result) ? 1 : 0;
        armstate->flagN = get_k_bit(result, 31);
    }
}

void execute_MUL(instruction_t* decode, ArmState armstate)
{
    uint32_t result;

    uint32_t Rm = bitfield_to_uint32(armstate->reg[decode->u.mul.Rm]);
    uint32_t Rs = bitfield_to_uint32(armstate->reg[decode->u.mul.Rs]);
    result = Rm * Rs;
    
    // the accumulate bit is set
    if (decode->u.mul.A)
    {
        uint32_t Rn = bitfield_to_uint32(armstate->reg[decode->u.mul.Rn]);
        result += Rn;
    }
    // Save the result
    armstate->reg[decode->u.mul.Rd] = uint32_to_bitfield(result);
    
    // If the S bit is set, we need to update the CPSR
    if (decode->u.mul.S)
    {
        armstate->flagN = get_k_bit(result, 31);
        armstate->flagZ = (!result) ? 1 : 0;
    }
}

void execute_BRANCH(instruction_t* decode, ArmState armstate)
{
    
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