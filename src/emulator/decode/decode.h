#ifndef DECODE
#define DECODE

#include "../utils/tools.h"

// decode will not receive NULL values!
/* decode will initialize an instruction
 * then return an instruction_t with tag
 */
instruction_t decode (bitfield fetched);
void find_type(instruction_t* instruction);

#endif //DECODE
