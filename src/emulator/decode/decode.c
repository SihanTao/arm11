#include <stdlib.h>

#include "../../global_utils/types_and_macros.h"

#include "decode.h"
#include "../../global_utils/tools.h"

instruction_t decode(bitfield fetched)
{
  instruction_t decode = init_instruction(fetched);
  find_type(&decode);
  return decode;
}

void find_type(instruction_t *instruction)
{
  // TODO : 1. handle ZERO instruction
  //        2. default case : tag undefined

  uint32_t n = instruction->word.i;
  if (get_bit(n, 27))
  {
    instruction->tag = BRANCH;
    // instruction->word.bf = convert_endian(instruction->word.bf);
  }
  else if (get_bit(n, 26))
  {
    instruction->tag = TRANS;
    // instruction->word.bf = convert_endian(instruction->word.bf);
  }
  else if (get_bit(n, 25))
  {
    instruction->tag = DATA_PROCESS; // the case with op2 an immediate value
    // instruction->word.bf = convert_endian(instruction->word.bf);
  }
  else if (get_bit(n, 4) && get_bit(n, 7))
  {
    instruction->tag = MUL;
    // instruction->word.bf = convert_endian(instruction->word.bf);
  }
  else
  {
    instruction->tag     = DATA_PROCESS;
    instruction->word.bf = convert_endian(instruction->word.bf);
  }
}

instruction_t init_instruction(bitfield fetched)
{
  instruction_t init;
  init.tag     = UNDEFINED;
  init.word.bf = fetched;
  return init;
}
