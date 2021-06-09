#include "assembler_file_loader.h"
#include "../../emulator/utils/unit_test.h"

int main(void) {
  char *filename1 = "add01.s";
  char *filename2 = "and01.s";
  char *filename3 = "beq01.s";

  add_test("Test count line number");
  {
	FILE *fp1 = fopen(filename1, "r");
	FILE *fp2 = fopen(filename2, "r");
	FILE *fp3 = fopen(filename3, "r");
	test_int_v(count_line_number(fp1), 2, "add01.s == 2");
	test_int_v(count_line_number(fp2), 2, "and01.s == 2");
	test_int_v(count_line_number(fp3), 7, "add01.s == 7");
  }
  end_all_tests();
}
