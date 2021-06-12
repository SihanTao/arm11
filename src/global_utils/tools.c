#include <assert.h>

#include "types_and_macros.h"

#include "tools.h"

void convert_endian_ptr(char *buffer)
{
  char temp;
  temp      = buffer[0];
  buffer[0] = buffer[3];
  buffer[3] = temp;
  temp      = buffer[1];
  buffer[1] = buffer[2];
  buffer[2] = temp;
}

bitfield convert_endian(bitfield word)
{
  byte temp;
  temp       = word.byte1;
  word.byte1 = word.byte4;
  word.byte4 = temp;
  temp       = word.byte2;
  word.byte2 = word.byte3;
  word.byte3 = temp;
  return word;
}

// It can perform some conditional big-endian convertion before return
uint32_t to_int(bitfield bf)
{
  if (TARGET_MACHINE_ENDIAN == BIG)
  {
    bf = convert_endian(bf);
  }
  uint32_t *result = (uint32_t *)&bf;
  return *result;
}

// It can perform some conditional big-endian convertion before return
bitfield to_bf(uint32_t i)
{
  bitfield *result = (bitfield *)&i;
  if (TARGET_MACHINE_ENDIAN == BIG)
  {
    *result = convert_endian(*result);
  }
  return *result;
}

int get_bit(uint32_t i, int k) { return (i & (1 << k)) >> k; }

int get_bit_range(int target, int start, int end)
{
  assert(start <= MAX_BIT_INDEX && start >= 0);
  assert(start < end);
  int      length = end - start + 1;
  uint32_t mask   = ALL_ONE >> (MAX_BIT_INDEX - length);
  return (target >> start) & mask;
}

//
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
  uint32_t selector = ~((ALL_ONE >> (MAX_BIT_INDEX - length)) << start);
  *dest             = (*dest & selector) | (src << start);
}
