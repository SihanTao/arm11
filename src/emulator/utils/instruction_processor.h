#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <stdint.h>
#include "types_and_macros.h"

// instruction_t* fetch(arm11_t *arm);
void decode(instruction_t* instruction);
// functions needed in decode
void test_instruction_cond(instruction_t* instruction);
int getcond(instruction_t* instruction);
void find_type(instruction_t* instruction);
/* TODO: execute instructions */

#endif
