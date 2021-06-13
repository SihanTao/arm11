#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "../data_structure/token_stream.h"

#include "tokenize.h"

void trim(char **cur_pos, char until, char **dst)
{
  int   i;
  char *string = *cur_pos;
  for (i = 0; string[i] != '\0'; i++)
  {
    if (string[i] == until)
    {
      *dst     = strndup(string, i);
      *cur_pos = string + i + 1;
      return;
    }
  }
  *dst     = strndup(string, i);
  *cur_pos = NULL;
}

operand_t *gen_operand(char *trimed)
{
  operand_t *current_operand = malloc(sizeof(operand_t));
  if (trimed[0] == '#')
  {
    current_operand->tag                 = NUMBER;
    current_operand->operand_data.number = atoi(trimed);
    free(trimed);
  }
  else if (trimed[0] == '=')
  {
    current_operand->tag                 = NUMBER;
    current_operand->operand_data.number = strtol(trimed + 1, NULL, 16);
    free(trimed);
  }
  else
  {
    current_operand->tag                  = STRING;
    current_operand->operand_data.letters = trimed;
  }
  return current_operand;
}

Token tokenize(char *line)
{
  Token token = calloc(1, sizeof(token_t));
  if (token == NULL)
  {
    perror("Fail to allocate memory in tokenize_instruction");
    exit(EXIT_FAILURE);
  }

  char *      trimed;
  int         count;
  char **     cur_pos        = &line;
  operand_t **operand_holder = &token->operands;

  // initialize opcode
  trim(cur_pos, ' ', &token->opcode);

  // initialize operands
  for (count = 0; *cur_pos != NULL; count++)
  {
    trim(cur_pos, ',', &trimed);
    *operand_holder = gen_operand(trimed);
    operand_holder  = &(*operand_holder)->next;
  }

  token->num_of_operands = count;
  return token;
}
