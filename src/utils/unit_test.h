#ifndef C_UNIT_TEST
#define U_UNIT_TEST

#include <stdlib.h>

extern int global_test_num_counter;
extern int global_pass_counter;

#define VERBOSE false;

extern void test_true(bool target);
extern void test_false(bool target);
extern void test_eq(void const *target, void const *answer, size_t size);
extern void add_test(char const *test_name);
extern void end_test();

#endif // C_UNIT_TEST