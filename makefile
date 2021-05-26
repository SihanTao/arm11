CC = gcc
CFLAGS  = -Wall -g -D_POSIX_SOURCE -D_BSD_SOURCE -std=c99 -Werror -pedantic
CFLAGS += -Wall -Wextra -Wpedantic \
          -Wformat=2 -Wno-unused-parameter -Wshadow \
          -Wwrite-strings -Wstrict-prototypes -Wold-style-definition \
          -Wredundant-decls -Wnested-externs -Wmissing-include-dirs

.SUFFIXES: .c .o

.PHONY: all clean

BIN = ./bin
TEST = ./src/tests
UTIL = ./src/utils
OBJ = ./obj
TEST_BIN = ./test_bin

test: compile_utils
	cd $(TEST); make

compile_utils:
	cd $(UTIL); make

clean_test_bin:
	rm -f $(TEST_BIN)/*.test

clean_obj:
	rm -f $(OBJ)/*.o

clean: clean_test_bin clean_obj