#include "../utils/types_and_macros.h"
#include "../execute/execute.h"
#include "../utils/unit_test.h"

int main(void)
{
  add_test("Test for data processing execution");
  {

  }

  add_test("Test for Mul execution");
  {
    /* mov r0,#4294967293
     * mov r1,#15
     * mul r2,r1,r0
     */
      instruction_t instructions[] = {
          {4294967293, {0xFFFFFFFD}}, {15, {0x0000000F}}
      };

      ArmState armstate[] = {
        0, 0, 0, 0, 0, 0, 0
      };

      instruction_t *p = instructions;
      ArmState *a = armstate;
      execute_MUL(p, a);
      test_int(p->u.mul.Rd, 0xEFFFFFFD3);
  }
  
  add_test("Test for Trans execution");
  {
    /* STR R0, [R1, #12]
     * LDR R0, [R1, #12] 
     * STR R0, [R1], #12
     * LDR R0, [R1], #12
     */
  }

  add_test("Test for Branch execution");
  {
    
  }
  
  end_all_tests();

  return 0;
}