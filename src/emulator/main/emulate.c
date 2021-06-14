#include <stdio.h>
#include <stdlib.h>

#include "../../global_utils/types_and_macros.h"

#include "emulate.h"

#include "../decode/decode.h"
#include "../execute/execute.h"
#include "../utils/file_loader.h"
#include "../utils/init_arm_state.h"
#include "../utils/load_store.h"
#include "../utils/output.h"

static bitfield fetch(size_t pc, byte *memory);

static void flash_cycle(size_t *pc, pipelines_t pipelines);

static void preheat_pipeline(ArmState arm_state, pipelines_t pipelines);
static void free_all(ArmState arm_state, pipelines_t pipelines);
static void free_pipeline(single_pipeline_t *pipeline);
static single_pipeline_t *init_single_pipeline(void);
static pipelines_t        init_pipelines(void);

/*!
 * main calls the following functions:
 * init_memory from file_loader.h to load the file into memory
 * init_pipelines(), preheat_pipeline() to create the three stage pipeline
 * decode and execute from decode.h and execute.h respectively for the fetch-decode-execute cycle
 * output from output.h which prints the registers
 *        and non-zeroed memory in a format which passes the automated tests.
 * @param argc should be 2
 * @param argv : argv[1] is the filename containing ARM11 object code
 * @return
 */
int main(int argc, char **argv)
{
  char *file_name = argv[1];

  ArmState arm_state = init_state();
  init_memory(file_name, arm_state->memory);
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

  output("something", arm_state);

  free_all(arm_state, pipelines);
  return EXIT_SUCCESS;
}

void flash_cycle(size_t *pc, pipelines_t pipelines)
{
  single_pipeline_t temp;
  temp               = *pipelines.current;
  *pipelines.current = *pipelines.next;
  *pipelines.next    = temp;
  *pc                = *pc + 4;
}

void preheat_pipeline(ArmState arm_state, pipelines_t pipelines)
{
  pipelines.next->fetched = fetch(arm_state->pc, arm_state->memory);
  flash_cycle(&arm_state->pc, pipelines);
  pipelines.next->fetched = fetch(arm_state->pc, arm_state->memory);
  pipelines.next->decoded = decode(pipelines.current->fetched);
  flash_cycle(&arm_state->pc, pipelines);
}


pipelines_t init_pipelines(void)
{
  pipelines_t result
      = { .current = init_single_pipeline(), .next = init_single_pipeline() };
  return result;
}

/*!
 * initialize a single pipeline, print error when memory allocation fails
 * @return an empty single pipeline
 */
single_pipeline_t *init_single_pipeline(void)
{
  single_pipeline_t *result = malloc(sizeof(single_pipeline_t));
  if (result == NULL)
  {
    perror("Error! out of memory!\n");
    exit(EXIT_FAILURE);
  }

  result->fetched     = 0;
  result->decoded.tag = UNDEFINED;

  return result;
}

/*!
 * Free all the allocated memory on the heap
 * @param arm_state
 * @param pipelines
 */
void free_all(ArmState arm_state, pipelines_t pipelines)
{
  free_state(arm_state);
  free(pipelines.current);
  free(pipelines.next);
}

/*!
 *
 * @param pc : the address of program counter
 * @param memory : the memory of the arm machine
 * @return fetched instruction in the form of bitfield at address pc
 */
bitfield fetch(size_t pc, byte *memory) { return load(pc, memory); }
