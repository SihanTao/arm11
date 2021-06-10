#include <string.h>
#include <stdio.h>

# include "../utils/types_and_macros.h"

# include "load_store.h"

bitfield load(size_t address, byte *memory)
{
  bitfield result;
  if(address > MAX_MEMORY_ADDRESS - 4 || address < 0)
  {
    perror("Error! Writing to invalid address!");
    exit(EXIT_FAILURE);
  }
  memcpy(&result, memory + address, sizeof(bitfield));
  return result;
}

void store(bitfield target, size_t address, byte *memory)
{
  if(address > MAX_MEMORY_ADDRESS - 4 || address < 0)
  {
    perror("Error! Writing to invalid address!");
    exit(EXIT_FAILURE);
  }
  memcpy(memory + address, &target, sizeof(bitfield));
}
