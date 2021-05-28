#ifndef EMULATE
#define EMULATE

typedef struct pipeline_state_struct {
    bitfield fetched;
    instruction_t decoded;
  } pipeline_state_struct;

typedef pipeline_state_struct * Pipeline;

# define FLASH_CYCLE temp = current; current = next; next = temp; states->pc++;

#endif //EMULATE
