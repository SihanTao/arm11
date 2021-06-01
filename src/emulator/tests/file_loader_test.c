#include <string.h>

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

    read_file_to_mem(file_name, memory_got, LITTLE);

    test_eq(memory_got, memory_expect, MAX_MEMORY_ADDRESS);

    free(memory_expect);
    free(memory_got);
  }

  add_test("little-endian test2");
  {
    char file_name[] = "file_loader_test_2";
    byte *memory_got = calloc(MAX_MEMORY_ADDRESS, sizeof (byte));
    byte *memory_expect = calloc(MAX_MEMORY_ADDRESS, sizeof (byte));
    bitfield write_to_expect[] = {{0xaa, 0xbb, 0xcc, 0xdd}, {0x01, 0x02, 0x03, 0x04}};
    memcpy(memory_expect, &write_to_expect, 8);
    // aa bb cc dd 01 02 03 04
    read_file_to_mem(file_name, memory_got, LITTLE);

    test_eq(memory_got, memory_expect, MAX_MEMORY_ADDRESS);

    free(memory_expect);
    free(memory_got);
  }

  add_test("big-endian test");
  {
    char file_name[] = "file_loader_test_2";
    byte *memory_got = calloc(MAX_MEMORY_ADDRESS, sizeof (byte));
    byte *memory_expect = calloc(MAX_MEMORY_ADDRESS, sizeof (byte));
    bitfield write_to_expect[] = {{0xdd, 0xcc, 0xbb, 0xaa}, {0x04, 0x03, 0x02, 0x01}};
    memcpy(memory_expect, &write_to_expect, 8);
    // dd cc bb aa 04 03 02 01
    read_file_to_mem(file_name, memory_got, BIG);

    test_eq(memory_got, memory_expect, MAX_MEMORY_ADDRESS);

    free(memory_expect);
    free(memory_got);
  }
  end_all_tests();
  return 0;
}
