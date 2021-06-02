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

/****************** enums **********************/

/* Include the four instruction kind
 * with an undefined type
 */
typedef enum instruction_kind
{
  UNDEFINED,
  DATA_PROCESS,
  MUL,
  TRANS,
  BRANCH,
  ZERO
} instruction_kind;

typedef enum dp_opcode{
    AND, EOR, SUB, RSB, ADD, TST, TEQ, CMP, ORR, MOV
} dp_opcode;

typedef enum shift_type{
  LSL,
  LSR,
  ASR,
  ROR
} shift_type;

typedef enum load_store{
    LOAD, STORE
} load_store;

typedef enum instruction_cond{
    EQ = 0, NE, GE = 10, LT, GT, LE, AL
} instruction_cond;
/************** components for words ***********************/

typedef struct shift_reg_t
{
  struct
  {
    unsigned int integer : 5;
    unsigned int s_type : 2;
    unsigned int : 1; // not used: 0.
  } shift;
  unsigned int Rm : 4;
} shift_reg_t;

typedef struct rotate_t
{
  unsigned int rot_amt : 4;
  unsigned int rot_val : 8;
} rotate_t;

/****************** words *****************/

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
  // the union is used to present different cases of Op2.
  union
  {
    // struct1 is the case when Op2 is an immediate value.
    rotate_t imm_val;
    // struct2 is the case when Op2 is a register.
    shift_reg_t shift_reg;
    //this is used to do operations on Op2.
    // TODO : I am not sure whether we need this or not
    //unsigned int op2 : 12;
  } operand2;
  unsigned int Rd : 4;
  unsigned int Rn : 4;
  unsigned int S : 1;
  unsigned int opcode : 4;
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
    shift_reg_t shift_reg;
    // struct2 is the case when Offset is an immediate offset.
    rotate_t imm_offset;
    // TODO : I am not sure whether we need this or not
    //unsigned int offset_value : 12;
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

/************************** tagged instruction *************************/

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

/************************** tagged instruction end ************************/

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
