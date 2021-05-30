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
    
    int Change_FlagC = 0; // the carry out bit need to be store in FlagC.
    
    if (decode->u.data_process.I) // OP2 is an immediate value.
    {
        
        int rotation_amount = 2 * bitfield_to_uint32(armstate->reg[decode->u.data_process.operand2.Iv.Rotate]);
        uint32_t Imm = bitfield_to_uint32(armstate->reg[decode->u.data_process.operand2.Iv.Imm]);
        int af_rot_val = 0;
        for (int i=0; i<rotation_amount; i++)
        {
            af_rot_val =+ get_k_bit(Imm, i) ^ (31-i);
        }
        armstate->reg[decode->u.data_process.operand2.Iv.Imm] = uint32_to_bitfield(af_rot_val + (Imm << rotation_amount));
        Change_FlagC = get_k_bit(Imm, rotation_amount-1);
    
    } 
      else //OP2 is a register.
      {
          int shift_val = bitfield_to_uint32(armstate->reg[decode->u.data_process.operand2.Register.Shift.Integer]);
          uint32_t rm = bitfield_to_uint32(armstate->reg[decode->u.data_process.operand2.Register.Rm]);
          
          switch (bitfield_to_uint32(armstate->reg[decode->u.data_process.operand2.Register.Shift.ShiftT]))
          {
              case 00:
              {
                  armstate->reg[decode->u.data_process.operand2.op2] = uint32_to_bitfield(rm >> shift_val);
                  Change_FlagC = get_k_bit(rm, 32-shift_val);
                  break;
              }
              case 01:
              {
                  armstate->reg[decode->u.data_process.operand2.op2] = uint32_to_bitfield(rm << shift_val);
                  Change_FlagC = get_k_bit(rm, shift_val-1);
                  break;
              }
              case 10:
              {
                  uint32_t after_shift = rm << shift_val;
                  int sign_bit = get_k_bit(rm, 31);
                  uint32_t mask = 0;
                  for (int i=31; i>=32-shift_val; i--)
                  {
                      mask =+ sign_bit ^ i;
                  }
                  armstate->reg[decode->u.data_process.operand2.op2] = uint32_to_bitfield(after_shift | mask);
                  Change_FlagC = get_k_bit(rm, shift_val-1);
                  break;
              }
              case 11:
              {
                  int af_rot_val = 0;
                  for (int i=0; i<shift_val; i++)
                  {
                      af_rot_val =+ get_k_bit(rm, i) ^ (31-i);
                  }
                  armstate->reg[decode->u.data_process.operand2.op2] = uint32_to_bitfield(af_rot_val + (rm << shift_val));
                  Change_FlagC = get_k_bit(rm, shift_val-1);
                  break;
              }
              default:
                break;
          }
      }
    
    //compute the result
    uint32_t Rn = bitfield_to_uint32(armstate->reg[decode->u.data_process.Rn]);
    uint32_t operand2 = bitfield_to_uint32(armstate->reg[decode->u.data_process.operand2.op2]);
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
        default:
            break;
    }

    //save the result.
    armstate->reg[decode->u.data_process.Rd] = uint32_to_bitfield(result);
    
    //if S is set then CPSR should be update.
    if (decode->u.data_process.S)
    {
        armstate->flagN = get_k_bit(result, 31);
        armstate->flagZ = (!result) ? 1 : 0;
        armstate->flagC = Change_FlagC;
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