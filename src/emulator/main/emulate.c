#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "emulate.h"
#include "../types_and_macros.h"
#include "../utils/file_loader.h"
#include "../decode/decode.h"
#include "../execute/execute.h"

static bitfield fetch(size_t pc, byte *memory);
static ArmState init_state(char const *file_name);
static void free_state(ArmState states);
static Pipeline init_pipeline();
static void free_pipeline(Pipeline pipeline);
static swap(void *a, void *b);

int main(int argc, char **argv)
{
  char file_name[] = "I DON'T KNOW";
  // initialize all states
  ArmState states = init_state(file_name);
  Pipeline next = init_pipeline();
  Pipeline current = init_pipeline();

  if (states == NULL || next == NULL || current == NULL)
  {
    return EXIT_FAILURE;
  }

  next->fetched = fetch(states->pc, states->memory);
  FLASH_CYCLE;
  next->fetched = fetch(states->pc, states->memory);
  next->decoded = decode(current->fetched);
  FLASH_CYCLE;

  while (true)
  {
    next->fetched = fetch(states->pc, states->memory);
    next->decoded = decode(current->fetched);
    if (execute(current->fetched, states))
    {
      free_state(states);
      free_pipeline(next);
      free_pipeline(current);
      return EXIT_SUCCESS;
    }
    FLASH_CYCLE;
  }
}

ArmState init_state(char const *file_name)
{
  ArmState result = (ArmState)malloc(sizeof(arm_state_struct));
  if (result == NULL)
  {
    return NULL;
  }

  result->reg = calloc(12, sizeof(bitfield));
  result->memory = calloc(65536, sizeof(byte));
  if (result->reg == NULL || result->memory == NULL)
  {
    return NULL;
  }

  result->pc = 0;
  result->flagN = false;
  result->flagZ = false;
  result->flagC = false;
  result->flagV = false;

  read_file_to_mem(file_name, result->memory, LITTLE_ENDIAN);

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
  result->fetched.byte1 = 0;
  result->fetched.byte2 = 0;
  result->fetched.byte3 = 0;
  result->fetched.byte4 = 0;

  result->decoded.tag = UNDEFINED;
  result->decoded.u.i = 0;

  return result;
}

void free_pipeline(Pipeline pipeline)
{
  free(pipeline);
}