#include "code_generate_helper.h"
#include "../../global_utils/unit_test.h"
#include "token_stream.h"
#include "types_and_macros.h"

int main(void)
{
  //mov r1, #56
  add_test("test generate_dp");
  
    Token token1 = { MOV, {56}, 1, NULL };
    instruction_t *instruction = (instruction_t *) malloc(sizeof(instruction_t));
    SymbolTable symbol_table = init_symbol_table();
    add_symbol_table(strdup("mov"), MOV, symbol_table);

    test_int_v(, 56, "mov r1, #56, r1 = 56");
  }

  end_all_tests();
}