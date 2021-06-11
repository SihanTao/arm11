#include "../data_structure/token_stream.h"
#include "../../emulator/utils/unit_test.h"
#include <stdio.h>
#include "../file_loader/assembler_file_loader.h"

int main(void){
	add_test("Test get opcode:");
	{
		char instruction[] = "mov r1,#3";
		token_t* token = calloc(1, sizeof(token_t));
		char *rest = get_opcode(token, instruction);
//		printf("%s\n", token->opcode);
//		printf("%s\n", rest);
		test_string_eq_v(token->opcode, "mov", "opcode == mov");
		test_string_eq_v(rest, "r1,#3", "rest == r1,#3");
	}

	add_test("Test count num operand");
	{
		char instruction[] = "mov r1,#3";
		token_t* token = calloc(1, sizeof(token_t));
		char *rest = get_opcode(token, instruction);
		test_int_v(count_num_operand(rest), 2, "number == 2");
	}

	add_test("Test split operand field");
	{
		char instruction1[] = "mov r1,#3";
		token_t* token = calloc(1, sizeof(token_t));
		char *rest = get_opcode(token, instruction1);
		int length = count_num_operand(rest);
		char **fields = split_operand_field(rest, length);
//		print_strings(fields);
		test_string_eq_v(fields[0], "r1", "fields[0] == r1");
		test_string_eq_v(fields[1], "#3", "fields[1] == #3");

		char instruction2[] = "cmp r1,r2";
		char *rest2 = get_opcode(token, instruction2);
		length = count_num_operand(rest2);
		fields = split_operand_field(rest2, length);
		test_string_eq_v(fields[0], "r1", "fields[0] == r1");
		test_string_eq_v(fields[1], "r2", "fields[1] == r2");
//		print_strings(fields);
	}


//	printf("Test strtol\n");
//	char eg[] = "0x42";
//	printf("%ld\n", strtol(eg, NULL, 0));
//	char eg1[] = "#3";
//	printf("%ld\n", strtol(eg1+1, NULL, 0));
//	char *field[] = {"0x42","#3"};
//	char **cursor = field;
//	printf("%ld\n", strtol(*cursor, NULL, 0));
//	cursor++;
//	printf("%ld\n", strtol(*cursor+1, NULL, 0));

	add_test("Test print token:");
	{
		operand_t operands[] = {{ .tag = STRING, "r1" },
								{.tag = NUMBER,.operand_data.number = 3}};
		token_t token = {.real_address = 0, .opcode = "mov",
						 .operands = operands,
						 .num_operand = 2};
		print_token(&token);
	}

	add_test("Test set_token_operand:");
	{
		char instruction1[] = "mov r1,#3";
		token_t* token = calloc(1, sizeof(token_t));
		char *rest = get_opcode(token, instruction1);
		int length = count_num_operand(rest);
		char **fields = split_operand_field(rest, length);
		set_token_operand(token, fields, length);
//		print_token(token, length);
		operand_t op1 = token->operands[0];
		operand_t op2 = token->operands[1];
//		printf("%s\n", op1.operand_data.letters);
		test_string_eq_v(op1.operand_data.letters, "r1", " ");
		test_int_v(token->operands[1].operand_data.number, 3, "");
	}

	add_test("Test tokenize instruction");
	{
		// example: beq01.s
		char instructions[][10] = {
			"mov r1,#1", "cmp r1,r2", "beq foo", "mov r3,#3", "foo:"
		};
		token_t* token = tokenize_instruction(instructions[0], 0);
		print_token(token);

		token = tokenize_instruction(instructions[1], 1);
		print_token(token);

		token = tokenize_instruction(instructions[2], 2);
		print_token(token);

		token = tokenize_instruction(instructions[3], 3);
		print_token(token);

		token = tokenize_instruction(instructions[4], 4);
		print_token(token);

	}
	end_all_tests();
}
