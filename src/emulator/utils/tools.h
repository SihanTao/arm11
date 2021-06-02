/*
 * Provide the interface of some
 * functions for bit operation
 *
 * Author: Sihan Tao
 */

#ifndef BITOPERATION_H
#define BITOPERATION_H



#define INT_BIT_LENGTH 32

/*
 * Print the bit representation
 * of given uint32_t integer
 */
extern void print_bit(uint32_t i);

/*
 * Return the kth bit of a
 * given integer
 */
extern int get_bit(uint32_t i, int k);

extern void convert_endian_ptr(char * buffer);
extern bitfield convert_endian(bitfield word);
extern uint32_t to_int(bitfield b);
extern bitfield to_bf(uint32_t i);
extern bitfield peek(size_t position, byte * memory);

#endif
