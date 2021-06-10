#include <stdio.h>

#include "../utils/types_and_macros.h"

#include "init_arm_state.h"

ArmState init_state()
{
  ArmState result = (ArmState)malloc(sizeof(arm_state_struct));
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

void free_state(ArmState state)
{
  free(state->memory);
  free(state->reg);
  free(state);
}
