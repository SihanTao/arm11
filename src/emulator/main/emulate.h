#ifndef EMULATE
#define EMULATE

typedef struct single_pipeline_t
{
  bitfield      fetched;
  instruction_t decoded;
} single_pipeline_t;

typedef struct pipelines_t
{
  single_pipeline_t *current;
  single_pipeline_t *next;
} pipelines_t;

/*
 * The main entrance of the emulator program
 */
extern int main(int argc, char **argv);

#endif // EMULATE
