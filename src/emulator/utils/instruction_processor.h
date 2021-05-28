#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <stdint.h>
#include "types_and_macros.h"

enum {
    EQ = 0, NE, GE = 0b1010, LT, GT, LE, AL
};

// instruction_t* fetch(arm11_t *arm);
// functions needed in decode
bool test_instruction_cond(instruction_t* instruction, ArmState armstate);
int getcond(instruction_t* instruction);
void find_type(instruction_t* instruction);
/* TODO: execute instructions */

#endif
