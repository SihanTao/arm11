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

void print_bit(uint32_t i)
{
  for (int k = INT_BIT_LENGTH - 1; k >= 0; k--)
  {
    printf("%d", get_bit(i, k));
  }
  printf("\n");
}

int get_bit(uint32_t i, int k) { return (i & (1 << k)) >> k; }

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

int get_bit_range(int target, int start, int end)
{
  int length = end - start + 1;
  int mask   = 0;
  for (int i = 0; i < length; i++)
  {
    mask += pow(2, i);
  }
  return (target >> start) & mask;
}

void set_bit(uint32_t *dest, bool value, int position)
{
  if (get_bit(*dest, position))
  {
    if (!value)
    {
      *dest -= pow(2, position);
    }
  }
  else if (value)
  {
    *dest += pow(2, position);
  }
}

void set_bit_range(uint32_t *dest, int src, int start, int end)
{
  for (int i = start; i <= end; i++)
  {
	  set_bit(dest, get_bit(src, i), i);
  }
}
