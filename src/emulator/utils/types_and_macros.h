#ifndef TYPES_AND_MACROS
#define TYPES_AND_MACROS

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef char byte;

#define MAX_MEMORY_ADDRESS 65536

#define NUM_OF_REG 12

/* Include the four instruction kind
 * with an undefined type
 */
enum instruction_kind
{
    UNDEFINED,
    DATA_PROCESS,
    MUL,
    TRANS,
    BRANCH,
    ZERO
};

//the struct of byte representation in memory
typedef struct
{
    unsigned int byte4: 8;
    unsigned int byte3: 8;
    unsigned int byte2: 8;
    unsigned int byte1: 8;
} bitfield;

/*
 * the definition of the structure
 * for instrctions
 */

typedef struct
{
    enum instruction_kind tag;
    union
    {
        uint32_t i;
        bitfield bf;
        struct
        {
            union // the union is used to present different cases of Op2.
            {
                struct // struct1 is the case when Op2 is an immediate value.
                {
                    unsigned int Rotate: 4;
                    unsigned int Imm: 8;
                }Iv;
                
                struct // struct2 is the case when Op2 is a register.
                {
                    struct
                    {
                        unsigned int Integer: 5;
                        unsigned int ShiftT: 2;
                        unsigned int: 1; // not used: 0.
                    }Shift;
                    unsigned int Rm: 4;
                }Register;
                
                unsigned int op2: 12; //this is used to do operations on Op2.
            }operand2;
            unsigned int Rd: 4;
            unsigned int Rn: 4;
            unsigned int S: 1;
            unsigned int OpCode: 4;
            unsigned int I: 1;
            unsigned int: 2; // not used: 00
            unsigned int cond: 4;
        } data_process;
        struct
        {
            unsigned int Rm: 4;
            unsigned int : 4; // not used:1001
            unsigned int Rs: 4;
            unsigned int Rn: 4;
            unsigned int Rd: 4;
            unsigned int S: 1;
            unsigned int A: 1;
            unsigned int: 6; // not used:0000
            unsigned int cond: 4;
        } mul;
        struct
        {
            union // the union is used to present different cases of Offset.
            {
                struct // struct1 is the case when Offset is a register.
                {
                    unsigned int Integer: 5;
                    unsigned int ShiftT: 2;
                    unsigned int: 1; // not used: 0.
                }Register;
                
                struct // struct2 is the case when Offset is an immediate offset.
                {
                    struct
                    {
                        unsigned int Rotate: 4;
                        unsigned int Imm: 8;

                    }Shift;
                    unsigned int Rm: 4;
                }Io;

                unsigned int offset_value: 12; //this is used to do operations on offset.
            }offset;
            unsigned int Rd: 4;
            unsigned int Rn: 4;
            unsigned int L: 1;
            unsigned int : 2; // not used 00
            unsigned int U: 1;
            unsigned int P: 1;
            unsigned int I: 1;
            unsigned int : 2; // not used 01
            unsigned int cond: 4;
        } trans;
        struct
        {
            unsigned int offset: 24;
            unsigned int: 4; // not used 1010
            unsigned int cond: 4;
        } branch;
    } u;
} instruction_t;

/*
 * The struct used to hold all states in this program
 */
typedef struct arm_state_struct{
  size_t pc;
  bitfield * reg;
  byte * memory;
  bool flagN;
  bool flagZ;
  bool flagC;
  bool flagV;
} arm_state_struct;

typedef arm_state_struct * ArmState;

#endif //TYPES_AND_MACROS
