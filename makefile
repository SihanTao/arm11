CC = gcc
CFLAGS  = -g -D_POSIX_SOURCE -D_BSD_SOURCE -std=c99 -pedantic #-Werror

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

test: clean compile_all
	cd $(DIR_EM_TESTS); make

clean:
	rm -f $(DIR_BIN)/*.bin
	rm -f $(DIR_TEST_BIN)/*.test
	rm -f $(DIR_OBJ)/*.o

emulate:
	cd $(DIR_EM_MAIN); make emulate

assemble:
	cd src/assembler/main; make assemble

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
