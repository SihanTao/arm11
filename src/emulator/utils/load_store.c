#include <string.h>
#include <stdio.h>

#include "../../global_utils/types_and_macros.h"

#include "load_store.h"

bool load(size_t address, byte *memory, bitfield *result)
{
  bitfield result;
  if (address > MAX_MEMORY_ADDRESS - 4 || address < 0)
  {
    printf("Error: Out of bounds memory access at address %p", address);
    return false;
  }
  memcpy(&result, memory + address, sizeof(bitfield));
  return true;
}

bool store(bitfield target, size_t address, byte *memory)
{
  if (address > MAX_MEMORY_ADDRESS - 4 || address < 0)
  {
    printf("Error: Out of bounds memory access at address %p", address);
    return false;
  }
  memcpy(memory + address, &target, sizeof(bitfield));
  return true;
}
