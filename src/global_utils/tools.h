#ifndef GLOBAL_TOOLS
#define GLOBAL_TOOLS

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include <stdbool.h>

#define INT_BIT_LENGTH 32
// target = 0b011101001 start = 0 end = 3
// returns : 0b011101001
int get_bit_range(int target,int start,int end);

// dest = 101001000101 src = 10001 start = 0 end = 5
// dest = 101001010001
void set_bit_range(int* dest, int src, int start, int end);

void set_bit(int* dest, bool value, int position);

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

extern void convert_endian_ptr (char *buffer);
extern bitfield convert_endian (bitfield word);
extern uint32_t to_int (bitfield b);
extern bitfield to_bf (uint32_t i);


#endif //GLOBAL_TOOLS
