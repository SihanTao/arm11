#include <stdlib.h>
#include <stdio.h>

#include "../../global_utils/types_and_macros.h"

#include "execute.h"

#include "../execute/execute_helper.h"

exit_type execute(instruction_t decoded, ArmState arm_state)
{
  if (decoded.tag == ZERO)
  {
    return EXIT;
  }

  if (test_instruction_cond(decoded, arm_state))
  {
    switch (decoded.tag)
    {
    case PROC:
      execute_proc(decoded.word.proc, arm_state);
      return CONTINUE;
    case MUL:
      execute_mul(decoded.word.mul, arm_state);
      return CONTINUE;
    case TRANS:
      execute_trans(decoded.word.trans, arm_state);
      return CONTINUE;
    case BRAN:
      execute_bran(decoded.word.branch, arm_state);
      return RESTART_PIPELINE;
    default:
      perror("Error! Unknown instruction!");
      exit(EXIT_FAILURE);
    }
  }

  return CONTINUE;
}
