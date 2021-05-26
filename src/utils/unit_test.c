#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "unit_test.h"

int global_test_num_counter = 0;
int global_pass_counter = 0;

static void summarise_test();

void test_true(bool target)
{
  global_test_num_counter += 1;

  if (target)
  {
    global_pass_counter += 1;
  }
  else
  {
    printf("[\033[0;31m FAIL \033[0m] test failed : %d \n",
           global_test_num_counter);
  }
}

void test_false(bool target)
{
  test_true(!target);
}

void test_eq(void const *got, void const *expect, size_t size)
{
  test_true(memcmp(got, expect, size) == 0);
}

void add_test(char const *test_name)
{
  summarise_test();
  printf("%s /n", test_name);

  global_test_num_counter = 0;
  global_pass_counter = 0;
}

void end_test()
{
  summarise_test();
}

static void summarise_test()
{
  char *msg = "\033[0;31m FAIL \033[0m"; // color : red

  if (global_test_num_counter != 0)
  {

    if (global_pass_counter == global_test_num_counter)
    {
      msg = "\033[0;32m PASS \033[0m"; // color : green
    }

    printf("[%s]Test passed : %d/ %d. \n", msg,
           global_pass_counter, global_test_num_counter);
    printf("---------------------------------\n\n");
  }
}