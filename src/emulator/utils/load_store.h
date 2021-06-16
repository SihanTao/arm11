#ifndef LOAD_STORE
#define LOAD_STORE

extern bool load(size_t address, byte *memory, bitfield *result);
extern bool store(bitfield target, size_t address, byte *memory);

#endif //LOAD_STORE
