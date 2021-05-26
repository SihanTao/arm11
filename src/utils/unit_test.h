#ifndef C_UNIT_TEST
#define C_UNIT_TEST

#include <stdlib.h>

extern int global_test_num_counter;
extern int global_pass_counter;

// typedef void (*on_fail)(void const * got, void const * expect);

#define VERBOSE false;

extern void test_int(int got, int expect);
extern void test_double(double got, double expect);
extern void test_char(char got, char expect);
extern void test_float(float got, float expect);
extern void test_long(long got, long expect);
extern void test_true(bool target);
extern void test_false(bool target);
extern void test_eq(void const *got, void const *expect, size_t size);
extern void add_test(char const *test_name);
extern void end_test(void);

#endif // C_UNIT_TEST
