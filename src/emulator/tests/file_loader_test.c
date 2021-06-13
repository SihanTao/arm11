#include <string.h>

#include "../../global_utils/types_and_macros.h"

#include "../utils/file_loader.h"

#include "../utils/unit_test.h"

int main(void)
{
  add_test("little-endian test1");
  {
    char  file_name[]   = "file_loader_test_1";
    byte *memory_got    = calloc(MAX_MEMORY_ADDRESS, sizeof(byte));
    byte *memory_expect = calloc(MAX_MEMORY_ADDRESS, sizeof(byte));
    // 68 65 6c 6c 6f 20 77 6f 72 6c 64 0a
    strcpy(memory_expect, "hello world\n");

    init_memory(file_name, memory_got);

    test_eq(memory_got, memory_expect, MAX_MEMORY_ADDRESS);

    free(memory_expect);
    free(memory_got);
  }
  end_all_tests();
  return 0;
}
