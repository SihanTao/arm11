#include <stdlib.h>
#include <stdio.h>

#include "file_loader/assembler_file_loader.h"

#include "../parse/parse.h"

int main(int argc, char **argv)
{
  if (argc != 3)
  {
    perror("The number of argument is invalid.");
    exit(EXIT_FAILURE);
  }

  char *source = argv[1];
  char *output = argv[2];

  // Load the source code
  char **loaded_file = load_source_file(source);
  // Tokenize the input

  NotSure parsed = parse(loaded_file);

 // char **result = code_generate(parsed);
	char ** result;
	
  write_file(output, result);
}



