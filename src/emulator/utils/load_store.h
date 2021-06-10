#ifndef LOAD_STORE
#define LOAD_STORE

bitfield load(size_t address, byte *memory);
void store(bitfield target, size_t address, byte *memory);

#endif LOAD_STORE
