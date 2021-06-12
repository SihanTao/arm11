#ifndef DECODE
#define DECODE

/*
 * decode will initialize an instruction find the type of instruction then
 * return an instruction_t with tag and initialize the fields of instruction
 */
instruction_t decode(bitfield fetched);

#endif // DECODE
