#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <stdint.h>

#define NUM_REG 16

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
    } u;
} instruction_t;

/*
 * The definition of structure for 
 * the ARM machine
 */
typedef struct arm11
{
    instruction_t* instructions;
    int registers[NUM_REG];
} arm11_t;

instruction_t* fetch(arm11_t *arm);
void decode(instruction_t* instruction);
// functions needed in decode
void test_instruction_cond(instruction_t* instruction);
void find_type(instruction_t* instruction);
/* TODO: execute instructions */

#endif
