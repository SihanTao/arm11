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

void execute_DP(instruction_t* decoded, ArmState armstate)
{
    uint32_t result;

    //if I is set, it means that OP2 is an immediate value.
    if (decoded->u.data_process.I)
    {
        int rotation_amount = 2 * get_k_bit(decoded->u.data_process.operand2, 4);
        uint32_t extented = get_k_bit(decoded->u.data_process.operand2, 8);
        //then do the rotation.
    } else //OP2 is a register.
      {
          int shift = get_k_bit(decoded->u.data_process.operand2, 4);
          uint32_t rm = get_k_bit(decoded->u.data_process.operand2, 4);
          
          switch (decoded->u.data_process.operand2)//take the shift type.
          {
              case 00:
              {
                  decoded->u.data_process.operand2 = rm >> shift;
                  break;
              }
              case 01:
              {
                  decoded->u.data_process.operand2 = rm << shift;
                  break;
              }
              case 10:
              {
                  //two's complement opration.
              }
              case 11:
              {
                  //rotate.
              }
          }
      }
    
    //compute the result
    uint32_t Rn = bitfield_to_uint32(armstate->reg[decoded->u.data_process.Rn]);
    uint32_t operand2 = bitfield_to_uint32(armstate->reg[decoded->u.data_process.operand2]);
    if (decoded->u.data_process.OpCode)
    {
        switch (decoded->u.data_process.OpCode)
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
        }
    }

    //save the result.
    armstate->reg[decoded->u.data_process.Rd] = uint32_to_bitfield(result);
    
    //if S is set then CPSR should be update.
    if (decoded->u.data_process.S)
    {
        armstate->flagN = get_k_bit(result, 31);
        if (result == 0) {armstate->flagZ = 0;}
        //changes with FlagC.
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