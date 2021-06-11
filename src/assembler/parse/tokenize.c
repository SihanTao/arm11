#include <stdbool.h>
#include <string.h>

#include "../data_structrure/token_stream.h"

#include "tokenize.h"

void tokenize(char *line, int address, TokenStream token_stream)
{
  token_t *token_ptr = calloc(1, sizeof(token_t));
  if (token_ptr == NULL)
  {
    perror("Fail to allocate memory in tokenize_instruction");
    exit(EXIT_FAILURE);
  }

  token_ptr->real_address = address;

  char *rest = get_opcode(token_ptr, line);
  int    length        = count_num_operand(rest);
  char **operand_field = split_operand_field(rest, length);
  set_token_operand(token_ptr, operand_field, length);

  add_token_stream(token_ptr, token_stream);
}

char *get_opcode(token_t *token, char *instruction)
{
  char *rest;
  token->opcode = strtok(instruction, " ");
  rest          = strtok(NULL, " ");
  return rest;
}

/*
 * rest: r1,#3
 * --> "r1", "#3"
 * */

int count_num_operand(char *rest)
{
  int counter = 0;

  for (int i = 0; rest[i]; ++i)
  {
    if (rest[i] == ',')
    {
      counter++;
    }
  }
#
  return counter + 1;
}

char **split_operand_field(char *rest, int length)
{
  char **fields = create_string_array(length);

  int i     = 0;
  fields[i] = strtok(rest, ",");
  while (fields[i] != NULL)
  {
    fields[++i] = strtok(NULL, ",");
  }

  return fields;
}

void set_token_operand(token_t *token, char **operand_field, int length)
{
  token->operands        = calloc(length, sizeof(operand_t));
  token->num_operand     = length;
  int    num             = 0;
  char **current_operand = operand_field;
  while (*current_operand != NULL)
  {
    operand_t op = token->operands[num];
    if (*current_operand[0] == '#' || *current_operand[0] == '=')
    {
      token->operands[num].tag = NUMBER;
      token->operands[num].operand_data.number
          = strtol(*current_operand + 1, NULL, 0);
    }
    else
    {
      token->operands[num].tag                  = STRING;
      token->operands[num].operand_data.letters = *current_operand;
    }
    current_operand++;
    num++;
  }
}

void print_token(token_t* token)
{
	printf("%d: opcode: %s\n", token->real_address, token->opcode);
	for (int i = 0; i < token->num_operand; ++i)
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