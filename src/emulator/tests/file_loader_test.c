#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "../utils/file_loader.h"
#include "../utils/unit_test.h"

#define ADDRESS_LENGTH 65536

int main(void)
{
  add_test("little-endian test1");
  {
    char file_name[] = "file_loader_test_1";
    char *memory_got = calloc(ADDRESS_LENGTH, sizeof (char));
    char *memory_expect = calloc(ADDRESS_LENGTH, sizeof (char));
    // 68 65 6c 6c 6f 20 77 6f 72 6c 64 0a
    strcpy(memory_expect, "hello world\n");

    read_file_to_mem(file_name, memory_got, little);

    test_eq(memory_got, memory_expect, ADDRESS_LENGTH);

    free(memory_expect);
    free(memory_got);
  }

  add_test("little-endian test2");
  {
    char file_name[] = "file_loader_test_2";
    char *memory_got = calloc(ADDRESS_LENGTH, sizeof (char));
    char *memory_expect = calloc(ADDRESS_LENGTH, sizeof (char));
    memcpy(memory_expect, "\xAA\xBB\xCC\xDD\x01\x02\x03\x04", 8);

    read_file_to_mem(file_name, memory_got, little);

    test_eq(memory_got, memory_expect, ADDRESS_LENGTH);

    free(memory_expect);
    free(memory_got);
  }

  add_test("big-endian test");
  {
    char file_name[] = "file_loader_test_2";
    char *memory_got = calloc(ADDRESS_LENGTH, sizeof (char));
    char *memory_expect = calloc(ADDRESS_LENGTH, sizeof (char));
    // 68 65 6c 6c 6f 20 77 6f 72 6c 64 0a
    memcpy(memory_expect, "\xDD\xCC\xBB\xAA\x04\x03\x02\x01", 8);

    read_file_to_mem(file_name, memory_got, big);

    test_eq(memory_got, memory_expect, ADDRESS_LENGTH);

    free(memory_expect);
    free(memory_got);
  }
  end_all_tests();
  return 0;
}
