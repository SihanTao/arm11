#include <stdlib.h>
#include <stdio.h>

#include "../../global_utils/types_and_macros.h"

#include "execute.h"

#include "../execute/execute_helper.h"

/*!
 *
 * @param decoded: A decoded instruction
 * @param arm_state: the current state of the arm machine
 * @return the exit type of the ARM machine after execute the instruction
 */
exit_type execute(instruction_t decoded, ArmState arm_state)
{
  if (test_instruction_cond(decoded, arm_state))
  {
    switch (decoded.tag)
    {
    case DATA_PROCESS:
      execute_DP(decoded.word.proc, arm_state);
      return CONTINUE;
    case MUL:
      execute_MUL(decoded.word.mul, arm_state);
      return CONTINUE;
    case TRANS:
      execute_TRANS(decoded.word.trans, arm_state);
      return CONTINUE;
    case BRANCH:
      execute_BRANCH(decoded.word.branch, arm_state);
      return RESTART_PIPELINE;
    case ZERO:
      return EXIT;
    default:
      perror("Internal Error! Unknown instruction!");
      exit(EXIT_FAILURE);
    }
  }

  return CONTINUE;
}
