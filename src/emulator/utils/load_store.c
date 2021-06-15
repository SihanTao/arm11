#include <string.h>
#include <stdio.h>

#include "../../global_utils/types_and_macros.h"

#include "load_store.h"

/*!
 *
 * @param address
 * @param memory
 * @return the contents of memory at the specified address

 */
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

/*!
 * Store the target bitfield into the specified memory
 * @param target : the bitfield to be stored
 * @param address
 * @param memory
 */
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
