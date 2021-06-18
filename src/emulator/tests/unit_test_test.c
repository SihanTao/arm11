#include "../utils/unit_test.h"
#include <stdbool.h>
#include <string.h>

int main (void)
{
  add_test ("test_test1");
  {
    test_true (true);
    // test_true(false);
    test_false (false);
    // test_false(true);
  }

  add_test ("test_test2");
  {
    char hello[] = "hello";
    char *hello2 = malloc (sizeof hello);
    strcpy (hello2, hello);

    test_eq ("hello", hello, sizeof hello);
    test_eq (hello2, hello, sizeof hello);
    // test_eq("hello_world", "hello", sizeof hello);
  }
  end_all_tests ();
}
