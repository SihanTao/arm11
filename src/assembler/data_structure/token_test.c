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
	end_all_tests();
}
