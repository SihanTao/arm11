/*
 * Provide the interface of some 
 * functions for bit operation
 * 
 * Author: Sihan Tao
 */

#ifndef BITOPERATION_H
#define BITOPERATION_H

#include <stdint.h>
#include "types_and_macros.h"
#define INT_BIT_LENGTH 32

/*
 * Print the bit representation 
 * of given uint32_t integer
 */
void print_bit(uint32_t i);

/*
 * Return the kth bit of a
 * given integer
 */
int get_k_bit(uint32_t i, int k);

instruction_t init_instruction(bitfield fetched);
uint32_t bitfield_to_uint32(bitfield b);

#endif
