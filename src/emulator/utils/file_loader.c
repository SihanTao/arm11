#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include "../utils/types_and_macros.h"

#include "file_loader.h"

#include "../utils/tools.h"

void read_file_to_mem(char const *file_name, void *write_to, endian_mode mode)
{
  assert(write_to);
  int position = 0;
  char buffer[WORD_LENGTH];
  FILE *file_handler = fopen(file_name, "rb");

  if (file_handler == NULL)
  {
    printf("fopen error! please check file name: %s \n", file_name);
    assert(file_handler != NULL);
  }

  while (fread(buffer, WORD_LENGTH, 1, file_handler))
  {
    if (mode == big)
    {
      convert_endian_ptr(buffer);
    }
    memcpy(write_to + position, buffer, WORD_LENGTH);
    position += 4;
  }
  // TODO : fix error reporting
  // should get an error if doesn't match word length
}
