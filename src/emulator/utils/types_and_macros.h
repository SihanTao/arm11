#define NUM_REG 16
#include <stdbool.h>
/* Include the four instruction kind
 * with an undefined type
 */
enum instruction_kind
{
    UNDEFINED,
    DATA_PROCESS,
    MUL,
    TRANS,
    BRANCH
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
            unsigned int operand2: 12;
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
            unsigned int offset: 12;
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

typedef char byte;

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