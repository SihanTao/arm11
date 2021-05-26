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
    unsigned int byte1: 8;
    unsigned int byte2: 8;
    unsigned int byte3: 8;
    unsigned int byte4: 8;
} byteRep_t;

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
        byteRep_t bt;
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
/* TODO: execute instructions */

#endif
