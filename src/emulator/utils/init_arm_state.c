#include "../utils/types_and_macros.h"

#include "init_arm_state.h"

ArmState init_state()
{
  ArmState result = (ArmState)malloc(sizeof(arm_state_struct));
  if (result == NULL)
  {
    return NULL;
  }

  result->reg    = calloc(NUM_OF_REG, sizeof(bitfield));
  result->memory = calloc(MAX_MEMORY_ADDRESS, sizeof(byte));
  if (result->reg == NULL || result->memory == NULL)
  {
    return NULL;
  }

  result->pc    = 0;
  result->neg   = false;
  result->zero  = false;
  result->carry = false;
  result->ovflw = false;

  return result;
}
