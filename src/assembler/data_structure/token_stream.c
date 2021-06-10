#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "token_stream.h"
#include "../file_loader/assembler_file_loader.h"

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
	// Get the operand field
	int length = comma_count(rest + 1);
	char** operand_field = split_operand_field(rest, length);

}

char* get_opcode(token_t* token, char* instruction)
{
	char* rest;
	token->opcode = strtok(instruction, " ");
	rest = strtok(NULL, " ");
	return rest;
}

/*
 * rest: r1,#3
 * --> "r1", "#3"
 * */

int comma_count(char* rest)
{
	int counter = 0;

	for (int i = 0; rest[i]; ++i)
	{
		if (rest[i] == ',')
		{
			counter++;
		}
	}

	return counter;
}

char** split_operand_field(char* rest, int length)
{
	char** fields = create_string_array(length);

	int i = 0;
	fields[i] = strtok(rest, ",");
	while (fields[i] != NULL)
	{
		fields[++i] = strtok(NULL, ",");
	}

	return fields;
}

void set_token_operand(token_t* token, char** operand_field, int length)
{
	token->operands = calloc(length + 1, sizeof(operand_t));
	char** current_operand = operand_field;
	while (*current_operand != NULL)
	{
		if (*current_operand[0] == '#' || *current_operand[0] == '=')
		{
			token->operands->tag = NUMBER;
			token->operands->operand_data.number = strtol(*current_operand + 1, NULL, 0);
		}
		else
		{
			token->operands->tag = STRING;
			token->operands->operand_data.letters = *current_operand;
		}
	}
}

void print_token(token_t* token)
{
	printf("%d: opcode: %s\n", token->line_num, token->opcode);
	bool flag = (token->operands->tag == STRING);
	printf("The token tag is: %s\n", flag ? "STRING" : "NUMBER");
	if (flag) {
		printf("Letters = %s\n", token->operands->operand_data.letters);
	} else {
		printf("Number = %d\n", token->operands->operand_data.number);
	}
}