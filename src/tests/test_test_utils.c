#include <stdbool.h>
#include "unit_test.h"

int main(void)
{
  add_test("test_test1");
  {
    test_true(true);
    test_true(false);
    test_false(false);
    test_false(true);
  }

  add_test("test_test2");
  {
    char *hello = "hello";
    char *hello2 = malloc(sizeof "hello");

    test_eq("hello", "hello", sizeof hello);
    test_eq("hello_world", "hello", sizeof hello);
  }

  add_test("success_test");
  {
    struct My_struct
    {
      int int_value;
      long long long_long_value;
      char char_value;
    };
    struct My_struct target_struct = {12, 13241234, 'a'};
    struct My_struct expect_struct = {13, 1232341, 'a'};
    struct My_struct target_struct2 = {13, 1232341, 'a'};

    // test_eq(&target_struct, &expect_struct, sizeof expect_struct);
    test_eq(&target_struct2, &expect_struct, sizeof expect_struct);
  }

  end_test();
}