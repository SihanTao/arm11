#include "../../global_utils/types_and_macros.h"

#include "../global_utils/types_and_macros.h"
#include "../global_utils/tools.h"

bitfield encode_DP(proc_t instruction)
{
  uint32_t result = 0;
  set_bit_range(&result, instruction.cond, 28, 31);
  set_bit(&result, instruction.iFlag, 25);
  set_bit_range(&result, instruction.opcode, 21, 24);
  set_bit_range(&result, instruction.Rn, 16, 19);
}
