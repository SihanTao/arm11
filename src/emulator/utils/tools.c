#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "../utils/types_and_macros.h"
#include "tools.h"


void convert_endian_ptr(char *buffer)
{
  char temp;
  temp = buffer[0];
  buffer[0] = buffer[3];
  buffer[3] = temp;
  temp = buffer[1];
  buffer[1] = buffer[2];
  buffer[2] = temp;
}


bitfield convert_endian(bitfield word)
{
  byte temp;
  temp = word.byte1;
  word.byte1 = word.byte4;
  word.byte4 = temp;
  temp = word.byte2;
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

int get_bit(uint32_t i, int k)
{
  return (i & (1 << k)) >> k;
}

// TODO :
// I think it is probably better to make this as a static function in decode.
instruction_t init_instruction(bitfield fetched)
{
  instruction_t init;
  init.tag = UNDEFINED;
  init.u.bf = fetched;
  return init;
}

// It can perform some conditional big-endian convertion before return
// Todo : test it if the endian is big!
uint32_t to_int(bitfield bf)
{
  if (TARGET_MACHINE_ENDIAN == BIG)
  {
    bf = convert_endian(bf);
  }
  uint32_t * result = (uint32_t *) &bf;
  return *result;
}

// It can perform some conditional big-endian convertion before return
// Todo : test it if the endian is big!
bitfield to_bf(uint32_t i)
{
  bitfield * result = (bitfield *) &i;
  if (TARGET_MACHINE_ENDIAN == BIG)
  {
    *result = convert_endian(*result);
  }
  return *result;
}

bitfield peek(size_t position, byte * memory)
{
  bitfield result;
  memcpy(&result, memory + position, sizeof(bitfield));
  return result;
}
