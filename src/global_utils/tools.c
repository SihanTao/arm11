#include <assert.h>

#include "types_and_macros.h"

#include "tools.h"

int get_bit(uint32_t i, int k) { return (i & (1 << k)) >> k; }

int get_bit_range(int target, int start, int end)
{
  assert(start <= MAX_BIT_INDEX && start >= 0);
  assert(start < end);
  int      length = end - start + 1;
  uint32_t mask   = ALL_ONE >> (NUM_OF_BIT_IN_WORD - length);
  return (target >> start) & mask;
}

void set_bit(uint32_t *dest, bool value, int position)
{
  assert(position <= MAX_BIT_INDEX && position >= 0);
  uint32_t mask = 1 << position;
  *dest         = value ? *dest | mask : *dest & ~mask;
}

void set_bit_range(uint32_t *dest, int src, int start, int end)
{
  assert(start <= MAX_BIT_INDEX && start >= 0);
  assert(start < end);
  int      length   = end - start + 1;
  uint32_t selector = ~((ALL_ONE >> (NUM_OF_BIT_IN_WORD - length)) << start);
  *dest             = (*dest & selector) | (src << start);
}
