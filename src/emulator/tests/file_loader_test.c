#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "../utils/types_and_macros.h"
#include "../utils/file_loader.h"
#include "../utils/unit_test.h"

int main(void)
{
  add_test("little-endian test1");
  {
    char file_name[] = "file_loader_test_1";
    byte *memory_got = calloc(MAX_MEMORY_ADDRESS, sizeof (byte));
    byte *memory_expect = calloc(MAX_MEMORY_ADDRESS, sizeof (byte));
    // 68 65 6c 6c 6f 20 77 6f 72 6c 64 0a
    strcpy(memory_expect, "hello world\n");

    read_file_to_mem(file_name, memory_got, little);

    test_eq(memory_got, memory_expect, MAX_MEMORY_ADDRESS);

    free(memory_expect);
    free(memory_got);
  }

  add_test("little-endian test2");
  {
    char file_name[] = "file_loader_test_2";
    byte *memory_got = calloc(MAX_MEMORY_ADDRESS, sizeof (byte));
    byte *memory_expect = calloc(MAX_MEMORY_ADDRESS, sizeof (byte));
    memcpy(memory_expect, "\170\187\204\221\x01\x02\x03\x04", 8);
    // aa bb cc dd 01 02 03 04
    read_file_to_mem(file_name, memory_got, little);

    test_eq(memory_got, memory_expect, MAX_MEMORY_ADDRESS);

    free(memory_expect);
    free(memory_got);
  }

  add_test("big-endian test");
  {
    char file_name[] = "file_loader_test_2";
    byte *memory_got = calloc(MAX_MEMORY_ADDRESS, sizeof (byte));
    byte *memory_expect = calloc(MAX_MEMORY_ADDRESS, sizeof (byte));
    memcpy(memory_expect, "\221\204\187\170\004\003\002\001", 8);
    // dd cc bb aa 04 03 02 01
    read_file_to_mem(file_name, memory_got, big);

    test_eq(memory_got, memory_expect, MAX_MEMORY_ADDRESS);

    free(memory_expect);
    free(memory_got);
  }
  end_all_tests();
  return 0;
}
