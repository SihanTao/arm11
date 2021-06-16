#include <string.h>
#include <stdio.h>

#include "../../global_utils/types_and_macros.h"

#include "load_store.h"

/*!
 * Invalid read and write is handled inside the function
 * @param address
 * @param memory
 * @param result output param, returns the contents of memory at the specified
 * address
 * @return if success returns true else false
 */
bool load(size_t address, byte *memory, bitfield *result)
{
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
 * Invalid read and write is handled inside the function
 * @param target : the bitfield to be stored
 * @param address
 * @param memory
 * @return if success returns true else false
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
