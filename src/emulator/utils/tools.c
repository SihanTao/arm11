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

// TODO: Haven't tested convert_endian
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

// TODO : should this be defined here?
// I think it is probably better to make this as a static function in decode.
instruction_t init_instruction(bitfield fetched)
{
  instruction_t init;
  init.tag = UNDEFINED;
  init.u.bf = fetched;
  return init;
}

// TODO : perform some conditional big-endian convertion before return
uint32_t to_int(bitfield bf)
{
  instruction_t temp = init_instruction(bf);
  return temp.u.i;
}

bitfield to_bf(uint32_t i)
{
  instruction_t temp;
  temp.tag = UNDEFINED;
  temp.u.i = i;
  return temp.u.bf;
}

bitfield read_word(size_t position, byte * memory)
{
  bitfield result;
  memcpy(&result, memory + position, sizeof(bitfield));
  return result;
}
