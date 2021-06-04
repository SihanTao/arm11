#ifndef TYPES_AND_MACROS
#define TYPES_AND_MACROS

/*
 *
 * Type Conventions
 * ====================
 * `*_type` s are used for enums
 *
 * `*_t` s are used for non pointer types
 *
 * CamelCase types are used for pointer types
 *
 *
 * Table of contents
 * =====================
 *  - enums - instruction tags and instruction opcodes
 *
 *  - components of instructions - used in trans and dp instructions
 *
 *  - words - including bitfield and 4 instructions 32 bits long
 *
 *  - tagged instructions
 *
 *  - arm state - represent all machine states
 */

/*
 * In order to minisize redundant code, include some std-libs here.
 */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef uint8_t byte;

// default target machine endian is littel
#define TARGET_MACHINE_ENDIAN LITTLE

typedef enum endian_type
{
  BIG,
  LITTLE
} endian_type;

// res-pi has 64k of memory, thus max address is 65536
#define MAX_MEMORY_ADDRESS (65536)

// 12 general purpose registers
#define NUM_OF_REG (12)

typedef enum exit_type{
  CONTINUE,
  EXIT,
  ERROR
} exit_type;

/****************** enums **********************/

/*
 * Include the four instruction kind
 * with an undefined type
 */
typedef enum ins_type
{
  UNDEFINED,
  DATA_PROCESS,
  MUL,
  TRANS,
  BRANCH,
  ZERO
} ins_type;

typedef enum dp_type
{
  AND,
  EOR,
  SUB,
  RSB,
  ADD,
  TST,
  TEQ,
  CMP,
  ORR,
  MOV
} dp_type;

typedef enum shift_type
{
  LSL,
  LSR,
  ASR,
  ROR
} shift_type;

typedef enum trans_type
{
  LOAD,
  STORE
} trans_type;

typedef enum ins_cond_type
{
  EQ = 0,
  NE,
  GE = 10,
  LT,
  GT,
  LE,
  AL
} ins_cond_type;

/************** components for words ***********************/

// TODO : find a better name for this
typedef struct shift_reg_t
{
  struct
  {
    unsigned int val : 5;
    unsigned int type : 2;
    unsigned int : 1; // not used: 0.
  } shift;
  unsigned int Rm : 4;
} shift_reg_t;

// TODO : find a better name for this
typedef struct rotate_t
{
  unsigned int amount : 4;
  unsigned int value : 8;
} rotate_t;

typedef union reg_or_imm_t
{
  rotate_t imm_val;
  shift_reg_t shift_reg;
} reg_or_imm_t;

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
  reg_or_imm_t operand2;
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
  reg_or_imm_t offset;
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
  enum ins_type tag;
  union
  {
    uint32_t i;
    bitfield bf;
    data_process_t dp;
    mul_t mul;
    trans_t trans;
    branch_t branch;
  } word;
} instruction_t;

/************************** tagged instruction end ************************/

/*
 * The struct used to hold the state of the machine
 *
 * - In order to make memory byte addressable, we
 * used `byte *` for memory
 *
 * - CPSR regsiters are represented using 4 `bool`
 * value, which makes it easier to use
 */
typedef struct arm_state_struct
{
  size_t pc;
  bitfield *reg;
  byte *memory;
  bool flagN;
  bool flagZ;
  bool flagC;
  bool flagV;
} arm_state_struct;

typedef arm_state_struct *ArmState;

#endif //TYPES_AND_MACROS
