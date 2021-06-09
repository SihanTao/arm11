//
// Created by tsh on 6/9/21.
//

#ifndef ARM11_49_SRC_ASSEMBLER_FILE_LOADER_ASSEMBLER_FILE_LOADER_H_
#define ARM11_49_SRC_ASSEMBLER_FILE_LOADER_ASSEMBLER_FILE_LOADER_H_
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

extern char **create_string_array(int line);
extern void free_string_array(char **strings, int line);
extern int count_line_number(FILE *fp);

#define MAX_LINE_LENGTH 511
#endif //ARM11_49_SRC_ASSEMBLER_FILE_LOADER_ASSEMBLER_FILE_LOADER_H_
