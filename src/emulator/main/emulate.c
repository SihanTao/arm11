#include <stdio.h>
#include <stdlib.h>

#include "../utils/types_and_macros.h"

#include "emulate.h"

#include "../decode/decode.h"
#include "../execute/execute.h"
#include "../utils/file_loader.h"
#include "../utils/tools.h"

static bitfield fetch(size_t pc, byte *memory);
static ArmState init_state(char const *file_name);
static void     free_state(ArmState states);
static Pipeline init_pipeline(void);
static void     free_pipeline(Pipeline pipeline);

int main(int argc, char **argv)
{
  char file_name[] = "I DON'T KNOW";
  // initialize all states
  ArmState states  = init_state(file_name);
  Pipeline next    = init_pipeline();
  Pipeline current = init_pipeline();
  Pipeline temp;
  if (states == NULL || next == NULL || current == NULL)
  {
    printf("%s", "Error : Running out of memory.");
    return EXIT_FAILURE;
    // OS will collect resources
  }

  // skip the first two cycles, for performance concern
  // no looger need to do NULL flag check in fetch and decode
  next->fetched = fetch(states->pc, states->memory);
  FLASH_CYCLE;
  next->fetched = fetch(states->pc, states->memory);
  next->decoded = decode(current->fetched);
  FLASH_CYCLE;

  // pipeline loop
  while (true)
  {
    next->fetched         = fetch(states->pc, states->memory);
    next->decoded         = decode(current->fetched);
    exit_type exit_status = execute(current->decoded, states);
    if (exit_status == EXIT)
    {
      break;
    }
    if (exit_status == ERROR)
    {
      perror("An error found, please check your code!");
      break;
    }
    FLASH_CYCLE;
  }

  // collect resources;
  free_state(states);
  free_pipeline(next);
  free_pipeline(current);
  return EXIT_SUCCESS;
}

ArmState init_state(char const *file_name)
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

  read_file_to_mem(file_name, result->memory, LITTLE);

  return result;
}

void free_state(ArmState state)
{
  free(state->memory);
  free(state->reg);
  free(state);
}

Pipeline init_pipeline(void)
{
  Pipeline result = malloc(sizeof(pipeline_state_struct));
  if (result == NULL)
  {
    return NULL;
  }

  result->fetched.byte1 = 0;
  result->fetched.byte2 = 0;
  result->fetched.byte3 = 0;
  result->fetched.byte4 = 0;

  result->decoded.tag    = UNDEFINED;
  result->decoded.word.i = 0;

  return result;
}

void free_pipeline(Pipeline pipeline) { free(pipeline); }

bitfield fetch(size_t pc, byte *memory) { return load(pc, memory); }
