#include "unit_test.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int global_test_num_counter = 0;
int global_pass_counter = 0;

static void summarise_test (void);

void test_int_v (int got, int expect, char const *msg)
{
  test_int (got, expect);
  printf ("%s got = %d, expect = %d\n", msg, got, expect);
}

void test_double_v (double got, double expect, char const *msg)
{
  test_double (got, expect);
  printf ("%s \n", msg);
}

void test_char_v (char got, char expect, char const *msg)
{
  test_char (got, expect);
  printf ("%s \n", msg);
}

void test_float_v (float got, float expect, char const *msg)
{
  test_float (got, expect);
  printf ("%s \n", msg);
}

void test_long_v (long got, long expect, char const *msg)
{
  test_float (got, expect);
  printf ("%s \n", msg);
}

void test_true_v (bool target, char const *msg)
{
  test_true (target);
  printf ("%s \n", msg);
}

void test_false_v (bool target, char const *msg)
{
  test_false (target);
  printf ("%s \n", msg);
}

void test_eq_v (void const *got, void const *expect, size_t size,
                char const *msg)
{
  test_eq (got, expect, size);
  printf ("%s \n", msg);
}

void test_string_eq_v(const char *got, const char *expect, const char* msg){
	test_true (strcmp(got, expect) == 0);
}


void test_int (int got, int expect) { test_true (got == expect); }

void test_double (double got, double expect) { test_true (got == expect); }

void test_char (char got, char expect) { test_true (got == expect); }

void test_float (float got, float expect) { test_true (got == expect); }

void test_long (long got, long expect) { test_true (got == expect); }

void test_true (bool target)
{
  global_test_num_counter += 1;

  if (target)
    {
      global_pass_counter += 1;
    }
  else
    {
      printf (
          "[\033[0;31m FAIL \033[0m] The %dth test in this block failed. \n",
          global_test_num_counter);
    }
}

void test_false (bool target) { test_true (!target); }

void test_eq (void const *got, void const *expect, size_t size)
{
  test_true (memcmp (got, expect, size) == 0);
}

void add_test (char const *test_name)
{
  summarise_test ();
  printf ("Now testing : %s \n", test_name);

  global_test_num_counter = 0;
  global_pass_counter = 0;
}

void end_all_tests (void) { summarise_test (); }

static void summarise_test (void)
{
  char msg[] = "\033[0;31m FAIL \033[0m"; // color : red

  if (global_test_num_counter != 0)
    {

      if (global_pass_counter == global_test_num_counter)
        {
          strcpy (msg, "\033[0;32m PASS \033[0m"); // color : green
        }

      printf ("[%s] Passed : %d Total : %d. \n", msg, global_pass_counter,
              global_test_num_counter);
      printf ("---------------------------------\n\n");
    }
}
