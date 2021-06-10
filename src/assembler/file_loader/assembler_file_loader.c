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
#include <string.h>
/*
 * Take a number of line of the file, allocate the memory
 */
char** create_string_array(int line)
{
//	char** new = calloc(line, sizeof(char*));
//	if (!new)
//	{
//		perror("Fail to allocate memory in create_string_array()");
//		exit(EXIT_FAILURE);
//	}
//
//	for (int i = 0; i < line; ++i)
//	{
//		new[i] = calloc(1, sizeof(char) * MAX_LINE_LENGTH);
//		if (!new[i])
//		{
//			perror("Fail to allocate memory in create_string_array(), new[i]");
//			exit(EXIT_FAILURE);
//		}
//	}
//	return new;
	char** new = malloc(line * sizeof(char*));
	if (!new)
	{
		perror("Error allocating memory for source file");
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < line; i++)
	{
		new[i] = malloc(MAX_LINE_LENGTH * sizeof(char));
		if (!new[i])
		{
			perror("Error allocating memory for source file");
			exit(EXIT_FAILURE);
		}
	}
	return new;
}

void free_string_array(char** strings, int line)
{
	for (int i = 0; i < line; ++i)
	{
		free(strings[i]);
	}
	free(strings);
}

/*
 * pass a FILE*, return the number of lines
 */
int count_line_number(FILE* fp)
{
	int counter = 0;
	char buffer[MAX_LINE_LENGTH];
	while (fgets(buffer, MAX_LINE_LENGTH, fp))
	{
		counter++;
	}
	return counter;
}

/*
 * load the file from filename, return a string array contains
 * the lines of the file
 * */
char** load_source_file(char* filename)
{
	FILE* fp = fopen(filename, "r");
	if (fp == NULL)
	{
		perror("Error in opening source file");
		exit(EXIT_FAILURE);
	}

	//Set up the correct size array;
	int num_line = count_line_number(fp);
	char** loaded_file = create_string_array(num_line);
	fclose(fp);

	// read the file line by line
	fp = fopen(filename, "r");
	int current = 0;
	while (fgets(loaded_file[current], MAX_LINE_LENGTH, fp))
	{
		loaded_file[current][strlen(loaded_file[current]) - 1] = '\0';
		current++;
	}
	fclose(fp);
	return loaded_file;
}

void print_strings(char** loaded)
{
	char** curr = loaded;
	while (*curr != NULL)
	{
		printf("%s\n", *curr);
		curr++;
	}
}