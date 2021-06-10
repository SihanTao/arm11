#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "token_stream.h"

//bool is_end(token_t current)
//{
//  return current.opcode == 0;
//}
char* get_opcode(token_t* token, char* instruction);

token_t* tokenize_instruction(char* instruction, int num_line)
{
	token_t* token = calloc(1, sizeof(token_t));
	if (token == NULL)
	{
		perror("Fail to allocate memory in tokenize_instruction");
		exit(EXIT_FAILURE);
	}
	// First get the opcode
	char* rest = get_opcode(token, instruction);
	// Set the line number
	token->line_num = num_line;


}

char* get_opcode(token_t* token, char* instruction)
{
	char* rest;
	token->opcode = strtok(instruction, " ");
	rest = strtok(NULL, " ");
	return rest;
}