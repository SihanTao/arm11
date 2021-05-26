#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include "file_loader.h"

static void sendian_handle(char *buffer)
{
  char temp;
  temp = buffer[0];
  buffer[0] = buffer[3];
  buffer[3] = temp;
  temp = buffer[1];
  buffer[1] = buffer[2];
  buffer[2] = temp;
}

void read_file_to_mem(char const *file_name, char *write_to, endian_mode mode)
{
  assert(write_to);
  int position = 0;
  char buffer[WORD_LENGTH];
  FILE *file_handler = fopen(file_name, "rb");

  while (fread(buffer, WORD_LENGTH, 1, file_handler))
  {
    if (mode == small)
    {
      sendian_handle(buffer);
    }
    memcpy(write_to + position, buffer, WORD_LENGTH);
    position += 4;
  }
  assert(EOF != fgetc(file_handler));
}
