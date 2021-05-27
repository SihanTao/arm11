CC = gcc
CFLAGS  = -Wall -g -D_POSIX_SOURCE -D_BSD_SOURCE -std=c99 -pedantic #-Werror
CFLAGS += -Wall -Wextra -Wpedantic \
          -Wformat=2 -Wno-unused-parameter -Wshadow \
          -Wwrite-strings -Wstrict-prototypes -Wold-style-definition \
          -Wredundant-decls -Wnested-externs -Wmissing-include-dirs

DIR_SRC = .
DIR_OBJ = ../../obj
DIR_TEST_BIN = ../../test_bin

SRC = $(wildcard $(DIR_SRC)/*.c)
OBJ = $(patsubst %.c,${DIR_OBJ}/%.o,$(notdir ${SRC}))
TEST = $(patsubst %.c,${DIR_TEST_BIN}/%.test,$(notdir ${SRC}))

all: $(TEST)
	cd $(DIR_TEST_BIN) ; python3 testall

$(DIR_TEST_BIN)/unit_test_test.test: $(DIR_OBJ)/unit_test_test.o
	$(CC) -o $@ $< $(DIR_OBJ)/unit_test.o

$(DIR_TEST_BIN)/instruction_processor_test.test: $(DIR_OBJ)/instruction_processor_test.o 
	$(CC) -o $@ $< $(DIR_OBJ)/unit_test.o $(DIR_OBJ)/instruction_processor.o

${DIR_OBJ}/%.o:${DIR_SRC}/%.c
	$(CC) $(CFLAGS) -c  $< -o $@