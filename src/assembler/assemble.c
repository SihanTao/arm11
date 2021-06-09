#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv) {
  if (argc != 3) {
	perror("The number of argument is invalid.");
	exit(EXIT_FAILURE);
  }

  char *source = argv[1];
  char *output = argv[2];

  // Load the source code

  // Tokenize the input

  // create a symbol table

  // reads opcode mnmonic and operand
  // then encode the instructions
}
