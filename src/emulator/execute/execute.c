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

void execute_DP(instruction_t* decode, ArmState armstate)
{
    uint32_t result;

    //if I is set, it means that OP2 is an immediate value.
    if (decode->u.data_process.I)
    {
        
        int rotation_amount = 2 * decode->u.data_process.operand2.Iv.Rotate;
        uint32_t extented_Imm = decode->u.data_process.operand2.Iv.Imm;
        //then do the rotation.
    
    } else //OP2 is a register.
      {
          int shift = decode->u.data_process.operand2.Register.Shift.Integer;
          uint32_t rm = decode->u.data_process.operand2.Register.Rm;
          
          switch (decode->u.data_process.operand2.Register.Shift.ShiftT)
          {
              case 00:
              {
                  decode->u.data_process.operand2.op2 = rm >> shift;
                  break;
              }
              case 01:
              {
                  decode->u.data_process.operand2.op2 = rm << shift;
                  break;
              }
              case 10:
              {
                  uint32_t after_shrift = rm << shift;
                  int sign_bit = get_k_bit(rm, 31);
                  uint32_t mask = 0;
                  int i;
                  for (i==31; i>=32-shift; i--)
                  {
                      mask =+ sign_bit ^ i;
                  }
                  decode->u.data_process.operand2.op2 = after_shrift | mask;
              }
              case 11:
              {
                  //rotate.
              }
          }
      }
    
    //compute the result
    uint32_t Rn = bitfield_to_uint32(armstate->reg[decode->u.data_process.Rn]);
    uint32_t operand2 = bitfield_to_uint32(armstate->reg[decode->u.data_process.operand2.op2]);
    if (decode->u.data_process.OpCode)
    {
        switch (decode->u.data_process.OpCode)
        {
            case AND: result = (Rn && operand2);
            case EOR: result = (Rn ^ operand2);
            case SUB: result = (Rn - operand2);
            case RSB: result = (operand2 - Rn);
            case ADD: result = (Rn + operand2);
            case TST: (Rn && operand2);//result not written
            case TEQ: (Rn ^ operand2);//result not written
            case CMP: (Rn - operand2);//result not written 
            case ORR: result = (Rn || operand2);
            case MOV: result = (operand2);
        }
    }

    //save the result.
    armstate->reg[decode->u.data_process.Rd] = uint32_to_bitfield(result);
    
    //if S is set then CPSR should be update.
    if (decode->u.data_process.S)
    {
        armstate->flagN = get_k_bit(result, 31);
        if (result == 0) {armstate->flagZ = 0;}
        //changes with FlagC.
    }
}

void execute_SDT(instruction_t* decode, ArmState armstate)
{
    uint32_t result;

    uint32_t Rn = bitfield_to_uint32(armstate->reg[decode->u.trans.Rn]);
    uint32_t offset = bitfield_to_uint32(armstate->reg[decode->u.trans.offset]);

    //if U is set then offset is added to Rn. Otherwise the offset is subtracted from Rn.
    Rn = (decode->u.trans.U) ? offset + Rn : Rn - offset;

    //save the result.
    armstate->reg[decode->u.trans.Rd] = uint32_to_bitfield(result);
}

//helper function for branch.
//uint32_t sign_extend_24_32(uint32_t n) 
//{
//    const int bits = 24;
//    uint32_t m = 1u << (bits - 1);
//    return (n ^ m) - m;
//}

void execute_BRANCH(instruction_t* decode, ArmState armstate)
{
    uint32_t offset = bitfield_to_uint32(armstate->reg[decode->u.branch.offset]);
    offset << 2;

    uint32_t extented = get_k_bit(decode->u.branch.offset, 24);
    armstate->pc += extented;
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