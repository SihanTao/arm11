#include "../../global_utils/types_and_macros.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file_loader.h"

#include "../../global_utils/tools.h"

void read_file_to_mem(char const *file_name, void *write_to, endian_type mode)
{
  assert(write_to);
  int   position = 0;
  char  buffer[WORD_LENGTH];
  FILE *file_handler = fopen(file_name, "rb");

  if (file_handler == NULL)
  {
    printf("fopen error! please check file name: %s \n", file_name);
    assert(file_handler != NULL);
  }

  while (fread(buffer, WORD_LENGTH, 1, file_handler))
  {
    if (mode == BIG)
    {
      convert_endian_ptr(buffer);
    }
    memcpy((char *)write_to + position, buffer, WORD_LENGTH);
    position += 4;
  }

  if (!feof(file_handler))
  {
    printf("file not ended!");
  }

  fclose(file_handler);
  // TODO : fix error reporting
  // should get an error if doesn't match word length
}
