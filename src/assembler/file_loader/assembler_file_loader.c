//
// Created by tsh on 6/9/21.
//
/*
 * The aim is to read a assembler file,
 * store it into a array of string
 * e.g. {"add r2, r4, r3", "mov r1, #56"}
 *
 * */

#include "assembler_file_loader.h"
/*
 * Take a number of line of the file, allocate the memory
 */
char **create_string_array(int line) {
  char **new = calloc(line, sizeof(char *));
  for (int i = 0; i < line; ++i) {
	new[i] = calloc(1, sizeof(char) * MAX_LINE_LENGTH);
  }
  return new;
}

void free_string_array(char **strings, int line) {
  for (int i = 0; i < line; ++i) {
	free(strings[i]);
  }
  free(strings);
}

/*
 * pass a FILE*, return the number of lines
 */
int count_line_number(FILE *fp) {
  int counter = 0;
  char buffer[MAX_LINE_LENGTH];
  while (fgets(buffer, MAX_LINE_LENGTH, fp)) {
	counter++;
  }
  return counter;
}

