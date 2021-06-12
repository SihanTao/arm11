#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#include "../data_structure/token_stream.h"

#include "tokenize.h"

#include "../../global_utils/unit_test.h"

void print_token(token_t *token)
{
  printf("opcode: %s\n", token->opcode);
  operand_t *cur_operand = token->operands;
  while (cur_operand != NULL)
  {
    printf("The token tag is: %s\n", cur_operand->tag ? "STRING" : "NUMBER");
    if (cur_operand->tag)
    {
      printf("Letters = %s\n", cur_operand->operand_data.letters);
    }
    else
    {
      printf("Number = %d\n", cur_operand->operand_data.number);
    }
    cur_operand = cur_operand->next;
  }
  printf("\n");
}

int main(void)
{
  add_test("trim test");
  {
    char * string  = "this is,a,string";
    char **cur_pos = &string;
    char * dst;

    trim(cur_pos, ' ', &dst);
    test_true(*cur_pos != NULL);
    test_true(strcmp(dst, "this") == 0);
    free(dst);

    trim(cur_pos, ',', &dst);
    test_true(*cur_pos != NULL);
    test_true(strcmp(dst, "is") == 0);
    free(dst);

    trim(cur_pos, ',', &dst);
    test_true(*cur_pos != NULL);
    test_true(strcmp(dst, "a") == 0);
    free(dst);

    trim(cur_pos, ',', &dst);
    test_true(*cur_pos == NULL);
    test_true(strcmp(dst, "string") == 0);
    free(dst);
  }

  add_test("tokenize test");
  {
    char *string = "this is,a,string";
    // example: beq01.s
    char instructions[][10]
        = { "mov r1,#1", "cmp r1,r2", "beq foo", "mov r3,#3" };

    token_t *token = tokenize(string);
    print_token(tokenize(string));
    free_token(token);

    token = tokenize(instructions[0]);
    print_token(token);
    free_token(token);

    token = tokenize(instructions[1]);
    print_token(token);
    free_token(token);

    token = tokenize(instructions[2]);
    print_token(token);
    free_token(token);

    token = tokenize(instructions[3]);
    print_token(token);
    free_token(token);
  }
  end_all_tests();
}