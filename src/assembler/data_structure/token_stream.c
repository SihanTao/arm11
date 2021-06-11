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

	// Set the line number
	token->line_num = num_line;
	if (is_label_line(instruction))
	{
		token->opcode = instruction;
		token->operands = NULL;
	}
	else
	{
		// get the opcode
		char* rest = get_opcode(token, instruction);

		// Get the operand field
		int length = count_num_operand(rest);
		char** operand_field = split_operand_field(rest, length);
		set_token_operand(token, operand_field, length);
	}

	return token;
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

int count_num_operand(char* rest)
{
	int counter = 0;

	for (int i = 0; rest[i]; ++i)
	{
		if (rest[i] == ',')
		{
			counter++;
		}
	}

	return counter + 1;
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
	token->operands = calloc(length, sizeof(operand_t));
	int num = 0;
	char** current_operand = operand_field;
	while (*current_operand != NULL)
	{
		operand_t op = token->operands[num];
		if (*current_operand[0] == '#' || *current_operand[0] == '=')
		{
			token->operands[num].tag = NUMBER;
			token->operands[num].operand_data.number = strtol(*current_operand + 1, NULL, 0);
		}
		else
		{
			token->operands[num].tag = STRING;
			token->operands[num].operand_data.letters = *current_operand;
		}
		current_operand++;
		num++;
	}

}

void print_token(token_t* token, int num_operand)
{
	printf("%d: opcode: %s\n", token->line_num, token->opcode);
	for (int i = 0; i < num_operand; ++i)
	{
		printf("-------------------------------\n");
		printf("Printing the %dth operand\n", i);
		operand_t op = token->operands[i];
		bool flag = (op.tag == STRING);
		printf("The token tag is: %s\n", flag ? "STRING" : "NUMBER");
		if (flag)
		{
			printf("Letters = %s\n", op.operand_data.letters);
		}
		else
		{
			printf("Number = %d\n", op.operand_data.number);
		}
	}
}

bool is_label_line(char* line)
{
	return line[strlen(line) - 1] == ':';
}