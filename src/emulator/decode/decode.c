#include <stdlib.h>

#include "../utils/types_and_macros.h"

#include "decode.h"
#include "../utils/tools.h"

instruction_t decode (bitfield fetched)
{
  instruction_t decode = init_instruction (fetched);
  find_type (&decode);
  return decode;
}

void find_type (instruction_t *instruction)
{
  // TODO : 1. handle ZERO instruction
  //        2. default case : tag undefined

  uint32_t n = instruction->word.i;
  if (get_bit (n, 27))
    {
      instruction->tag = BRANCH;
    }
  else if (get_bit (n, 26))
    {
      instruction->tag = TRANS;
    }
  else if (get_bit (n, 25))
    {
      instruction->tag = DATA_PROCESS; // the case with op2 an immediate value
    }
  else if (get_bit (n, 4) && get_bit (n, 7))
    {
      instruction->tag = MUL;
    }
  else
    {
      instruction->tag = DATA_PROCESS;
    }
}

instruction_t init_instruction (bitfield fetched)
{
  instruction_t init;
  init.tag = UNDEFINED;
  init.word.bf = fetched;
  return init;
}
