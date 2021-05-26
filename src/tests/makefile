CC = gcc
CFLAGS  = -Wall -g -D_POSIX_SOURCE -D_BSD_SOURCE -std=c99 -Werror -pedantic
CFLAGS += -Wall -Wextra -Wpedantic \
          -Wformat=2 -Wno-unused-parameter -Wshadow \
          -Wwrite-strings -Wstrict-prototypes -Wold-style-definition \
          -Wredundant-decls -Wnested-externs -Wmissing-include-dirs

OBJ = ../../obj
TEST_BIN = ../../test_bin

all: test_test_utils.test instruction_processor_test.test
	cd $(TEST_BIN) ; python3 testall

test_test_utils.test: test_test_utils.o
	$(CC) -o $(TEST_BIN)/$@ $(OBJ)/$< $(OBJ)/unit_test.o

instruction_processor_test.test: instruction_processor_test.o
	$(CC) -o $(TEST_BIN)/$@ $(OBJ)/$< $(OBJ)/unit_test.o

# %.test: %.o
# 	$(CC) -o $(TEST_BIN)/$@ $(OBJ)/$< $(OBJ)/unit_test.o

%.o: %.c
	$(CC) $(CFLAGS) -o $(OBJ)/$@ -c $<