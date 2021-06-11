#include <stdlib.h>

#include "../../global_utils/types_and_macros.h"

#include "decode.h"
#include "../../global_utils/tools.h"

instruction_t decode_branch(uint32_t instruction);
instruction_t decode_dp(uint32_t fetched);
instruction_t decode_mul(uint32_t fetched);
instruction_t decode_trans(uint32_t fetched);

instruction_t decode(bitfield fetched)
{
  uint32_t fetched_val;
  if (fetched_val == 0)
  {
    instruction_t result;
    result.tag = ZERO;
    return result;
  }
  if (get_bit(fetched_val, 27))
  {
    return decode_branch(fetched_val);
  }
  else if (get_bit(fetched_val, 26))
  {
    return decode_trans(fetched_val);
  }
  else if (get_bit_range(fetched_val, 22, 27) == 0
           && get_bit_range(fetched_val, 4, 7) == 0x9)
  {
    return decode_mul(fetched_val);
  }
  else
  {
    return decode_dp(fetched_val);
  }
}

instruction_t decode_branch(uint32_t fetched)
{
  instruction_t result;
  result.word.branch.cond   = get_bit_range(fetched, 28, 31);
  result.word.branch.offset = get_bit_range(fetched, 0, 23);
  return result;
}

instruction_t decode_dp(uint32_t fetched)
{
  instruction_t result;
  result.word.branch.cond   = get_bit_range(fetched, 28, 31);
  result.word.branch.offset = get_bit_range(fetched, 0, 23);
  return result;
}

instruction_t decode_mul(uint32_t fetched)
{
  instruction_t result;
  result.word.branch.cond   = get_bit_range(fetched, 28, 31);
  result.word.branch.offset = get_bit_range(fetched, 0, 23);
  return result;
}

instruction_t decode_trans(uint32_t fetched)
{
  instruction_t result;
  result.word.branch.cond   = get_bit_range(fetched, 28, 31);
  result.word.branch.offset = get_bit_range(fetched, 0, 23);
  return result;
}
