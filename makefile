CC = gcc
CFLAGS  = -Wall -g -D_POSIX_SOURCE -D_BSD_SOURCE -std=c99 -pedantic #-Werror
CFLAGS += -Wall -Wextra -Wpedantic \
          -Wformat=2 -Wno-unused-parameter -Wshadow \
          -Wwrite-strings -Wstrict-prototypes -Wold-style-definition \
          -Wredundant-decls -Wnested-externs -Wmissing-include-dirs

.SUFFIXES: .c .o

.PHONY: all clean test mkfolders compile_all

DIR_BIN = ./bin
DIR_OBJ = ./obj
DIR_TEST_BIN = ./test_bin
DIR_TEST = ./src/tests
DIR_UTIL = ./src/utils

test: compile_all
	cd $(DIR_TEST); make

clean:
	rm -f $(DIR_BIN)/*.bin
	rm -f $(DIR_TEST_BIN)/*.test
	rm -f $(DIR_OBJ)/*.o

compile_all: mkfolders
	cd $(DIR_UTIL); make

mkfolders: bin obj test_bin

bin:
	mkdir bin

obj:
	mkdir obj

test_bin:
	mkdir test_bin
