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
bitfield load(size_t address, byte *memory)
{
  bitfield result;
  if (address > MAX_MEMORY_ADDRESS - ADDRESS_SHIFT || address < 0)
  {
    perror("Error! Reading from invalid address!");
    exit(EXIT_FAILURE);
  }
  memcpy(&result, memory + address, sizeof(bitfield));
  return result;
}

/*!
 * Store the target bitfield into the specified memory
 * @param target : the bitfield to be stored
 * @param address
 * @param memory
 */
void store(bitfield target, size_t address, byte *memory)
{
  if (address > MAX_MEMORY_ADDRESS - ADDRESS_SHIFT || address < 0)
  {
    perror("Error! Writing to invalid address!");
    exit(EXIT_FAILURE);
  }
  memcpy(memory + address, &target, sizeof(bitfield));
}
