#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/types_and_macros.h"

bitfield read_word(size_t position, byte * memory)
{
  bitfield result;
  memcpy(&result, memory + position, sizeof(bitfield));
  return result;
}
