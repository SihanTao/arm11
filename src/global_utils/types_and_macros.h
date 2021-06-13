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

typedef uint8_t  byte;
typedef uint32_t bitfield;

// res-pi has 64k of memory, thus max address is 65536
#define MAX_MEMORY_ADDRESS (65536)

// word_size = 32 (4 bytes)
#define NUM_OF_BIT_IN_WORD  (32)
#define NUM_OF_BYTE_IN_WORD (4)

// 12 general purpose registers
#define NUM_OF_REG (13)

typedef enum exit_type
{
  CONTINUE,
  EXIT,
  RESTART_PIPELINE
} exit_type;

/****************** enums **********************/

/*
 * Include the four instruction kind
 * with an undefined type
 */
typedef enum ins_type
{
  UNDEFINED,
  PROC,
  MUL,   // Multiplication
  TRANS, // Single data transfer
  BRAN,
  ZERO
} ins_type;

typedef enum pd_opcode_type
{
  AND = 0,
  EOR = 1,
  SUB = 2,
  RSB = 3,
  ADD = 4,
  TST = 8,
  TEQ = 9,
  CMP = 10,
  ORR = 12,
  MOV = 13
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
  NE = 1,  // not equal
  GE = 10, // greater or equal
  LT = 11, // less than
  GT = 12, // greater than
  LE = 13, // less than or equal
  AL = 14  // always
} cond_type;

typedef enum no_reg_t
{
  R0,
  R1,
  R2,
  R3,
  R4,
  R5,
  R6,
  R7,
  R8,
  R9,
  R10,
  R11,
  R12,
  PC = 15
} no_reg_t;

/************** components for words ***********************/

typedef struct shift_reg_t
{
  int        val;  // amount of shifting
  shift_type type; // datatype : shift type
  no_reg_t   Rm;
} shift_reg_t;

typedef struct rot_imm_t
{
  int amount; // amount of rotation
  int imm;    // target that rotation applied on
} rot_imm_t;

typedef union reg_or_imm_t
{
  rot_imm_t   rot_imm;
  shift_reg_t shift_reg;
} reg_or_imm_t;

/****************** words *****************/

typedef struct proc_t
{
  reg_or_imm_t   operand2;
  no_reg_t       Rd;
  no_reg_t       Rn;
  bool           set_cond;
  pd_opcode_type opcode; // datatype : pd_opcode_type
  bool           iFlag;  // else is register
} proc_t;

typedef struct mul_t
{
  no_reg_t Rm;
  no_reg_t Rs;
  no_reg_t Rn;
  no_reg_t Rd;
  bool     set_cond;
  bool     is_mla;
} mul_t;

typedef struct trans_t
{
  reg_or_imm_t offset;
  no_reg_t     Rd;
  no_reg_t     Rn;
  bool         is_load; // else is store
  bool         is_up;   // else is down
  bool         is_pre;  // else is post
  bool         iFlag;   // 0 -> immediate value; 1 -> register
} trans_t;

typedef struct branch_t
{
  int offset;
} branch_t;

/************************** tagged instruction *************************/

/*
 * the definition of the structure
 * for instructions
 */
typedef struct
{
  enum ins_type tag;
  cond_type     cond;
  union
  {
    proc_t   proc;
    mul_t    mul;
    trans_t  trans;
    branch_t branch;
  } word;
} instruction_t;

/************************** tagged instruction end ************************/

typedef struct arm_state_struct
{
  size_t    pc;
  bitfield *reg;
  byte *    memory;
  bool      neg;
  bool      zero;
  bool      carry;
  bool      ovflw; // overflow
} arm_state_struct;

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
typedef arm_state_struct *ArmState;

#endif // TYPES_AND_MACROS
