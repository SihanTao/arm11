#ifndef LOAD_STORE
#define LOAD_STORE

/*
 * invalid read and write is handled in these functions
 */
extern bitfield load(size_t address, byte *memory);

/*
 *
 */
extern void store(bitfield target, size_t address, byte *memory);

#endif LOAD_STORE
