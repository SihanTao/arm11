#include <stdio.h>

#include "../../global_utils/types_and_macros.h"

#include "init_arm_state.h"

/*!
 * initialize the state of the arm machine,
 * print error when memory allocation fails
 * @return initialized state of the arm machine
 */
ArmState init_state()
{
  ArmState result = malloc(sizeof(arm_state_struct));
  if (result == NULL)
  {
    perror("Out of memory!");
    exit(EXIT_FAILURE);
  }

  result->reg    = calloc(NUM_OF_REG, sizeof(bitfield));
  result->memory = calloc(MAX_MEMORY_ADDRESS, sizeof(byte));
  if (result->reg == NULL || result->memory == NULL)
  {
    perror("Out of memory!");
    exit(EXIT_FAILURE);
  }

  result->pc    = 0;
  result->neg   = false;
  result->zero  = false;
  result->carry = false;
  result->ovflw = false;

  return result;
}

/*!
 * free the memory used to store the state of the arm machine
 * @param state : the arm state to be freed
 */
void free_state(ArmState state)
{
  free(state->memory);
  free(state->reg);
  free(state);
}
