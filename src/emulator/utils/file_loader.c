#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../global_utils/types_and_macros.h"

#include "file_loader.h"

#include "../../global_utils/tools.h"

/*!
 * pre: file_name is not NULL
 * With error handling when failure to open the file and the file is not in expected format
 * @param file_name : the name of the ARM11 object code file
 * @param write_to : the address to store the information
 */
void init_memory(char const *file_name, void *write_to)
{
  if (!file_name)
  {
    perror("Error! please input a file_name!\n");
    exit(EXIT_FAILURE);
  }

  int   position = 0;
  char  buffer[NUM_OF_BYTE_IN_WORD];
  FILE *file_handler = fopen(file_name, "rb");

  if (file_handler == NULL)
  {
    perror("Error! invalid file name!\n");
    exit(EXIT_FAILURE);
  }

  while (fread(buffer, NUM_OF_BYTE_IN_WORD, 1, file_handler))
  {
    memcpy((char *)write_to + position, buffer, NUM_OF_BYTE_IN_WORD);
    position += ADDRESS_SHIFT;
  }

  if (!feof(file_handler))
  {
    perror("Error! format of file doesn't match!\n");
    exit(EXIT_FAILURE);
  }

  fclose(file_handler);
}
