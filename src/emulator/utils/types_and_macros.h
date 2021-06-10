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
 *  - components of instructions - used in trans and proc instructions
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

// word_size = 32 (4 bytes)
#define WORD_SIZE (32)

// 12 general purpose registers
#define NUM_OF_REG (12)

typedef enum exit_type
{
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
  MUL,   // Multiplication
  TRANS, // Single data transfer
  BRANCH,
  ZERO
} ins_type;

typedef enum pd_opcode_type
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
} pd_opcode_type;

typedef enum shift_type
{
  LSL, // Logical left
  LSR, // Logical right
  ASR, // Arithmetic right
  ROR  // Rotate right
} shift_type;

typedef enum trans_type
{
  LOAD,
  STORE
} trans_type;

typedef enum cond_type
{
  EQ = 0,  // equal
  NE,      // not equal
  GE = 10, // greater or equal
  LT,      // less than
  GT,      // greater than
  LE,      // less than or equal
  AL       // always
} cond_type;

/************** components for words ***********************/

typedef struct shift_reg_t
{
  unsigned int val : 5;  // amount of shifting
  unsigned int type : 2; // datatype : shift type
  unsigned int : 1;      // not used: 0.
  unsigned int Rm : 4;
} shift_reg_t;

typedef struct rot_imm_t
{
  unsigned int amount : 4; // amount of rotation
  unsigned int imm : 8;    // target that rotation applied on
} rot_imm_t;

typedef union reg_or_imm_t
{
  rot_imm_t rot_imm;
  shift_reg_t shift_reg;
} reg_or_imm_t;

/****************** words *****************/

// represents word in memory and registers
typedef struct bitfield
{
  unsigned int byte4 : 8;
  unsigned int byte3 : 8;
  unsigned int byte2 : 8;
  unsigned int byte1 : 8;
} bitfield;

typedef struct proc_t
{
  reg_or_imm_t operand2;
  unsigned int Rd : 4;
  unsigned int Rn : 4;
  unsigned int set_cond : 1;
  unsigned int opcode : 4; // datatype : pd_opcode_type
  unsigned int iFlag : 1; // else is register
  unsigned int : 2;        // not used: 00
  unsigned int cond : 4;
} proc_t;

typedef struct mul_t
{
  unsigned int Rm : 4;
  unsigned int : 4; // not used:1001
  unsigned int Rs : 4;
  unsigned int Rn : 4;
  unsigned int Rd : 4;
  unsigned int set_cond : 1;
  unsigned int acc : 1;
  unsigned int : 6; // not used:0000
  unsigned int cond : 4;
} mul_t;

typedef struct trans_t
{
  reg_or_imm_t offset;
  unsigned int Rd : 4;
  unsigned int Rn : 4;
  unsigned int is_load : 1; // else is store
  unsigned int : 2;         // not used 00
  unsigned int is_up : 1;   // else is down
  unsigned int is_pre : 1;  // else is post
  unsigned int iFlag : 1;  // 0 -> immediate value; 1 -> register
  unsigned int : 2;         // not used 01
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
    proc_t proc;
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
 *   used `byte *` for memory
 *
 * - CPSR regsiters are represented using 4 `bool`
 *   value, which makes it easier to use
 *
 * - The reason we used bitfield for `reg *` is
 *   because we left the opportunity to make the
 *   code compatible with both big and little endian
 *   machines
 */
typedef struct arm_state_struct
{
  size_t pc;
  bitfield *reg;
  byte *memory;
  bool neg;
  bool zero;
  bool carry;
  bool ovflw; // overflow
} arm_state_struct;

typedef arm_state_struct *ArmState;

#endif // TYPES_AND_MACROS
