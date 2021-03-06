#include <stdio.h>

#include "../../global_utils/types_and_macros.h"

#include "output.h"

#include "../utils/load_store.h"
#include "../../global_utils/tools.h"

static int cpsr_to_int(ArmState arm_state);

/*!
 *
 * @param arm_state : current state of the arm machine
 */
void output(ArmState arm_state)
{
  FILE *file_handle = stdout;

  int reg_value;
  int pc_val  = arm_state->pc;
  int CPSR    = cpsr_to_int(arm_state);
  int memory_val;

  // output registers
  fprintf(file_handle, "Registers:\n");
  for (int i = 0; i < NUM_OF_REG; i++)
  {
    reg_value =arm_state->reg[i];
    fprintf(file_handle, "$%-3d: %10d (%010p)\n", i, reg_value, reg_value);
  }

  // outputs pc
  fprintf(file_handle, "PC  : %10d (%010p)\n", pc_val, pc_val);
  fprintf(file_handle, "CPSR: %10d (%010p)\n", CPSR, CPSR);

  // outputs non zero memory
  fprintf(file_handle, "Non-zero memory:\n");
  for (int address = 0; address < 65536; address += 4)
  {
    load(address, arm_state->memory, &memory_val);
    memory_val = convert_endian(memory_val);
    if (memory_val == 0)
    {
      continue;
    }

    fprintf(file_handle, "%010p: %010p\n", address, memory_val);
  }

  fclose(file_handle);
}

/*!
 *
 * @param arm_state
 * @return an integer that represents the CPSR of the current arm_state
 */
int cpsr_to_int(ArmState arm_state)
{
  int CPSR = 0;
  if (arm_state->neg)
  {
    CPSR |= Nbit;
  }
  if (arm_state->zero)
  {
    CPSR |= Zbit;
  }
  if (arm_state->carry)
  {
    CPSR |= Cbit;
  }
  if (arm_state->ovflw)
  {
    CPSR |= Vbit;
  }

  return CPSR;
}