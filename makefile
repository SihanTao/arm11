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
DIR_EM_MAIN = ./src/emulator/main
DIR_EM_DECODE = ./src/emulator/decode
DIR_EM_UTILS = ./src/emulator/utils
DIR_EM_EXECUTE = ./src/emulator/execute
DIR_EM_TESTS = ./src/emulator/tests
DIR_EM_UTILS = ./src/emulator/utils
DIR_GLB_UTILS = ./src/global_utils

# I don't know why but if don't make clean, some file will not recompile
test: clean compile_all
	cd $(DIR_EM_TESTS); make

clean:
	rm -f $(DIR_BIN)/*.bin
	rm -f $(DIR_TEST_BIN)/*.test
	rm -f $(DIR_OBJ)/*.o

link:
	cd $(DIR_EM_MAIN); make main

compile_all: mkfolders
	cd $(DIR_EM_UTILS); make
	cd $(DIR_EM_DECODE); make
	cd $(DIR_EM_EXECUTE); make
	cd $(DIR_EM_MAIN); make compile
	cd $(DIR_GLB_UTILS); make compile_all

mkfolders: bin obj test_bin

bin:
	mkdir bin

obj:
	mkdir obj

test_bin:
	mkdir test_bin
