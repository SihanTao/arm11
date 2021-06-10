#include "token_stream.h"
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

	add_test("Test count-comma");
	{
		char instruction[] = "mov r1,#3";
		token_t* token = calloc(1, sizeof(token_t));
		char *rest = get_opcode(token, instruction);
		test_int_v(comma_count(rest), 1, "comma == 1");
	}

	add_test("Test split operand field");
	{
		char instruction1[] = "mov r1,#3";
		token_t* token = calloc(1, sizeof(token_t));
		char *rest = get_opcode(token, instruction1);
		char **fields = split_operand_field(rest);
//		print_strings(fields);
		test_string_eq_v(fields[0], "r1", "fields[0] == r1");
		test_string_eq_v(fields[1], "#3", "fields[1] == #3");

		char instruction2[] = "cmp r1,r2";
		char *rest2 = get_opcode(token, instruction2);
		fields = split_operand_field(rest2);
		test_string_eq_v(fields[0], "r1", "fields[0] == r1");
		test_string_eq_v(fields[1], "r2", "fields[1] == r2");
//		print_strings(fields);
	}



	end_all_tests();
}
