#include "../utils/types_and_macros.h"

#include "../utils/output.h"

#include "../utils/tools.h"
#include "../utils/init_arm_state.h"
#include "../utils/unit_test.h"

int main(void)
{
  add_test("test output file");
  {
    ArmState arm_state = init_state();

    for(int i = 0; i < NUM_OF_REG; i++)
    {
      arm_state->reg[i] = to_bf(i + 0xFFAA00A0);
    }

    for (int i = 0; i < 30; i++)
    {
      arm_state->memory[i] = (unsigned char) i;
    }

    arm_state->carry = 0;
    arm_state->neg = 1;
    arm_state->ovflw = 0;
    arm_state->zero = 1;

    output("target_file", arm_state);
  }
  end_all_tests();
  return 0;
}