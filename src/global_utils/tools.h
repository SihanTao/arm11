#ifndef GLOBAL_TOOLS
#define GLOBAL_TOOLS

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include <stdbool.h>

#define ALL_ONE (0xFFFFFFFF)
#define ALL_ZERO (0x0)
#define MAX_BIT_INDEX (31)

#if defined(__linux__) || defined(__CYGWIN__)

#include <endian.h>
#define convert_endian(x) htobe32(x)

#elif defined(__APPLE__)

#include <libkern/OSByteOrder.h>
#define convert_endian(x) OSSwapHostToBigInt32(x)

#else

#	error platform not supported

#endif //linux or apple




// target = 0b011101001 start = 0 end = 3
// returns : 0b011101001
int get_bit_range(int target,int start,int end);

// dest = 101001000101 src = 10001 start = 0 end = 5
// dest = 101001010001
void set_bit_range(uint32_t * dest, int src, int start, int end);

void set_bit(uint32_t * dest, bool value, int position);

/*
 * Print the bit representation
 * of given uint32_t integer
 */
extern void print_bit (uint32_t i);

/*
 * Return the kth bit of a
 * given integer
 */
extern int get_bit (uint32_t i, int k);

#endif //GLOBAL_TOOLS
