#include "assembler_file_loader.h"
#include "../../emulator/utils/unit_test.h"
#include <assert.h>
#include <string.h>

int main(void)
{
	char* filename1 = "add01.s";
	char* filename2 = "and01.s";
	char* filename3 = "beq01.s";

	add_test("Test count line number");
	{
		FILE* fp1 = fopen(filename1, "r");
		FILE* fp2 = fopen(filename2, "r");
		FILE* fp3 = fopen(filename3, "r");
		test_int_v(count_line_number(fp1), 2, "add01.s == 2");
		test_int_v(count_line_number(fp2), 2, "and01.s == 2");
		test_int_v(count_line_number(fp3), 7, "add01.s == 7");
		fclose(fp1);
		fclose(fp2);
		fclose(fp3);
	}

	add_test("Test create and free string array");
	{
		char** p_string = create_string_array(2);
		assert(p_string);
		free_string_array(p_string, 2);
	}

	add_test("Load source file");
	{
		char** strings = load_source_file("add01.s");
		test_int_v(strcmp(strings[0], "mov r1,#1"), 0, "First line");
		test_int_v(strcmp(strings[1], "add r2,r1,#2"), 0, "Second line");
		print_strings(strings);
	}
	end_all_tests();
}
