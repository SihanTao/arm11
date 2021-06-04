#ifndef DECODE
#define DECODE

/*
 * decode will initialize an instruction
 * then return an instruction_t with tag
 */
instruction_t decode (bitfield fetched);

void find_type(instruction_t* instruction);

/*
 * initialize an instruction to UNDEFINED
 * category.
 */
instruction_t init_instruction(bitfield fetched);

#endif //DECODE
