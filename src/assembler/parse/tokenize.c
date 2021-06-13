#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "../data_structure/token_stream.h"
#include "../parse/mnemonic.h"

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

mnemonic_type find_type(char *str_mne)
{
  if (strcmp(str_mne, "add") == 0)
  {
    return ADD_M;
  }
  else if (strcmp(str_mne, "sub") == 0)
  {
    return SUB_M;
  }
  else if (strcmp(str_mne, "rsb") == 0)
  {
    return RSB_M;
  }
  else if (strcmp(str_mne, "and") == 0)
  {
    return AND_M;
  }
  else if (strcmp(str_mne, "eor") == 0)
  {
    return EOR_M;
  }
  else if (strcmp(str_mne, "mov") == 0)
  {
    return MOV_M;
  }
  else if (strcmp(str_mne, "tst") == 0)
  {
    return TST_M;
  }
  else if (strcmp(str_mne, "teq") == 0)
  {
    return TEQ_M;
  }
  else if (strcmp(str_mne, "cmp") == 0)
  {
    return CMP_M;
  }
  else if (strcmp(str_mne, "mul") == 0)
  {
    return MUL_M;
  }
  else if (strcmp(str_mne, "mla") == 0)
  {
    return MLA_M;
  }
  else if (strcmp(str_mne, "ldr") == 0)
  {
    return LDR_M;
  }
  else if (strcmp(str_mne, "str") == 0)
  {
    return STR_M;
  }
  else if (strcmp(str_mne, "beq") == 0)
  {
    return BEQ_M;
  }
  else if (strcmp(str_mne, "bne") == 0)
  {
    return BGE_M;
  }
  else if (strcmp(str_mne, "blt") == 0)
  {
    return BLT_M;
  }
  else if (strcmp(str_mne, "bgt") == 0)
  {
    return BGT_M;
  }
  else if (strcmp(str_mne, "ble") == 0)
  {
    return BLE_M;
  }
  else if (strcmp(str_mne, "b") == 0)
  {
    return B_M;
  }
  else if (strcmp(str_mne, "lsl") == 0)
  {
    return BLT_M;
  }
  else if (strcmp(str_mne, "anded") == 0)
  {
    return ANDEQ_M;
  }
  else
  {
    perror("Error! unsupported operation");
    exit(EXIT_FAILURE);
  }
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
  trim(cur_pos, ' ', &trimed);
  token->type = find_type(trimed);

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
