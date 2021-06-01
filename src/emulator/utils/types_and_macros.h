#ifndef TYPES_AND_MACROS
#define TYPES_AND_MACROS

/*
 * In order to minisize redundant code, include some std-libs here.
 */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef char byte;

// default target machine endian is littel
#define TARGET_MACHINE_ENDIAN LITTLE
typedef enum endian_mode
{
  BIG,
  LITTLE
} endian_mode;

// res-pi has 64k of memory, thus max address is 65536
#define MAX_MEMORY_ADDRESS (65536)

// 12 general purpose registers
#define NUM_OF_REG (12)

/****************** instruction_t *********************/

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
typedef struct bitfield
{
  unsigned int byte4 : 8;
  unsigned int byte3 : 8;
  unsigned int byte2 : 8;
  unsigned int byte1 : 8;
} bitfield;

typedef struct data_process_t
{
  /*** begin operand 2 ***/
  // the union is used to present different cases of Op2.
  union
  {
    // struct1 is the case when Op2 is an immediate value.
    struct
    {
      unsigned int Rotate : 4;
      unsigned int Imm : 8;
    } Iv;
    // struct2 is the case when Op2 is a register.
    struct
    {
      struct
      {
        unsigned int Integer : 5;
        unsigned int ShiftT : 2;
        unsigned int : 1; // not used: 0.
      } Shift;
      unsigned int Rm : 4;
    } Register;
    //this is used to do operations on Op2.
    unsigned int op2 : 12;
  } operand2;
  /*** end operand 2 ***/

  unsigned int Rd : 4;
  unsigned int Rn : 4;
  unsigned int S : 1;
  unsigned int OpCode : 4;
  unsigned int I : 1;
  unsigned int : 2; // not used: 00
  unsigned int cond : 4;
} data_process_t;

typedef struct mul_t
{
  unsigned int Rm : 4;
  unsigned int : 4; // not used:1001
  unsigned int Rs : 4;
  unsigned int Rn : 4;
  unsigned int Rd : 4;
  unsigned int S : 1;
  unsigned int A : 1;
  unsigned int : 6; // not used:0000
  unsigned int cond : 4;
} mul_t;

typedef struct trans_t
{
  /*** begin offset ***/
  // the union is used to present different cases of Offset.
  union
  {
    // struct1 is the case when Offset is a register.
    struct
    {
      struct
      {
        unsigned int Integer : 5;
        unsigned int ShiftT : 2;
        unsigned int : 1; // not used: 0.
      } Shift;
      unsigned int Rm : 4;
    } Register;
    // struct2 is the case when Offset is an immediate offset.
    struct
    {
      unsigned int Rotate : 4;
      unsigned int Imm : 8;
    } Io;
    unsigned int offset_value : 12;
    //this is used to do operations on offset.
  } offset;
  /*** end offset ***/

  unsigned int Rd : 4;
  unsigned int Rn : 4;
  unsigned int L : 1;
  unsigned int : 2; // not used 00
  unsigned int U : 1;
  unsigned int P : 1;
  unsigned int I : 1;
  unsigned int : 2; // not used 01
  unsigned int cond : 4;
} trans_t;

typedef struct branch_t
{
  unsigned int offset : 24;
  unsigned int : 4; // not used 1010
  unsigned int cond : 4;
} branch_t;

/*
 * the definition of the structure
 * for instructions
 */
typedef struct
{
  enum instruction_kind tag;
  union
  {
    uint32_t i;
     // I wanted to remove this i, since it will brake protability
    bitfield bf;
    data_process_t data_process;
    mul_t mul;
    trans_t trans;
    branch_t branch;
  } u;
} instruction_t;

/*
 * The struct used to hold all states in this program
 */
typedef struct arm_state_struct
{
  size_t pc;
  bitfield *reg; // use bitfield to avoid
  byte *memory;  // In order to make it byte addressable
  bool flagN;
  bool flagZ;
  bool flagC;
  bool flagV;
} arm_state_struct;

typedef arm_state_struct *ArmState;

#endif //TYPES_AND_MACROS
