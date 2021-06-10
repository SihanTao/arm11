#include <stdio.h>
#include <stdlib.h>

#include "../utils/types_and_macros.h"

#include "emulate.h"

#include "../decode/decode.h"
#include "../execute/execute.h"
#include "../utils/file_loader.h"
#include "../utils/init_arm_state.h"
#include "../utils/load_store.h"

static bitfield           fetch(size_t pc, byte *memory);
static void               free_all(ArmState arm_state, pipelines_t pipelines);
static single_pipeline_t *init_single_pipeline(void);
static pipelines_t        init_pipelines(void);
static void               free_pipeline(single_pipeline_t *pipeline);
static void               flash_cycle(size_t *pc, pipelines_t pipelines);
void preheat_pipeline(ArmState arm_state, pipelines_t pipelines);

int main(int argc, char **argv)
{
  char *file_name = argv[1];

  ArmState arm_state = init_state();
  read_file_to_mem(file_name, arm_state->memory, LITTLE);
  pipelines_t pipelines = init_pipelines();

  // skip the first two cycles
  preheat_pipeline(arm_state, pipelines);

  while (true)
  {
    pipelines.next->fetched = fetch(arm_state->pc, arm_state->memory);
    pipelines.next->decoded = decode(pipelines.current->fetched);
    exit_type exit_status   = execute(pipelines.current->decoded, arm_state);
    if (exit_status == EXIT)
    {
      break;
    }

    if (exit_status == RESTART_PIPELINE)
    {
      preheat_pipeline(arm_state, pipelines);
      continue;
    }

    flash_cycle(&arm_state->pc, pipelines);
  }

  free_all(arm_state, pipelines);
  return EXIT_SUCCESS;
}

pipelines_t init_pipelines(void)
{
  pipelines_t result
      = { .current = init_single_pipeline(), .next = init_single_pipeline() };
  return result;
}

void flash_cycle(size_t *pc, pipelines_t pipelines)
{
  single_pipeline_t temp;
  temp               = *pipelines.current;
  *pipelines.current = *pipelines.next;
  *pipelines.next    = temp;
  *pc                = *pc + 1;
}

void preheat_pipeline(ArmState arm_state, pipelines_t pipelines)
{
  pipelines.next->fetched = fetch(arm_state->pc, arm_state->memory);
  flash_cycle(&arm_state->pc, pipelines);
  pipelines.next->fetched = fetch(arm_state->pc, arm_state->memory);
  pipelines.next->decoded = decode(pipelines.current->fetched);
  flash_cycle(&arm_state->pc, pipelines);
}

single_pipeline_t *init_single_pipeline(void)
{
  single_pipeline_t *result = malloc(sizeof(single_pipeline_t));
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

void free_all(ArmState arm_state, pipelines_t pipelines)
{
  free_state(arm_state);
  free(pipelines.current);
  free(pipelines.next);
}

bitfield fetch(size_t pc, byte *memory) { return load(pc, memory); }
