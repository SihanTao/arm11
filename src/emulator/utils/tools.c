#include <stdio.h>
#include <stdint.h>

#include "tools.h"

void print_bit(uint32_t i)
{
  for (int k = INT_BIT_LENGTH - 1; k >= 0; k--)
  {
    printf("%d", get_k_bit(i, k));
  }
  printf("\n");
}

int get_k_bit(uint32_t i, int k)
{
  return (i & (1 << k)) >> k;
}

instruction_t init_instruction(bitfield fetched)
{
    instruction_t init;
    init.tag = UNDEFINED;
    init.u.bf = fetched;
    return init;
}

uint32_t bitfield_to_uint32(bitfield bf)
{
    instruction_t temp = init_instruction(bf);
    return temp.u.i;
}

bitfield uint32_to_bitfield(uint32_t i)
{
   instruction_t temp;
   temp.tag = UNDEFINED;
   temp.u.i = i;
   return temp.u.bf;
}